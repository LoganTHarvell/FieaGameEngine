#pragma once

#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include <gsl\gsl>

namespace Library
{
	class RasterizerStates final
	{
	public:
		inline static winrt::com_ptr<ID3D11RasterizerState> BackCulling;
		inline static winrt::com_ptr<ID3D11RasterizerState> FrontCulling;
		inline static winrt::com_ptr<ID3D11RasterizerState> DisabledCulling;
		inline static winrt::com_ptr<ID3D11RasterizerState> Wireframe;

		static void Initialize(gsl::not_null<ID3D11Device*> direct3DDevice);
		static void Shutdown();

		RasterizerStates() = delete;
		RasterizerStates(const RasterizerStates&) = delete;
		RasterizerStates& operator=(const RasterizerStates&) = delete;
		RasterizerStates(RasterizerStates&&) = delete;
		RasterizerStates& operator=(RasterizerStates&&) = delete;
		~RasterizerStates() = default;
	};
}
