#pragma once

#include "RTTI.h"
#include <d3d11.h>
#include <stack>
#include <gsl\gsl>
#include <vector>

namespace Library
{
	class RenderTarget : public RTTI
	{
		RTTI_DECLARATIONS_ABSTRACT(RenderTarget, RTTI)

	public:
		RenderTarget() = default;
		RenderTarget(const RenderTarget&) = delete;
		RenderTarget(RenderTarget&&) = default;
		RenderTarget& operator=(const RenderTarget&) = delete;		
		RenderTarget& operator=(RenderTarget&&) = default;
		virtual ~RenderTarget() = default;

		virtual void Begin() = 0;
		virtual void End() = 0;

	protected:
		struct RenderTargetData
		{
			uint32_t ViewCount() const { return gsl::narrow_cast<uint32_t>(RenderTargetViews.size()); }

			std::vector<ID3D11RenderTargetView*> RenderTargetViews;
			gsl::not_null<ID3D11DepthStencilView*> DepthStencilView;
			D3D11_VIEWPORT Viewport;

			RenderTargetData(const gsl::span<ID3D11RenderTargetView*>& renderTargetViews, gsl::not_null<ID3D11DepthStencilView*> depthStencilView, const D3D11_VIEWPORT& viewport) :
				RenderTargetViews(renderTargetViews.begin(), renderTargetViews.end()), DepthStencilView(depthStencilView), Viewport(viewport) { }
		};

		void Begin(gsl::not_null<ID3D11DeviceContext*> deviceContext, const gsl::span<ID3D11RenderTargetView*>& renderTargetViews, gsl::not_null<ID3D11DepthStencilView*> depthStencilView, const D3D11_VIEWPORT& viewport);
		void End(gsl::not_null<ID3D11DeviceContext*> deviceContext);
		void RebindCurrentRenderTargets(gsl::not_null<ID3D11DeviceContext*> deviceContext);

	private:
		static std::stack<RenderTargetData> sRenderTargetStack;
	};
}