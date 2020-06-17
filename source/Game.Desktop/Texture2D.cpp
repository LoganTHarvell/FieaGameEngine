#include "pch.h"
#include "Texture2D.h"
#include "GameException.h"

using namespace std;
using namespace gsl;
using namespace DirectX;
using namespace winrt;

namespace Library
{
	Texture2D::Texture2D(const com_ptr<ID3D11ShaderResourceView>& shaderResourceView, uint32_t width, uint32_t height) :
		Texture(shaderResourceView), mWidth(width), mHeight(height)
	{
	}

	shared_ptr<Texture2D> Texture2D::CreateTexture2D(not_null<ID3D11Device*> device, const D3D11_TEXTURE2D_DESC& textureDesc)
	{
		HRESULT hr;
		com_ptr<ID3D11Texture2D> texture;
		if (FAILED(hr = device->CreateTexture2D(&textureDesc, nullptr, texture.put())))
		{
			throw GameException("IDXGIDevice::CreateTexture2D() failed.", hr);
		}

		com_ptr<ID3D11ShaderResourceView> shaderResourceReview;
		if (FAILED(hr = device->CreateShaderResourceView(texture.get(), nullptr, shaderResourceReview.put())))
		{
			throw GameException("IDXGIDevice::CreateShaderResourceView() failed.", hr);
		}

		return make_shared<Texture2D>(shaderResourceReview, textureDesc.Width, textureDesc.Height);
	}

	shared_ptr<Texture2D> Texture2D::CreateTexture2D(not_null<ID3D11Device*> device, uint32_t width, uint32_t height, uint32_t mipLevels, uint32_t arraySize, DXGI_FORMAT format, DXGI_SAMPLE_DESC sampleDesc, uint32_t bindFlags, uint32_t cpuAccessFlags)
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

	uint32_t Texture2D::Width() const
	{
		return mWidth;
	}

	uint32_t Texture2D::Height() const
	{
		return mHeight;
	}

	Rectangle Texture2D::Bounds() const
	{
		return Rectangle(0, 0, narrow<int32_t>(mWidth), narrow<int32_t>(mHeight));
	}	
}