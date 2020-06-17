#include "pch.h"
#include "FullScreenQuadMaterial.h"
#include "Game.h"
#include "GameException.h"
#include "VertexDeclarations.h"
#include "VertexShader.h"

using namespace std;
using namespace gsl;
using namespace winrt;
using namespace DirectX;

namespace Library
{
	FullScreenQuadMaterial::FullScreenQuadMaterial(ContentManager& contentManager, RenderingManager& renderingManager, const winrt::com_ptr<ID3D11SamplerState>& samplerState) :
		Material(contentManager, renderingManager),
		mSamplerState(samplerState)
	{
	}

	com_ptr<ID3D11SamplerState> FullScreenQuadMaterial::SamplerState() const
	{
		return mSamplerState;
	}

	void FullScreenQuadMaterial::SetSamplerState(const com_ptr<ID3D11SamplerState>& samplerState)
	{
		mSamplerState = samplerState;
		Material::SetSamplerState(ShaderStages::PS, mSamplerState.get());
	}

	void FullScreenQuadMaterial::SetTexture(ID3D11ShaderResourceView* texture)
	{
		if (mTextures.size() > 1 || (!mTextures.empty() && (mTextures.front() != texture)))
		{
			mTextures.clear();
			Material::ClearShaderResources(ShaderStages::PS);
		}

		if (texture != nullptr)
		{
			mTextures.push_back(texture);
			Material::AddShaderResource(ShaderStages::PS, texture);
		}
	}

	void FullScreenQuadMaterial::SetTextures(gsl::span<ID3D11ShaderResourceView*> textures)
	{
		mTextures = move(vector<ID3D11ShaderResourceView*>(textures.begin(), textures.end()));
		ClearShaderResources(ShaderStages::PS);
		AddShaderResources(ShaderStages::PS, textures);
	}

	uint32_t FullScreenQuadMaterial::VertexSize() const
	{
		return sizeof(VertexPositionTexture);
	}

	void FullScreenQuadMaterial::Initialize()
	{
		Material::Initialize();

		auto vertexShader = mContentManager.Load<VertexShader>(L"Shaders\\TexturedQuadPassThroughVS.cso");
		SetShader(vertexShader);

		auto direct3DDevice = mRenderingManager.GetDevice().DevicePtr;
		vertexShader->CreateInputLayout<VertexPositionTexture>(direct3DDevice);
		SetInputLayout(vertexShader->InputLayout());

		AddSamplerState(ShaderStages::PS, mSamplerState.get());
	}
}