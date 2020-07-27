#include "pch.h"
#include "SkyboxMaterial.h"

#include <utility>
#include "Game.h"
#include "Shader.h"
#include "VertexDeclarations.h"
#include "Texture.h"
#include "RasterizerStates.h"

namespace Library
{
	SkyboxMaterial::SkyboxMaterial(ContentManager& contentManager, RenderingManager& renderingManager, std::shared_ptr<TextureCube> texture, const Sampler::Type samplerType) :
		Material(contentManager, renderingManager),
		mTexture(std::move(texture)), mSamplerState(renderingManager.GetSamplerState(samplerType))
	{
	}

	Sampler* SkyboxMaterial::SamplerState() const
	{
		return mSamplerState;
	}

	void SkyboxMaterial::SetSamplerState(const gsl::not_null<Sampler*> samplerState)
	{
		mSamplerState = samplerState;
		SetShaderResource(ShaderStages::PS, *mSamplerState);
	}

	std::shared_ptr<TextureCube> SkyboxMaterial::Texture() const
	{
		return mTexture;
	}

	void SkyboxMaterial::SetTexture(std::shared_ptr<TextureCube> texture)
	{
		assert(texture != nullptr);
		mTexture = move(texture);
		SetShaderResource(ShaderStages::PS, *mTexture);
	}

	uint32_t SkyboxMaterial::VertexSize() const
	{
		return sizeof(VertexPosition);
	}

	void SkyboxMaterial::Initialize()
	{
		Material::Initialize();

		auto vertexShader = mContentManager.Load<VertexShader>(L"Shaders\\SkyboxVS.cso");
		SetShader(vertexShader);

		const auto pixelShader = mContentManager.Load<PixelShader>(L"Shaders\\SkyboxPS.cso");
		vertexShader->CreateInputLayout<VertexPosition>(mRenderingManager);
		SetShader(pixelShader);

		mConstantBuffer = mRenderingManager.CreateConstantBuffer(sizeof(DirectX::XMFLOAT4X4));
		assert(mConstantBuffer);
		AddShaderResource(ShaderStages::VS, *mConstantBuffer);

		assert(mTexture);
		assert(mSamplerState);
		
		AddShaderResource(ShaderStages::PS, *mTexture);
		AddShaderResource(ShaderStages::PS, *mSamplerState);
	}

	void SkyboxMaterial::BeginDraw()
	{
		Material::BeginDraw();

		static_cast<RenderingManagerD3D11&>(mRenderingManager).Context()->RSSetState(RasterizerStates::DisabledCulling.get());
	}

	void SkyboxMaterial::EndDraw()
	{
		Material::EndDraw();

		static_cast<RenderingManagerD3D11&>(mRenderingManager).Context()->RSSetState(nullptr);
	}

	void SkyboxMaterial::UpdateTransforms(DirectX::CXMMATRIX worldViewProjectionMatrix)
	{
		assert(mConstantBuffer);
		mRenderingManager.UpdateBuffer(*mConstantBuffer, &worldViewProjectionMatrix.r, sizeof(DirectX::CXMMATRIX));
	}
}