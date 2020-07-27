#pragma once

// Header

#include "ShaderD3D11.h"

namespace Library
{
	inline VertexShaderD3D11::VertexShaderD3D11(const winrt::com_ptr<ID3D11VertexShader>& vertexShader) :
		mShader(vertexShader)
	{
	}

	inline VertexShaderD3D11::VertexShaderD3D11(Vector<char> compiledShader, const winrt::com_ptr<ID3D11VertexShader>& vertexShader) :
		mCompiledShader(std::move(compiledShader)), mShader(vertexShader)
	{
	}

	inline const Vector<char>& VertexShaderD3D11::CompiledShader() const
	{
		return mCompiledShader;
	}

	inline winrt::com_ptr<ID3D11VertexShader> VertexShaderD3D11::Native() const
	{
		return mShader;
	}

	inline winrt::com_ptr<ID3D11InputLayout> VertexShaderD3D11::InputLayout() const
	{
		return mInputLayout;
	}

	inline HullShaderD3D11::HullShaderD3D11(const winrt::com_ptr<ID3D11HullShader>& hullShader) :
		mShader(hullShader)
	{
	}

	inline winrt::com_ptr<ID3D11HullShader> HullShaderD3D11::Native() const
	{
		return mShader;
	}

	inline DomainShaderD3D11::DomainShaderD3D11(const winrt::com_ptr<ID3D11DomainShader>& domainShader) :
		mShader(domainShader)
	{
	}

	inline winrt::com_ptr<ID3D11DomainShader> DomainShaderD3D11::Native() const
	{
		return mShader;
	}

	inline GeometryShaderD3D11::GeometryShaderD3D11(const winrt::com_ptr<ID3D11GeometryShader>& geometryShader) :
		mShader(geometryShader)
	{
	}

	inline winrt::com_ptr<ID3D11GeometryShader> GeometryShaderD3D11::Native() const
	{
		return mShader;
	}

	inline PixelShaderD3D11::PixelShaderD3D11(const winrt::com_ptr<ID3D11PixelShader>& pixelShader) :
		mShader(pixelShader)
	{
	}

	inline winrt::com_ptr<ID3D11PixelShader> PixelShaderD3D11::Native() const
	{
		return mShader;
	}

	inline ComputeShaderD3D11::ComputeShaderD3D11(const winrt::com_ptr<ID3D11ComputeShader>& computeShader) :
		mShader(computeShader)
	{
	}
	
	inline winrt::com_ptr<ID3D11ComputeShader> ComputeShaderD3D11::Native() const
	{
		return mShader;
	}
}
