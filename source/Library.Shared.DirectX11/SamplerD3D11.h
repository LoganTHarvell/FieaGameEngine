#pragma once

// Third Party
#include <winrt/Windows.Foundation.h>
#include <d3d11.h>

// First Party
#include "Sampler.h"

namespace Library
{
	class SamplerD3D11 final : public Sampler
	{
		RTTI_DECLARATIONS(SamplerD3D11, Sampler)
		
    public:
        explicit SamplerD3D11(const winrt::com_ptr<ID3D11SamplerState>& samplerState=winrt::com_ptr<ID3D11SamplerState>()) : mNative(samplerState) {}
		virtual ~SamplerD3D11() = default;
		SamplerD3D11(const SamplerD3D11& rhs) = default;
		SamplerD3D11& operator=(const SamplerD3D11& rhs) = default;
		SamplerD3D11(SamplerD3D11&& rhs) = default;
		SamplerD3D11& operator=(SamplerD3D11&& rhs) = default;

	public:
        ID3D11SamplerState* Native() const { return mNative.get(); }

    private:
        winrt::com_ptr<ID3D11SamplerState> mNative;
	};
}
