#include "pch.h"
#include "VertexShader.h"
#include "GameException.h"

using namespace DirectX;
using namespace winrt;

namespace Library
{
	VertexShader::VertexShader(Vector<char> compiledShader, const com_ptr<ID3D11VertexShader>& vertexShader) :
		mCompiledShader(std::move(compiledShader)), mShader(vertexShader)
	{
	}

	const Vector<char>& VertexShader::CompiledShader() const
	{
		return mCompiledShader;
	}

	com_ptr<ID3D11VertexShader> VertexShader::Shader() const
	{
		return mShader;
	}

	com_ptr<ID3D11InputLayout> VertexShader::InputLayout() const
	{
		return mInputLayout;
	}

	void VertexShader::CreateInputLayout(gsl::not_null<ID3D11Device*> device, const gsl::span<const D3D11_INPUT_ELEMENT_DESC>& inputElementDescriptions, const bool releaseCompiledShader)
	{
		mInputLayout = nullptr;
		ThrowIfFailed(device->CreateInputLayout(&inputElementDescriptions[0], gsl::narrow_cast<uint32_t>(inputElementDescriptions.size()), &mCompiledShader[0], mCompiledShader.Size(), mInputLayout.put()), "ID3D11Device::CreateInputLayout() failed.");

		if (releaseCompiledShader)
		{
			mCompiledShader.Clear();
			mCompiledShader.ShrinkToFit();
		}
	}
}