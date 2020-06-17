#pragma once

#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <gsl\gsl>

namespace Library
{
	class SamplerStates final
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

		static DirectX::XMVECTORF32 BorderColor;
		static DirectX::XMVECTORF32 ShadowMapBorderColor;

		static void Initialize(gsl::not_null<ID3D11Device*> direct3DDevice);
		static void Shutdown();

		SamplerStates() = delete;
		SamplerStates(const SamplerStates&) = delete;
		SamplerStates& operator=(const SamplerStates&) = delete;
		SamplerStates(SamplerStates&&) = delete;
		SamplerStates& operator=(SamplerStates&&) = delete;
		~SamplerStates() = default;
	};
}