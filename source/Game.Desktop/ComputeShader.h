#pragma once

#include "Shader.h"

namespace Library
{
	class ComputeShader final : public Shader
	{
		RTTI_DECLARATIONS(ComputeShader, Shader)

	public:
		ComputeShader(const winrt::com_ptr<ID3D11ComputeShader>& computeShader=winrt::com_ptr<ID3D11ComputeShader>());
		ComputeShader(const ComputeShader&) = default;
		ComputeShader& operator=(const ComputeShader&) = default;
		ComputeShader(ComputeShader&&) = default;
		ComputeShader& operator=(ComputeShader&&) = default;
		~ComputeShader() = default;

		winrt::com_ptr<ID3D11ComputeShader> Shader() const;

	private:
		winrt::com_ptr<ID3D11ComputeShader> mShader;
	};
}
