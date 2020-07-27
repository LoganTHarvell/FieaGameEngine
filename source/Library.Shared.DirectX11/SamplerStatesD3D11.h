#pragma once

#include <winrt/Windows.Foundation.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <gsl/gsl>

#include "SamplerD3D11.h"

namespace Library
{
	class SamplerStatesD3D11 final
	{
	public:
		inline static winrt::com_ptr<ID3D11SamplerState> TrilinearWrap;
		inline static winrt::com_ptr<ID3D11SamplerState> TrilinearMirror;
		inline static winrt::com_ptr<ID3D11SamplerState> TrilinearClamp;
		inline static winrt::com_ptr<ID3D11SamplerState> TrilinearBorder;
		inline static winrt::com_ptr<ID3D11SamplerState> PointClamp;
		inline static winrt::com_ptr<ID3D11SamplerState> DepthMap;
		inline static winrt::com_ptr<ID3D11SamplerState> ShadowMap;
		inline static winrt::com_ptr<ID3D11SamplerState> PcfShadowMap;

		static HashMap<Sampler::Type, std::shared_ptr<SamplerD3D11>> TypeSamplerMap;

		static DirectX::XMVECTORF32 BorderColor;
		static DirectX::XMVECTORF32 ShadowMapBorderColor;

		static void Initialize(gsl::not_null<ID3D11Device*> direct3DDevice);
		static void Shutdown();

		SamplerStatesD3D11() = delete;
		SamplerStatesD3D11(const SamplerStatesD3D11&) = delete;
		SamplerStatesD3D11& operator=(const SamplerStatesD3D11&) = delete;
		SamplerStatesD3D11(SamplerStatesD3D11&&) = delete;
		SamplerStatesD3D11& operator=(SamplerStatesD3D11&&) = delete;
		~SamplerStatesD3D11() = default;
	};
}
