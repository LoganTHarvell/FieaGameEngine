#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "CoreD3D.h"
#pragma endregion Includes

namespace Library
{
	// TODO: Move these functions to RenderingManagerD3D11 as CreateBuffer
	void CreateIndexBuffer(gsl::not_null<ID3D11Device*> device, const gsl::span<const uint16_t>& indices, gsl::not_null<ID3D11Buffer**> indexBuffer)
	{
		D3D11_BUFFER_DESC indexBufferDesc{ 0 };
		indexBufferDesc.ByteWidth = gsl::narrow<uint32_t>(indices.size_bytes());
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexSubResourceData{ 0 };
		indexSubResourceData.pSysMem = &indices[0];
		ThrowIfFailed(device->CreateBuffer(&indexBufferDesc, &indexSubResourceData, indexBuffer), "ID3D11Device::CreateBuffer() failed.");

	}

	void CreateIndexBuffer(gsl::not_null<ID3D11Device*> device, const gsl::span<const uint32_t>& indices, gsl::not_null<ID3D11Buffer**> indexBuffer)
	{
		D3D11_BUFFER_DESC indexBufferDesc{ 0 };
		indexBufferDesc.ByteWidth = gsl::narrow<uint32_t>(indices.size_bytes());
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexSubResourceData{ 0 };
		indexSubResourceData.pSysMem = &indices[0];
		ThrowIfFailed(device->CreateBuffer(&indexBufferDesc, &indexSubResourceData, indexBuffer), "ID3D11Device::CreateBuffer() failed.");
	}

	void CreateConstantBuffer(gsl::not_null<ID3D11Device*> device, std::size_t byteWidth, gsl::not_null<ID3D11Buffer**> constantBuffer)
	{
		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
		constantBufferDesc.ByteWidth = gsl::narrow_cast<uint32_t>(byteWidth);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(device->CreateBuffer(&constantBufferDesc, nullptr, constantBuffer), "ID3D11Device::CreateBuffer() failed.");
	}
}