#pragma once

#include "Shader.h"

namespace Library
{
	class PixelShader final : public Shader
	{
		RTTI_DECLARATIONS(PixelShader, Shader)

	public:
		PixelShader(const winrt::com_ptr<ID3D11PixelShader>& pixelShader=winrt::com_ptr<ID3D11PixelShader>());
		PixelShader(const PixelShader&) = default;
		PixelShader& operator=(const PixelShader&) = default;
		PixelShader(PixelShader&&) = default;
		PixelShader& operator=(PixelShader&&) = default;
		~PixelShader() = default;

		winrt::com_ptr<ID3D11PixelShader> Shader() const;

	private:
		winrt::com_ptr<ID3D11PixelShader> mShader;
	};
}
