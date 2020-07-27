#include "pch.h"
#include "FullScreenQuadMaterial.h"
#include "Game.h"
#include "VertexDeclarations.h"
#include "Shader.h"
#include "Texture.h"

namespace Library
{
	FullScreenQuadMaterial::FullScreenQuadMaterial(ContentManager& contentManager, RenderingManager& renderingManager, const Sampler::Type samplerType) :
		Material(contentManager, renderingManager),
		mSamplerState(renderingManager.GetSamplerState(samplerType))
	{
	}

	Sampler* FullScreenQuadMaterial::SamplerState() const
	{
		return mSamplerState;
	}

	void FullScreenQuadMaterial::SetSamplerState(const gsl::not_null<Sampler*> samplerState)
	{
		mSamplerState = samplerState;
		SetShaderResource(ShaderStages::PS, *mSamplerState);
	}

	void FullScreenQuadMaterial::SetTexture(gsl::not_null<Texture*> texture)
	{
		if (mTextures.Size() > 1 || (!mTextures.IsEmpty() && (mTextures.Front() != texture)))
		{
			mTextures.Clear();
			ClearShaderResources(ShaderStages::PS);
		}

		if (texture != nullptr)
		{
			mTextures.EmplaceBack(texture);
			AddShaderResource(ShaderStages::PS, *texture);
		}
	}

	void FullScreenQuadMaterial::SetTextures(const gsl::span<gsl::not_null<Texture*>> textures)
	{
		mTextures.Clear();
		mTextures.Reserve(textures.size());
		mTextures.Insert(mTextures.begin(), textures.begin(), textures.end());

		ClearShaderResources(ShaderStages::PS);
		AddShaderResources(ShaderStages::PS, gsl::make_span(mTextures.Data(), mTextures.Size()));
	}

	uint32_t FullScreenQuadMaterial::VertexSize() const
	{
		return sizeof(VertexPositionTexture);
	}

	void FullScreenQuadMaterial::Initialize()
	{
		Material::Initialize();

		auto vertexShader = mContentManager.Load<VertexShader>(L"Shaders\\TexturedQuadPassThroughVS.cso");
		vertexShader->CreateInputLayout<VertexPositionTexture>(mRenderingManager);
		SetShader(vertexShader);

		assert(mSamplerState);
	AddShaderResource(ShaderStages::PS, *mSamplerState);
	}
}
