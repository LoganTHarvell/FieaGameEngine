#include "pch.h"

#ifdef RENDERINGAPI_DIRECTX11
#include "RenderingAPI_DirectX11.h"
#include "Mesh.h"

namespace Library
{
	RenderingAPI_DirectX11::RenderingAPI_DirectX11(ID3D11Device3& device, ID3D11DeviceContext3& context) :
		mDevice({ &device, &context })
	{
	}

	RenderingManager::Device& RenderingAPI_DirectX11::GetDevice()
	{
		return mDevice;
	}

	const RenderingManager::Device& RenderingAPI_DirectX11::GetDevice() const
	{
		return mDevice;
	}

	void RenderingAPI_DirectX11::SetDevice(const Device device)
	{
		mDevice = device;
	}

	RenderingManager::IndexBuffer RenderingAPI_DirectX11::CreateMeshIndexBuffer(const Mesh& mesh) const
	{
		if (!mDevice.DevicePtr) return IndexBuffer{ nullptr };

		ID3D11Buffer* indexBuffer = nullptr;
		
		D3D11_BUFFER_DESC indexBufferDesc{ 0 };
		indexBufferDesc.ByteWidth = gsl::narrow_cast<uint32_t>(sizeof(uint32_t) * mesh.Indices().Size());
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexSubResourceData{ 0 };
		indexSubResourceData.pSysMem = &mesh.Indices()[0];

		if (FAILED(mDevice.DevicePtr->CreateBuffer(&indexBufferDesc, &indexSubResourceData, &indexBuffer)))
		{
			throw std::runtime_error("ID3D11Device::CreateBuffer() failed.");
		}

		return IndexBuffer{ indexBuffer };
	}
}
#endif
