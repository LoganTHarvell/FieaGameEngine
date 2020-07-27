// Pre-compiled Header
#include "pch.h"

// Header
#include "SamplerStatesD3D11.h"

// Third Party
#include <DirectXColors.h>

// First Party
#include "GameException.h"

namespace Library
{
	const HashMap<Sampler::Type, std::shared_ptr<SamplerD3D11>> SamplerStatesD3D11::TypeSamplerMap
	{
		{ Sampler::Type::TrilinearWrap,		std::make_shared<SamplerD3D11>(TrilinearWrap)	},
		{ Sampler::Type::TrilinearMirror,	std::make_shared<SamplerD3D11>(TrilinearMirror) },
		{ Sampler::Type::TrilinearClamp,	std::make_shared<SamplerD3D11>(TrilinearClamp)	},
		{ Sampler::Type::TrilinearBorder,	std::make_shared<SamplerD3D11>(TrilinearBorder)	},
		{ Sampler::Type::PointClamp,		std::make_shared<SamplerD3D11>(PointClamp)		},
		{ Sampler::Type::DepthMap,			std::make_shared<SamplerD3D11>(DepthMap)		},
		{ Sampler::Type::ShadowMap,			std::make_shared<SamplerD3D11>(ShadowMap)		},
		{ Sampler::Type::PcfShadowMap,		std::make_shared<SamplerD3D11>(PcfShadowMap)	}
	};

	DirectX::XMVECTORF32 SamplerStatesD3D11::BorderColor{ DirectX::Colors::White };
	DirectX::XMVECTORF32 SamplerStatesD3D11::ShadowMapBorderColor{ DirectX::Colors::White };

	void SamplerStatesD3D11::Initialize(gsl::not_null<ID3D11Device*> direct3DDevice)
	{
		D3D11_SAMPLER_DESC samplerStateDesc;
		ZeroMemory(&samplerStateDesc, sizeof(samplerStateDesc));
		samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerStateDesc.MinLOD = -FLT_MAX;
		samplerStateDesc.MaxLOD = FLT_MAX;
		samplerStateDesc.MipLODBias = 0.0f;
		samplerStateDesc.MaxAnisotropy = 1;
		samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		ThrowIfFailed(direct3DDevice->CreateSamplerState(&samplerStateDesc, TrilinearWrap.put()), "ID3D11Device::CreateSamplerState() failed.");

		samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
		ThrowIfFailed(direct3DDevice->CreateSamplerState(&samplerStateDesc, TrilinearMirror.put()), "ID3D11Device::CreateSamplerState() failed.");

		samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		ThrowIfFailed(direct3DDevice->CreateSamplerState(&samplerStateDesc, TrilinearClamp.put()), "ID3D11Device::CreateSamplerState() failed.");

		samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		memcpy(samplerStateDesc.BorderColor, BorderColor.f, sizeof(FLOAT) * 4);
		ThrowIfFailed(direct3DDevice->CreateSamplerState(&samplerStateDesc, TrilinearBorder.put()), "ID3D11Device::CreateSamplerState() failed.");

		samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		ThrowIfFailed(direct3DDevice->CreateSamplerState(&samplerStateDesc, PointClamp.put()), "ID3D11Device::CreateSamplerState() failed.");

		samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		memcpy(samplerStateDesc.BorderColor, BorderColor.f, sizeof(FLOAT) * 4);
		ThrowIfFailed(direct3DDevice->CreateSamplerState(&samplerStateDesc, DepthMap.put()), "ID3D11Device::CreateSamplerState() failed.");

		samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		memcpy(samplerStateDesc.BorderColor, ShadowMapBorderColor.f, sizeof(FLOAT) * 4);
		ThrowIfFailed(direct3DDevice->CreateSamplerState(&samplerStateDesc, ShadowMap.put()), "ID3D11Device::CreateSamplerState() failed.");

		samplerStateDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		memcpy(samplerStateDesc.BorderColor, ShadowMapBorderColor.f, sizeof(FLOAT) * 4);
		ThrowIfFailed(direct3DDevice->CreateSamplerState(&samplerStateDesc, PcfShadowMap.put()), "ID3D11Device::CreateSamplerState() failed.");
	}

	void SamplerStatesD3D11::Shutdown()
	{
		TrilinearWrap = nullptr;
		TrilinearMirror = nullptr;
		TrilinearClamp = nullptr;
		TrilinearBorder = nullptr;
		PointClamp = nullptr;
		DepthMap = nullptr;
		ShadowMap = nullptr;
		PcfShadowMap = nullptr;
	}
}