#include "pch.h"
#include "BasicMaterial.h"
#include "Game.h"
#include "GameException.h"
#include "VertexDeclarations.h"
#include "VertexShader.h"
#include "PixelShader.h"

using namespace std;
using namespace gsl;
using namespace DirectX;

namespace Library
{
	BasicMaterial::BasicMaterial(ContentManager& contentManager, RenderingManager& renderingManager) :
		Material(contentManager, renderingManager)
	{
	}	

	void BasicMaterial::SetSurfaceColor(const DirectX::XMFLOAT4& color)
	{
		SetSurfaceColor(reinterpret_cast<const float*>(&color));
	}

	uint32_t BasicMaterial::VertexSize() const
	{
		return sizeof(VertexPosition);
	}

	void BasicMaterial::Initialize()
	{
		Material::Initialize();

		auto vertexShader = mContentManager.Load<VertexShader>(L"Shaders\\BasicVS.cso");
		SetShader(vertexShader);

		auto pixelShader = mContentManager.Load<PixelShader>(L"Shaders\\BasicPS.cso");
		SetShader(pixelShader);

		auto* direct3DDevice = static_cast<RenderingManagerD3D11&>(mRenderingManager).Device();
		vertexShader->CreateInputLayout<VertexPosition>(direct3DDevice);
		SetInputLayout(vertexShader->InputLayout());

		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
		constantBufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mVSConstantBuffer.put()), "ID3D11Device::CreateMeshIndexBuffer() failed.");
		AddConstantBuffer(ShaderStages::VS, mVSConstantBuffer.get());

		constantBufferDesc.ByteWidth = sizeof(XMFLOAT4);		
		ThrowIfFailed(direct3DDevice->CreateBuffer(&constantBufferDesc, nullptr, mPSConstantBuffer.put()), "ID3D11Device::CreateMeshIndexBuffer() failed.");
		AddConstantBuffer(ShaderStages::PS, mPSConstantBuffer.get());

		SetSurfaceColor(Colors::White.f);
	}

	void BasicMaterial::UpdateTransform(CXMMATRIX worldViewProjectionMatrix)
	{
		static_cast<RenderingManagerD3D11&>(mRenderingManager).Context()->UpdateSubresource(mVSConstantBuffer.get(), 0, nullptr, worldViewProjectionMatrix.r, 0, 0);
	}

	void BasicMaterial::SetSurfaceColor(const float* color)
	{
		static_cast<RenderingManagerD3D11&>(mRenderingManager).Context()->UpdateSubresource(mPSConstantBuffer.get(), 0, nullptr, color, 0, 0);
	}
}