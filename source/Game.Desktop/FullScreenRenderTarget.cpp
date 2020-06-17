#include "pch.h"
#include "FullScreenRenderTarget.h"
#include "Game.h"

using namespace winrt;
using namespace gsl;

namespace Library
{
	FullScreenRenderTarget::FullScreenRenderTarget(Game& game) :
		mGame(&game)
	{
		SIZE renderTargetSize = game.RenderTargetSize();

		D3D11_TEXTURE2D_DESC fullScreenTextureDesc{ 0 };
		fullScreenTextureDesc.Width = renderTargetSize.cx;
		fullScreenTextureDesc.Height = renderTargetSize.cy;
        fullScreenTextureDesc.MipLevels = 1;
        fullScreenTextureDesc.ArraySize = 1;
        fullScreenTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        fullScreenTextureDesc.SampleDesc.Count = 1;
        fullScreenTextureDesc.SampleDesc.Quality = 0;
        fullScreenTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        fullScreenTextureDesc.Usage = D3D11_USAGE_DEFAULT;

		auto direct3DDevice = game.Direct3DDevice();
        com_ptr<ID3D11Texture2D> fullScreenTexture;
		ThrowIfFailed(direct3DDevice->CreateTexture2D(&fullScreenTextureDesc, nullptr, fullScreenTexture.put()), "ID3D11Device::CreateTexture2D() failed.");
		ThrowIfFailed(direct3DDevice->CreateShaderResourceView(fullScreenTexture.get(), nullptr, mOutputTexture.put()), "ID3D11Device::CreateShaderResourceView() failed.");
		ThrowIfFailed(direct3DDevice->CreateRenderTargetView(fullScreenTexture.get(), nullptr, mRenderTargetView.put()), "ID3D11Device::CreateRenderTargetView() failed.");

        D3D11_TEXTURE2D_DESC depthStencilDesc;
        ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
		depthStencilDesc.Width = renderTargetSize.cx;
		depthStencilDesc.Height = renderTargetSize.cy;
        depthStencilDesc.MipLevels = 1;
        depthStencilDesc.ArraySize = 1;
        depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
        depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;

        com_ptr<ID3D11Texture2D> depthStencilBuffer;
		ThrowIfFailed(game.Direct3DDevice()->CreateTexture2D(&depthStencilDesc, nullptr, depthStencilBuffer.put()), "ID3D11Device::CreateTexture2D() failed.");
		ThrowIfFailed(game.Direct3DDevice()->CreateDepthStencilView(depthStencilBuffer.get(), nullptr, mDepthStencilView.put()), "ID3D11Device::CreateDepthStencilView() failed.");
    }

	com_ptr<ID3D11ShaderResourceView> FullScreenRenderTarget::OutputTexture() const
    {
        return mOutputTexture;
    }

	com_ptr<ID3D11RenderTargetView> FullScreenRenderTarget::RenderTargetView() const
    {
		return mRenderTargetView;
    }

    com_ptr<ID3D11DepthStencilView> FullScreenRenderTarget::DepthStencilView() const
    {
		return mDepthStencilView;
    }

    void FullScreenRenderTarget::Begin()
    {
		ID3D11RenderTargetView* views[] = { mRenderTargetView.get() };
		const span<ID3D11RenderTargetView*> renderTargetViews{ views };
		RenderTarget::Begin(mGame->Direct3DDeviceContext(), views, not_null<ID3D11DepthStencilView*>(mDepthStencilView.get()), mGame->Viewport());
    }

    void FullScreenRenderTarget::End()
    {
		RenderTarget::End(mGame->Direct3DDeviceContext());
    }
}