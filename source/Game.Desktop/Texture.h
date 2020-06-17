#pragma once

#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include "RTTI.h"

namespace Library
{
	class Texture : public RTTI
	{
		RTTI_DECLARATIONS(Texture, RTTI)

	public:
		Texture() = default;
		Texture(const Texture&) = default;
		Texture& operator=(const Texture&) = default;
		Texture(Texture&&) = default;
		Texture& operator=(Texture&&) = default;
		virtual ~Texture() = default;

		winrt::com_ptr<ID3D11ShaderResourceView> ShaderResourceView() const;

	protected:
		Texture(const winrt::com_ptr<ID3D11ShaderResourceView>& shaderResourceView);

		winrt::com_ptr<ID3D11ShaderResourceView> mShaderResourceView;
	};
}

#include "Texture.inl"