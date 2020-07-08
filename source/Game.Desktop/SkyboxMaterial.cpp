#include "pch.h"
#include "SkyboxMaterial.h"
#include "Game.h"
#include "GameException.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "VertexDeclarations.h"
#include "TextureCube.h"
#include "RasterizerStates.h"

using namespace std;
using namespace gsl;
using namespace DirectX;
using namespace winrt;

namespace Library
{
	SkyboxMaterial::SkyboxMaterial(ContentManager& contentManager, RenderingManager& renderingManager, const shared_ptr<TextureCube>& texture, const com_ptr<ID3D11SamplerState>& samplerState) :
		Material(contentManager, renderingManager),
		mTexture(texture), mSamplerState(samplerState)
	{
	}

	com_ptr<ID3D11SamplerState> SkyboxMaterial::SamplerState() const
	{
		return mSamplerState;
	}

	void SkyboxMaterial::SetSamplerState(const com_ptr<ID3D11SamplerState>& samplerState)
	{
		assert(samplerState != nullptr);
		mSamplerState = samplerState;
		Material::SetSamplerState(ShaderStages::PS, mSamplerState.get());
	}

	shared_ptr<TextureCube> SkyboxMaterial::Texture() const
	{
		return mTexture;
	}

	void SkyboxMaterial::SetTexture(shared_ptr<TextureCube> texture)
	{
		assert(texture != nullptr);
		mTexture = move(texture);
		Material::SetShaderResource(ShaderStages::PS, mTexture->ShaderResourceView().get());
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

		auto pixelShader = mContentManager.Load<PixelShader>(L"Shaders\\SkyboxPS.cso");
		SetShader(pixelShader);

		auto* direct3DDevice = static_cast<RenderingManagerD3D11&>(mRenderingManager).Device();
		vertexShader->CreateInputLayout<VertexPosition>(direct3DDevice);
		SetInputLayout(vertexShader->InputLayout());

		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
		constantBufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(static_cast<RenderingManagerD3D11&>(mRenderingManager).Device()->CreateBuffer(&constantBufferDesc, nullptr, mConstantBuffer.put()), "ID3D11Device::CreateMeshIndexBuffer() failed.");
		AddConstantBuffer(ShaderStages::VS, mConstantBuffer.get());

		AddShaderResource(ShaderStages::PS, mTexture->ShaderResourceView().get());
		AddSamplerState(ShaderStages::PS, mSamplerState.get());
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

	void SkyboxMaterial::UpdateTransforms(CXMMATRIX worldViewProjectionMatrix)
	{
		static_cast<RenderingManagerD3D11&>(mRenderingManager).Context()->UpdateSubresource(mConstantBuffer.get(), 0, nullptr, worldViewProjectionMatrix.r, 0, 0);
	}
}