#pragma once

#include "Shader.h"

namespace Library
{
	class DomainShader final : public Shader
	{
		RTTI_DECLARATIONS(DomainShader, Shader)

	public:
		DomainShader(const winrt::com_ptr<ID3D11DomainShader>& domainShader=winrt::com_ptr<ID3D11DomainShader>());
		DomainShader(const DomainShader&) = default;
		DomainShader& operator=(const DomainShader&) = default;
		DomainShader(DomainShader&&) = default;
		DomainShader& operator=(DomainShader&&) = default;
		~DomainShader() = default;

		winrt::com_ptr<ID3D11DomainShader> Shader() const;

	private:
		winrt::com_ptr<ID3D11DomainShader> mShader;
	};
}
