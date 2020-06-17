#pragma once

#include "Shader.h"

namespace Library
{
	class HullShader final : public Shader
	{
		RTTI_DECLARATIONS(HullShader, Shader)

	public:
		HullShader(const winrt::com_ptr<ID3D11HullShader>& hullShader=winrt::com_ptr<ID3D11HullShader>());
		HullShader(const HullShader&) = default;
		HullShader& operator=(const HullShader&) = default;
		HullShader(HullShader&&) = default;
		HullShader& operator=(HullShader&&) = default;
		~HullShader() = default;

		winrt::com_ptr<ID3D11HullShader> Shader() const;

	private:
		winrt::com_ptr<ID3D11HullShader> mShader;
	};
}