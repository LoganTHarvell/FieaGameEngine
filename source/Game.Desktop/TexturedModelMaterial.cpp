#include "pch.h"
#include "TexturedModelMaterial.h"
#include "Game.h"
#include "VertexDeclarations.h"
#include "Shader.h"
#include "Texture.h"

using namespace std;
using namespace gsl;
using namespace winrt;
using namespace DirectX;

namespace Library
{
	TexturedModelMaterial::TexturedModelMaterial(ContentManager& contentManager, RenderingManager& renderingManager, shared_ptr<Texture2D> texture) :
		Material(contentManager, renderingManager), mTexture(move(texture)), mSamplerState(renderingManager.GetSamplerState(Sampler::Type::TrilinearClamp))
	{
		if (mTexture != nullptr)
		{
			AddShaderResource(ShaderStages::PS, *mTexture);
		}
	}

	Sampler* TexturedModelMaterial::SamplerState() const
	{
		return mSamplerState;
	}

	void TexturedModelMaterial::SetSamplerState(const gsl::not_null<Sampler*> samplerState)
	{
		assert(samplerState != nullptr);
		mSamplerState = move(samplerState);
		SetShaderResource(ShaderStages::PS, *mSamplerState);
	}

	shared_ptr<Texture2D> TexturedModelMaterial::Texture() const
	{
		return mTexture;
	}

	void TexturedModelMaterial::SetTexture(shared_ptr<Texture2D> texture)
	{
		mTexture = move(texture);
		ClearShaderResources(ShaderStages::PS);
		
		if (mTexture != nullptr)
		{
			AddShaderResource(ShaderStages::PS, *mTexture);
		}
	}

	uint32_t TexturedModelMaterial::VertexSize() const
	{
		return sizeof(VertexPositionTexture);
	}

	void TexturedModelMaterial::Initialize()
	{
		Material::Initialize();

		auto vertexShader = mContentManager.Load<VertexShader>(L"Shaders\\TexturedModelVS.cso");
		SetShader(vertexShader);

		const auto pixelShader = mContentManager.Load<PixelShader>(L"Shaders\\TexturedModelPS.cso");
		SetShader(pixelShader);

		vertexShader->CreateInputLayout<VertexPositionTexture>(mRenderingManager);

		mVertexCBufferPerObject = mRenderingManager.CreateConstantBuffer(sizeof(XMFLOAT4X4));

		assert(mVertexCBufferPerObject);
		assert(mSamplerState);
		
		AddShaderResource(ShaderStages::VS, *mVertexCBufferPerObject);
		AddShaderResource(ShaderStages::PS, *mSamplerState);
	}

	void TexturedModelMaterial::UpdateTransform(CXMMATRIX worldViewProjectionMatrix)
	{
		assert(mVertexCBufferPerObject);
		mRenderingManager.UpdateBuffer(*mVertexCBufferPerObject, worldViewProjectionMatrix.r, sizeof(CXMMATRIX));
	}
}