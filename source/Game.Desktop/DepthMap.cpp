#include "pch.h"
#include "DepthMap.h"
#include "Game.h"

using namespace std;
using namespace gsl;
using namespace winrt;

namespace Library
{
    DepthMap::DepthMap(Game& game, uint32_t width, uint32_t height) :
		mGame(&game)
    {
		D3D11_TEXTURE2D_DESC textureDesc{ 0 };
        textureDesc.Width = width;
        textureDesc.Height = height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL| D3D11_BIND_SHADER_RESOURCE;

		auto direct3DDevice = game.Direct3DDevice();
        com_ptr<ID3D11Texture2D> texture;
		ThrowIfFailed(direct3DDevice->CreateTexture2D(&textureDesc, nullptr, texture.put()), "IDXGIDevice::CreateTexture2D() failed.");

        D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
        ZeroMemory(&resourceViewDesc, sizeof(resourceViewDesc));
        resourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
        resourceViewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
        resourceViewDesc.Texture2D.MipLevels = 1;

		ThrowIfFailed(direct3DDevice->CreateShaderResourceView(texture.get(), &resourceViewDesc, mOutputTexture.put()), "IDXGIDevice::CreateShaderResourceView() failed.");

        D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
        ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
        depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        depthStencilViewDesc.Texture2D.MipSlice = 0;    

		ThrowIfFailed(direct3DDevice->CreateDepthStencilView(texture.get(), &depthStencilViewDesc, mDepthStencilView.put()), "IDXGIDevice::CreateDepthStencilView() failed.");

        mViewport.TopLeftX = 0.0f;
        mViewport.TopLeftY = 0.0f;
        mViewport.Width = static_cast<float>(width);
        mViewport.Height = static_cast<float>(height);
        mViewport.MinDepth = 0.0f;
        mViewport.MaxDepth = 1.0f;
    }

	uint32_t DepthMap::Width() const
	{
		return static_cast<uint32_t>(mViewport.Width);
	}

	uint32_t DepthMap::Height() const
	{
		return static_cast<uint32_t>(mViewport.Height);
	}

	const D3D11_VIEWPORT& DepthMap::Viewport() const
	{
		return mViewport;
	}

	com_ptr<ID3D11ShaderResourceView> DepthMap::OutputTexture() const
	{
		return mOutputTexture;
	}

	com_ptr<ID3D11DepthStencilView> DepthMap::DepthStencilView() const
	{
		return mDepthStencilView;
	}

    void DepthMap::Begin()
    {
		static std::array<ID3D11RenderTargetView*, 1> emptyRenderTargetViews{ nullptr };
		RenderTarget::Begin(mGame->Direct3DDeviceContext(), emptyRenderTargetViews, not_null<ID3D11DepthStencilView*>(mDepthStencilView.get()), mViewport);
    }

    void DepthMap::End()
    {
        RenderTarget::End(mGame->Direct3DDeviceContext());
    }
}