#include "pch.h"
#include "DomainShader.h"

namespace Library
{
	DomainShader::DomainShader(const winrt::com_ptr<ID3D11DomainShader>& domainShader) :
		mShader(domainShader)
	{
	}

	winrt::com_ptr<ID3D11DomainShader> DomainShader::Shader() const
	{
		return mShader;
	}
}