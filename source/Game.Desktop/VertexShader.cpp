#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "VertexShader.h"

// First Party
#include "CoreD3D.h"
#include "VertexAttribute.h"
#include "GameException.h"
#pragma endregion Includes

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

	void VertexShader::CreateInputLayout(gsl::not_null<ID3D11Device*> device, const gsl::span<const VertexAttribute>& vertexAttributes, const bool releaseCompiledShader)
	{
		if (vertexAttributes.empty()) return;
		
		Vector inputElementDescriptors(vertexAttributes.size(), Vector<D3D11_INPUT_ELEMENT_DESC>::EqualityFunctor());

		for (const auto& va : vertexAttributes)
		{
			D3D11_INPUT_ELEMENT_DESC desc
			{
				va.Name.c_str(),
				va.SemanticIndex,
				Direct3D::FormatMap[va.FormatValue],
				va.Slot,
				D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
				va.InstanceDivisor
			};
			
			inputElementDescriptors.EmplaceBack(desc);
		}
		
		mInputLayout = nullptr;
		ThrowIfFailed(device->CreateInputLayout(inputElementDescriptors.Data(), gsl::narrow_cast<uint32_t>(inputElementDescriptors.Size()), &mCompiledShader[0], mCompiledShader.Size(), mInputLayout.put()), "ID3D11Device::CreateInputLayout() failed.");

		if (releaseCompiledShader)
		{
			mCompiledShader.Clear();
			mCompiledShader.ShrinkToFit();
		}
	}
}