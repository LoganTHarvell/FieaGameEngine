#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "ShaderD3D11.h"

// First Party
#include "CoreD3D.h"
#include "RenderingManagerD3D11.h"
#include "VertexAttribute.h"
#include "GameException.h"
#pragma endregion Includes

namespace Library
{
	void VertexShaderD3D11::CreateInputLayout(const RenderingManager& renderingManager, const gsl::span<const VertexAttribute>& vertexAttributes, const bool releaseCompiledShader)
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
		ThrowIfFailed(renderingManager.AssertAs<RenderingManagerD3D11>()->Device()->CreateInputLayout(inputElementDescriptors.Data(), gsl::narrow_cast<uint32_t>(inputElementDescriptors.Size()), &mCompiledShader[0], mCompiledShader.Size(), mInputLayout.put()), "ID3D11Device::CreateInputLayout() failed.");

		if (releaseCompiledShader)
		{
			mCompiledShader.Clear();
			mCompiledShader.ShrinkToFit();
		}
	}
}