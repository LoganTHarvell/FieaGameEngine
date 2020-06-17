#pragma once

#include <winrt\Windows.Foundation.h>
#include <cstdint>
#include <d3d11.h>
#include <DirectXMath.h>
#include <dxgidebug.h>
#include <gsl\gsl>
#include "GameException.h"

namespace Library
{
	enum class ShaderStages
	{
		IA,
		VS,
		HS,
		DS,
		GS,
		SO,
		RS,
		PS,
		OM,

		CS
	};

	const std::array<ShaderStages, 6> ProgrammableGraphicsShaderStages
	{
		ShaderStages::VS,
		ShaderStages::HS,
		ShaderStages::DS,
		ShaderStages::GS,
		ShaderStages::PS,
		ShaderStages::CS,
	};

	inline bool ShaderStageIsProgrammable(ShaderStages shaderStage)
	{
		static const std::map<ShaderStages, bool> isProgrammableMap
		{
			{ShaderStages::IA, false },
			{ ShaderStages::VS, true },
			{ ShaderStages::HS, true },
			{ ShaderStages::DS, true },
			{ ShaderStages::GS, true },
			{ ShaderStages::SO, false },
			{ ShaderStages::RS, false },
			{ ShaderStages::PS, true },
			{ ShaderStages::OM, false },
			{ ShaderStages::CS, true },
		};

		return isProgrammableMap.at(shaderStage);
	}

	void CreateIndexBuffer(gsl::not_null<ID3D11Device*> device, const gsl::span<const std::uint16_t>& indices, gsl::not_null<ID3D11Buffer**> indexBuffer);
	void CreateIndexBuffer(gsl::not_null<ID3D11Device*> device, const gsl::span<const std::uint32_t>& indices, gsl::not_null<ID3D11Buffer**> indexBuffer);
	void CreateConstantBuffer(gsl::not_null<ID3D11Device*> device, std::size_t byteWidth, gsl::not_null<ID3D11Buffer**> constantBuffer);

	inline float ConvertDipsToPixels(float dips, float dpi)
	{
		static const float dipsPerInch = 96.0f;
		return floorf(dips * dpi / dipsPerInch + 0.5f); // Round to nearest integer.
	}

#if defined(DEBUG) || defined(_DEBUG)
	// Check for SDK Layer support.
	inline bool SdkLayersAvailable()
	{
		HRESULT hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
			0,
			D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
			nullptr,                    // Any feature level will do.
			0,
			D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
			nullptr,                    // No need to keep the D3D device reference.
			nullptr,                    // No need to know the feature level.
			nullptr                     // No need to keep the D3D device context reference.
			);

		return SUCCEEDED(hr);
	}
#endif

#if defined(DEBUG) || defined(_DEBUG)
	inline void DumpD3DDebug()
	{
		winrt::com_ptr<IDXGIDebug1> debugInterface = nullptr;
		ThrowIfFailed(DXGIGetDebugInterface1(0, IID_PPV_ARGS(debugInterface.put())));
		ThrowIfFailed(debugInterface->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL));
	}
#endif
}