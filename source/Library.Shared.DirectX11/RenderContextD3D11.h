#pragma once

#pragma region Includes
// Third Party
#include <d3d11.h>
#include <winrt/base.h>

// First Party
#include "RenderContext.h"
#include "Factory.h"
#pragma endregion Includes

namespace Library
{
	class RenderContextD3D11 final : public RenderContext
	{
		RTTI_DECLARATIONS_ABSTRACT(RenderContextD3D11, RenderContext)

	public:
		RenderContextD3D11() = delete;
		~RenderContextD3D11() = default;
		RenderContextD3D11(const RenderContextD3D11& rhs) = default;
		RenderContextD3D11& operator=(const RenderContextD3D11& rhs) = default;
		RenderContextD3D11(RenderContextD3D11&& rhs) = default;
		RenderContextD3D11& operator=(RenderContextD3D11&& rhs) = default;

		RenderContextD3D11(const winrt::com_ptr<ID3D11Device>& device, const winrt::com_ptr<ID3D11DeviceContext>& context);
		
	private:
		winrt::com_ptr<ID3D11Device> mDevice;
		winrt::com_ptr<ID3D11DeviceContext> mContext;
	};
}
