#include "pch.h"
#include "Bloom.h"
#include "FullScreenQuadMaterial.h"
#include "Game.h"
#include "VectorHelper.h"
#include "PixelShader.h"
#include "PixelShaderReader.h"
#include "Utility.h"

using namespace std;
using namespace gsl;
using namespace winrt;
using namespace DirectX;

namespace Library
{
	const map<BloomDrawModes, string> Bloom::DrawModeDisplayNames
	{
		{ BloomDrawModes::Normal, "Normal"s },
		{ BloomDrawModes::GlowMap, "Glow Map"s },
		{ BloomDrawModes::BlurredGlowMap, "Blurred Glow Map"s }
	};

    Bloom::Bloom(Game& game, const BloomSettings& bloomSettings) :
		DrawableGameComponent(game),
		mFullScreenQuad(game), mBloomSettings(bloomSettings),
		mPixelCBufferPerObjectData(bloomSettings),
		mRenderTarget(game), mGaussianBlur(game, bloomSettings.BlurAmount)
    {
    }

    ID3D11ShaderResourceView* Bloom::SceneTexture() const
    {
        return mSceneTexture.get();
    }

    void Bloom::SetSceneTexture(com_ptr<ID3D11ShaderResourceView> sceneTexture)
    {
		mSceneTexture = sceneTexture;
    }

	const BloomSettings& Bloom::GetBloomSettings() const
	{
		return mBloomSettings;
	}

	void Bloom::SetBloomSettings(const BloomSettings& bloomSettings)
	{
		mBloomSettings = bloomSettings;
		mPixelCBufferPerObjectData = bloomSettings;
		mGame->Direct3DDeviceContext()->UpdateSubresource(mPixelCBufferPerObject.get(), 0, nullptr, &mPixelCBufferPerObjectData, 0, 0);
		mGaussianBlur.SetBlurAmount(bloomSettings.BlurAmount);
	}

	BloomDrawModes Bloom::DrawMode() const
	{
		return mDrawMode;
	}

	const string& Bloom::DrawModeString() const
	{
		return DrawModeDisplayNames.at(mDrawMode);
	}

	void Bloom::SetDrawMode(BloomDrawModes drawMode)
	{
		mDrawMode = drawMode;
	}

    void Bloom::Initialize()
    {
		mFullScreenQuad.Initialize();
		auto fullScreenQuadMaterial = mFullScreenQuad.Material();		

		mGaussianBlur.SetSceneTexture(mRenderTarget.OutputTexture());
		mGaussianBlur.Initialize();
		
		// Create the bloom shader with class linkage
		auto direct3DDevice = mGame->Direct3DDevice();
		auto classLinkage = Shader::CreateClassLinkage(direct3DDevice);
		PixelShaderWithClassLinkageReader pixelShaderContentReader(*mGame, classLinkage);
		auto pixelShader = mGame->Content().Load<PixelShader>(L"Shaders\\BloomPS.cso", false, pixelShaderContentReader);
		fullScreenQuadMaterial->SetShader(ShaderStages::PS, pixelShader);

		ThrowIfFailed(classLinkage->CreateClassInstance("ExtractBloomShader", 0, 0, 0, 0, mShaderClassInstances[BloomShaderClass::Extract].put()));
		ThrowIfFailed(classLinkage->CreateClassInstance("CompositeBloomShader", 0, 0, 0, 0, mShaderClassInstances[BloomShaderClass::Composite].put()));
		ThrowIfFailed(classLinkage->CreateClassInstance("NoBloomShader", 0, 0, 0, 0, mShaderClassInstances[BloomShaderClass::NoBloom].put()));
		
		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
		constantBufferDesc.ByteWidth = sizeof(PixelCBufferPerObject);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mPixelCBufferPerObject.put()), "ID3D11Device::CreateMeshIndexBuffer() failed.");
		fullScreenQuadMaterial->AddConstantBuffer(ShaderStages::PS, mPixelCBufferPerObject.get());
		mGame->Direct3DDeviceContext()->UpdateSubresource(mPixelCBufferPerObject.get(), 0, nullptr, &mPixelCBufferPerObjectData, 0, 0);

		using namespace std::placeholders;
		mDrawFunctions[BloomDrawModes::Normal] = bind(&Bloom::DrawNormal, this, _1);
		mDrawFunctions[BloomDrawModes::GlowMap] = bind(&Bloom::DrawGlowMap, this, _1);
		mDrawFunctions[BloomDrawModes::BlurredGlowMap] = bind(&Bloom::DrawBlurredGlowMap, this, _1);
    }

    void Bloom::Draw(const GameTime& gameTime)
    {
		mDrawFunctions.at(mDrawMode)(gameTime);
    }

	void Bloom::DrawNormal(const GameTime& gameTime)
	{
		auto fullScreenQuadMaterial = mFullScreenQuad.Material();
		if (mPixelCBufferPerObjectData.BloomThreshold < 1.0f)
		{
			// Extract the bright spots in the scene
			mRenderTarget.Begin();
			mGame->Direct3DDeviceContext()->ClearRenderTargetView(mRenderTarget.RenderTargetView().get(), Colors::Purple.f);
			mGame->Direct3DDeviceContext()->ClearDepthStencilView(mRenderTarget.DepthStencilView().get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
						
			fullScreenQuadMaterial->SetShaderClassInstance(ShaderStages::PS, mShaderClassInstances.at(BloomShaderClass::Extract).get());
			fullScreenQuadMaterial->SetTexture(mSceneTexture.get());
			
			mFullScreenQuad.Draw(gameTime);
			mRenderTarget.End();
			fullScreenQuadMaterial->UnbindShaderResources<1>(ShaderStages::PS);

			// Blur the bright spots in the scene
			mGaussianBlur.DrawToTexture(gameTime);
			
			// Combine the original scene with the blurred bright spot image
			fullScreenQuadMaterial->SetShaderClassInstance(ShaderStages::PS, mShaderClassInstances.at(BloomShaderClass::Composite).get());

			ID3D11ShaderResourceView* psShaderResources[] = { mSceneTexture.get(), mGaussianBlur.OutputTexture() };			
			const span<ID3D11ShaderResourceView*> textures{ psShaderResources };
			fullScreenQuadMaterial->SetTextures(textures);
			mFullScreenQuad.Draw(gameTime);
			fullScreenQuadMaterial->UnbindShaderResources<2>(ShaderStages::PS);
		}
		else
		{
			fullScreenQuadMaterial->SetShaderClassInstance(ShaderStages::PS, mShaderClassInstances.at(BloomShaderClass::NoBloom).get());
			fullScreenQuadMaterial->SetTexture(mSceneTexture.get());

			mFullScreenQuad.Draw(gameTime);
			fullScreenQuadMaterial->UnbindShaderResources<2>(ShaderStages::PS);
		}
	}

	void Bloom::DrawGlowMap(const GameTime& gameTime)
	{
		auto fullScreenQuadMaterial = mFullScreenQuad.Material();
		fullScreenQuadMaterial->SetShaderClassInstance(ShaderStages::PS, mShaderClassInstances.at(BloomShaderClass::Extract).get());
		fullScreenQuadMaterial->SetTexture(mSceneTexture.get());
		mFullScreenQuad.Draw(gameTime);
		fullScreenQuadMaterial->UnbindShaderResources<1>(ShaderStages::PS);
	}

	void Bloom::DrawBlurredGlowMap(const GameTime& gameTime)
	{
		// Extract the bright spots in the scene
		mRenderTarget.Begin();
		mGame->Direct3DDeviceContext()->ClearRenderTargetView(mRenderTarget.RenderTargetView().get(), Colors::Purple.f);
		mGame->Direct3DDeviceContext()->ClearDepthStencilView(mRenderTarget.DepthStencilView().get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		auto fullScreenQuadMaterial = mFullScreenQuad.Material();
		fullScreenQuadMaterial->SetShaderClassInstance(ShaderStages::PS, mShaderClassInstances.at(BloomShaderClass::Extract).get());
		fullScreenQuadMaterial->SetTexture(mSceneTexture.get());

		mFullScreenQuad.Draw(gameTime);
		mRenderTarget.End();
		mFullScreenQuad.Material()->UnbindShaderResources<1>(ShaderStages::PS);
		
		// Blur the bright spots in the scene
		mGaussianBlur.Draw(gameTime);
	}
}