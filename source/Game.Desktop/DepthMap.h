#pragma once

#include "RenderTarget.h"
#include <winrt\Windows.Foundation.h>

namespace Library
{
    class Game;

    class DepthMap final : public RenderTarget
    {
		RTTI_DECLARATIONS_ABSTRACT(DepthMap, RenderTarget)

    public:
        DepthMap(Game& game, std::uint32_t width, std::uint32_t height);
		DepthMap(const DepthMap&) = delete;
		DepthMap(DepthMap&&) = default;
		DepthMap& operator=(const DepthMap&) = delete;
		DepthMap& operator=(DepthMap&&) = default;
		~DepthMap() = default;

		std::uint32_t Width() const;
		std::uint32_t Height() const;
		const D3D11_VIEWPORT& Viewport() const;

		winrt::com_ptr<ID3D11ShaderResourceView> OutputTexture() const;
		winrt::com_ptr<ID3D11DepthStencilView> DepthStencilView() const;
	
        virtual void Begin() override;
		virtual void End() override;

    private:
        Game* mGame;
		winrt::com_ptr<ID3D11DepthStencilView> mDepthStencilView;
		winrt::com_ptr<ID3D11ShaderResourceView> mOutputTexture;
		D3D11_VIEWPORT mViewport;
    };
}