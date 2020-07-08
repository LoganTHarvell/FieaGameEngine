#include "pch.h"
#include "TexturedModelMaterial.h"
#include "Game.h"
#include "GameException.h"
#include "VertexDeclarations.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture2D.h"

using namespace std;
using namespace gsl;
using namespace winrt;
using namespace DirectX;

namespace Library
{
	TexturedModelMaterial::TexturedModelMaterial(ContentManager& contentManager, RenderingManager& renderingManager, shared_ptr<Texture2D> texture) :
		Material(contentManager, renderingManager), mTexture(move(texture))
	{
		if (mTexture != nullptr)
		{
			AddShaderResource(ShaderStages::PS, mTexture->ShaderResourceView().get());
		}
	}

	com_ptr<ID3D11SamplerState> TexturedModelMaterial::SamplerState() const
	{
		return mSamplerState;
	}

	void TexturedModelMaterial::SetSamplerState(com_ptr<ID3D11SamplerState> samplerState)
	{
		assert(samplerState != nullptr);
		mSamplerState = move(samplerState);
		Material::SetSamplerState(ShaderStages::PS, mSamplerState.get());
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
			AddShaderResource(ShaderStages::PS, mTexture->ShaderResourceView().get());
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

		auto pixelShader = mContentManager.Load<PixelShader>(L"Shaders\\TexturedModelPS.cso");
		SetShader(pixelShader);

		auto* direct3DDevice = static_cast<RenderingManagerD3D11&>(mRenderingManager).Device();
		vertexShader->CreateInputLayout<VertexPositionTexture>(direct3DDevice);
		SetInputLayout(vertexShader->InputLayout());

		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
		constantBufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mVertexCBufferPerObject.put()), "ID3D11Device::CreateMeshIndexBuffer() failed.");
		AddConstantBuffer(ShaderStages::VS, mVertexCBufferPerObject.get());

		AddSamplerState(ShaderStages::PS, mSamplerState.get());
	}

	void TexturedModelMaterial::UpdateTransform(CXMMATRIX worldViewProjectionMatrix)
	{
		static_cast<RenderingManagerD3D11&>(mRenderingManager).Context()->UpdateSubresource(mVertexCBufferPerObject.get(), 0, nullptr, worldViewProjectionMatrix.r, 0, 0);
	}
}