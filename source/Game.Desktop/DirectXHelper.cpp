#include "pch.h"
#include "DirectXHelper.h"
#include "GameException.h"

using namespace std;
using namespace gsl;

namespace Library
{
	void CreateIndexBuffer(not_null<ID3D11Device*> device, const span<const uint16_t>& indices, not_null<ID3D11Buffer**> indexBuffer)
	{
		D3D11_BUFFER_DESC indexBufferDesc{ 0 };
		indexBufferDesc.ByteWidth = narrow<uint32_t>(indices.size_bytes());
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexSubResourceData{ 0 };
		indexSubResourceData.pSysMem = &indices[0];
		ThrowIfFailed(device->CreateBuffer(&indexBufferDesc, &indexSubResourceData, indexBuffer), "ID3D11Device::CreateBuffer() failed.");

	}

	void CreateIndexBuffer(not_null<ID3D11Device*> device, const span<const uint32_t>& indices, not_null<ID3D11Buffer**> indexBuffer)
	{
		D3D11_BUFFER_DESC indexBufferDesc{ 0 };
		indexBufferDesc.ByteWidth = narrow<uint32_t>(indices.size_bytes());
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexSubResourceData{ 0 };
		indexSubResourceData.pSysMem = &indices[0];
		ThrowIfFailed(device->CreateBuffer(&indexBufferDesc, &indexSubResourceData, indexBuffer), "ID3D11Device::CreateBuffer() failed.");
	}

	void CreateConstantBuffer(not_null<ID3D11Device*> device, std::size_t byteWidth, not_null<ID3D11Buffer**> constantBuffer)
	{
		D3D11_BUFFER_DESC constantBufferDesc{ 0 };
		constantBufferDesc.ByteWidth = narrow_cast<uint32_t>(byteWidth);
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ThrowIfFailed(device->CreateBuffer(&constantBufferDesc, nullptr, constantBuffer), "ID3D11Device::CreateBuffer() failed.");
	}
}