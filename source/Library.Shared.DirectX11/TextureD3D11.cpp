#include "pch.h"
#include "TextureD3D11.h"
#include "GameException.h"

namespace Library
{
	std::shared_ptr<Texture2D> Texture2DD3D11::CreateTexture2D(gsl::not_null<ID3D11Device*> device, const D3D11_TEXTURE2D_DESC& textureDesc)
	{
		HRESULT hr;
		winrt::com_ptr<ID3D11Texture2D> texture;
		if (FAILED(hr = device->CreateTexture2D(&textureDesc, nullptr, texture.put())))
		{
			throw GameException("IDXGIDevice::CreateTexture2D() failed.", hr);
		}

		winrt::com_ptr<ID3D11ShaderResourceView> shaderResourceReview;
		if (FAILED(hr = device->CreateShaderResourceView(texture.get(), nullptr, shaderResourceReview.put())))
		{
			throw GameException("IDXGIDevice::CreateShaderResourceView() failed.", hr);
		}

		return std::make_shared<Texture2DD3D11>(shaderResourceReview, textureDesc.Width, textureDesc.Height);
	}

	std::shared_ptr<Texture2D> Texture2DD3D11::CreateTexture2D(gsl::not_null<ID3D11Device*> device, uint32_t width, uint32_t height, uint32_t mipLevels, uint32_t arraySize, DXGI_FORMAT format, DXGI_SAMPLE_DESC sampleDesc, uint32_t bindFlags, uint32_t cpuAccessFlags)
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = mipLevels;
		textureDesc.ArraySize = arraySize;
		textureDesc.Format = format;
		textureDesc.SampleDesc = sampleDesc;
		textureDesc.BindFlags = bindFlags;
		textureDesc.CPUAccessFlags = cpuAccessFlags;

		return CreateTexture2D(device, textureDesc);		
	}
}