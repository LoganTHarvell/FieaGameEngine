#pragma once

#include "RenderTarget.h"
#include <winrt\Windows.Foundation.h>

namespace Library
{
    class Game;

	class FullScreenRenderTarget final : public RenderTarget
    {
    public:
        FullScreenRenderTarget(Game& game);
		FullScreenRenderTarget(const FullScreenRenderTarget&) = delete;
		FullScreenRenderTarget(FullScreenRenderTarget&&) = default;
		FullScreenRenderTarget& operator=(const FullScreenRenderTarget&) = delete;
		FullScreenRenderTarget& operator=(FullScreenRenderTarget&&) = default;
		~FullScreenRenderTarget() = default;

		winrt::com_ptr<ID3D11ShaderResourceView> OutputTexture() const;
		winrt::com_ptr<ID3D11RenderTargetView> RenderTargetView() const;
        winrt::com_ptr<ID3D11DepthStencilView> DepthStencilView() const;

        void Begin() override;
        void End() override;

    private:
        Game* mGame;
        winrt::com_ptr<ID3D11RenderTargetView> mRenderTargetView;
		winrt::com_ptr<ID3D11DepthStencilView> mDepthStencilView;
		winrt::com_ptr<ID3D11ShaderResourceView> mOutputTexture;
    };
}