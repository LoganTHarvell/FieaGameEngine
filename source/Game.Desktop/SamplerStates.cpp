#include "pch.h"
#include "SamplerStates.h"
#include "GameException.h"

using namespace DirectX;

namespace Library
{
	XMVECTORF32 SamplerStates::BorderColor{ DirectX::Colors::White };
	XMVECTORF32 SamplerStates::ShadowMapBorderColor{ DirectX::Colors::White };

	void SamplerStates::Initialize(gsl::not_null<ID3D11Device*> direct3DDevice)
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

	void SamplerStates::Shutdown()
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