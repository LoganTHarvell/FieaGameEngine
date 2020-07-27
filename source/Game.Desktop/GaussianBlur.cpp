#include "pch.h"
//#include "GaussianBlur.h"
//#include "FullScreenQuadMaterial.h"
//#include "Game.h"
//#include "VectorHelper.h"
//#include "Shader.h"
//
//using namespace std;
//using namespace gsl;
//using namespace winrt;
//using namespace DirectX;
//
//namespace Library
//{
//    GaussianBlur::GaussianBlur(Game& game, float blurAmount) :
//		DrawableGameComponent(game),
//		mBlurAmount(blurAmount), mFullScreenQuad(game),
//		mHorizontalBlurTarget(game), mVerticalBlurTarget(game)
//    {
//    }
//
//    ID3D11ShaderResourceView* GaussianBlur::SceneTexture() const
//    {
//        return mSceneTexture.get();
//    }
//
//    void GaussianBlur::SetSceneTexture(com_ptr<ID3D11ShaderResourceView> sceneTexture)
//    {
//		mSceneTexture = sceneTexture;
//    }
//
//	ID3D11ShaderResourceView* GaussianBlur::OutputTexture() const
//	{
//		return mOutputTexture.get();
//	}
//
//    float GaussianBlur::BlurAmount() const
//    {
//        return mBlurAmount;
//    }
//
//    void GaussianBlur::SetBlurAmount(float blurAmount)
//    {
//		mBlurAmount = blurAmount;
//		InitializeSampleWeights();
//    }
//
//    void GaussianBlur::Initialize()
//    {
//		mFullScreenQuad.Initialize();	
//		
//		mHorizontalCBufferPerFrameMaterialCallback = [&]
//		{
//			auto psConstantBuffers = mHorizontalCBufferPerFrame.get();
//			mGame->Direct3DDeviceContext()->PSSetConstantBuffers(0, 1, &psConstantBuffers);
//		};
//
//		mVerticalCBufferPerFrameMaterialCallback = [&]
//		{
//			auto psConstantBuffers = mHorizontalCBufferPerFrame.get();
//			mGame->Direct3DDeviceContext()->PSSetConstantBuffers(0, 1, &psConstantBuffers);
//		};
//
//		mBlurPixelShader = mGame->Content().Load<PixelShader>(L"Shaders\\GaussianBlurPS.cso");
//		mNoBlurPixelShader = mGame->Content().Load<PixelShader>(L"Shaders\\TexturedModelPS.cso");
//
//		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
//		constantBufferDesc.ByteWidth = sizeof(PixelCBufferPerFrame);
//		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, mHorizontalCBufferPerFrame.put()), "ID3D11Device::CreateMeshIndexBuffer() failed.");
//		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, mVerticalCBufferPerFrame.put()), "ID3D11Device::CreateMeshIndexBuffer() failed.");
//
//		InitializeSampleWeights();
//		InitializeSampleOffsets();
//    }
//
//    void GaussianBlur::Draw(const GameTime& gameTime)
//    {
//		auto fullScreenQuadMaterial = mFullScreenQuad.Material();
//        if (mBlurAmount > 0.0f)
//        {
//            // Horizontal blur
//            mHorizontalBlurTarget.Begin();
//			auto direct3DDeviceContext = mGame->Direct3DDeviceContext();
//            direct3DDeviceContext->ClearRenderTargetView(mHorizontalBlurTarget.RenderTargetView().get(), Colors::Purple.f);
//            direct3DDeviceContext->ClearDepthStencilView(mHorizontalBlurTarget.DepthStencilView().get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//			
//			fullScreenQuadMaterial->SetShader(mBlurPixelShader);
//			fullScreenQuadMaterial->SetTexture(mSceneTexture.get());
//			fullScreenQuadMaterial->SetUpdateMaterialCallback(mHorizontalCBufferPerFrameMaterialCallback);
//            mFullScreenQuad.Draw(gameTime);
//            mHorizontalBlurTarget.End();
//			fullScreenQuadMaterial->UnbindShaderResources<1>(ShaderStages::PS);
//
//			// Vertical blur for the final image
//			fullScreenQuadMaterial->SetTexture(mHorizontalBlurTarget.OutputTexture().get());
//			fullScreenQuadMaterial->SetUpdateMaterialCallback(mVerticalCBufferPerFrameMaterialCallback);
//            mFullScreenQuad.Draw(gameTime);
//        }
//        else
//        {			
//			fullScreenQuadMaterial->SetShader(mNoBlurPixelShader);
//			fullScreenQuadMaterial->SetTexture(mSceneTexture.get());	
//            mFullScreenQuad.Draw(gameTime);
//        }
//
//		fullScreenQuadMaterial->UnbindShaderResources<1>(ShaderStages::PS);
//    }	
//
//	void GaussianBlur::DrawToTexture(const GameTime& gameTime)
//	{
//		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();
//		auto fullScreenQuadMaterial = mFullScreenQuad.Material();
//		if (mBlurAmount > 0.0f)
//		{
//			// Horizontal blur
//			mHorizontalBlurTarget.Begin();
//			direct3DDeviceContext->ClearRenderTargetView(mHorizontalBlurTarget.RenderTargetView().get(), Colors::Purple.f);
//			direct3DDeviceContext->ClearDepthStencilView(mHorizontalBlurTarget.DepthStencilView().get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//
//			fullScreenQuadMaterial->SetShader(mBlurPixelShader);
//			fullScreenQuadMaterial->SetTexture(mSceneTexture.get());
//			fullScreenQuadMaterial->SetUpdateMaterialCallback(mHorizontalCBufferPerFrameMaterialCallback);
//			mFullScreenQuad.Draw(gameTime);
//			mHorizontalBlurTarget.End();
//			fullScreenQuadMaterial->UnbindShaderResources<1>(ShaderStages::PS);
//
//			// Vertical blur for the final image (with vertical blur render target for output to texture)
//			mVerticalBlurTarget.Begin();
//			direct3DDeviceContext->ClearRenderTargetView(mVerticalBlurTarget.RenderTargetView().get(), Colors::Purple.f);
//			direct3DDeviceContext->ClearDepthStencilView(mVerticalBlurTarget.DepthStencilView().get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//
//			fullScreenQuadMaterial->SetTexture(mHorizontalBlurTarget.OutputTexture().get());
//			fullScreenQuadMaterial->SetUpdateMaterialCallback(mVerticalCBufferPerFrameMaterialCallback);
//			mFullScreenQuad.Draw(gameTime);
//
//			mVerticalBlurTarget.End();
//			fullScreenQuadMaterial->UnbindShaderResources<1>(ShaderStages::PS);
//
//			mOutputTexture = mVerticalBlurTarget.OutputTexture();
//		}
//		else
//		{
//			// Repurposing the horizontal blur target for output to texture
//			mHorizontalBlurTarget.Begin();
//			direct3DDeviceContext->ClearRenderTargetView(mHorizontalBlurTarget.RenderTargetView().get(), Colors::Purple.f);
//			direct3DDeviceContext->ClearDepthStencilView(mHorizontalBlurTarget.DepthStencilView().get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//
//			fullScreenQuadMaterial->SetShader(mNoBlurPixelShader);
//			fullScreenQuadMaterial->SetTexture(mSceneTexture.get());
//			mFullScreenQuad.Draw(gameTime);
//			mHorizontalBlurTarget.End();
//			fullScreenQuadMaterial->UnbindShaderResources<1>(ShaderStages::PS);
//			
//			mOutputTexture = mHorizontalBlurTarget.OutputTexture();
//		}
//	}
//
//	void GaussianBlur::InitializeSampleOffsets()
//	{
//		SIZE renderTargetSize = mGame->RenderTargetSize();
//		float horizontalPixelSize = 1.0f / renderTargetSize.cx;
//		float verticalPixelSize = 1.0f / renderTargetSize.cy;
//
//		mHorizontalCBufferData.SampleOffsets[0].Offset = Vector2Helper::Zero;
//		mVerticalCBufferData.SampleOffsets[0].Offset = Vector2Helper::Zero;
//
//		for (UINT i = 0; i < SampleCount / 2; i++)
//		{
//			float sampleOffset = i * 2 + 1.5f;
//			float horizontalOffset = horizontalPixelSize * sampleOffset;
//			float verticalOffset = verticalPixelSize * sampleOffset;
//
//			mHorizontalCBufferData.SampleOffsets[i * 2 + 1].Offset = XMFLOAT2(horizontalOffset, 0.0f);
//			mHorizontalCBufferData.SampleOffsets[i * 2 + 2].Offset = XMFLOAT2(-horizontalOffset, 0.0f);
//
//			mVerticalCBufferData.SampleOffsets[i * 2 + 1].Offset = XMFLOAT2(0.0f, verticalOffset);
//			mVerticalCBufferData.SampleOffsets[i * 2 + 2].Offset = XMFLOAT2(0.0f, -verticalOffset);
//		}
//
//		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();
//		direct3DDeviceContext->UpdateSubresource(mHorizontalCBufferPerFrame.get(), 0, nullptr, &mHorizontalCBufferData, 0, 0);
//		direct3DDeviceContext->UpdateSubresource(mVerticalCBufferPerFrame.get(), 0, nullptr, &mVerticalCBufferData, 0, 0);
//	}
//
//	void GaussianBlur::InitializeSampleWeights()
//	{
//		mHorizontalCBufferData.SampleWeights[0].Weight = GetWeight(0);
//
//		float totalWeight = mHorizontalCBufferData.SampleWeights[0].Weight;
//		for (uint32_t i = 0; i < SampleCount / 2; i++)
//		{
//			float weight = GetWeight((float)i + 1);
//			mHorizontalCBufferData.SampleWeights[i * 2 + 1].Weight = weight;
//			mHorizontalCBufferData.SampleWeights[i * 2 + 2].Weight = weight;
//			totalWeight += weight * 2;
//		}
//
//		// Normalize the weights so that they sum to one
//		for (uint32_t i = 0; i < SampleCount; i++)
//		{
//			mHorizontalCBufferData.SampleWeights[i].Weight /= totalWeight;
//		}
//
//		memcpy(mVerticalCBufferData.SampleWeights, mHorizontalCBufferData.SampleWeights, sizeof(PixelCBufferPerFrame::_SampleWeight) * SampleCount);
//
//		auto direct3DDeviceContext = mGame->Direct3DDeviceContext();
//		direct3DDeviceContext->UpdateSubresource(mHorizontalCBufferPerFrame.get(), 0, nullptr, &mHorizontalCBufferData, 0, 0);
//		direct3DDeviceContext->UpdateSubresource(mVerticalCBufferPerFrame.get(), 0, nullptr, &mVerticalCBufferData, 0, 0);
//	}
//
//	float GaussianBlur::GetWeight(float x) const
//	{
//		return (float)(exp(-(x * x) / (2 * mBlurAmount * mBlurAmount)));
//	}
//}