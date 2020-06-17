#pragma once

#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include <gsl\gsl>

namespace Library
{
	class DepthStencilStates final
	{
	public:
		inline static winrt::com_ptr<ID3D11DepthStencilState> DefaultDepthCulling;
		inline static winrt::com_ptr<ID3D11DepthStencilState> NoDepthCulling;

		static void Initialize(gsl::not_null<ID3D11Device*> direct3DDevice);
		static void Shutdown();

		DepthStencilStates() = delete;
		DepthStencilStates(const DepthStencilStates&) = delete;
		DepthStencilStates& operator=(const DepthStencilStates&) = delete;
		DepthStencilStates(DepthStencilStates&&) = delete;
		DepthStencilStates& operator=(DepthStencilStates&&) = delete;
		~DepthStencilStates() = default;
	};
}
