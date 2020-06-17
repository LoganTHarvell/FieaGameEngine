#pragma once

#include "Shader.h"
#include "Vector.h"

namespace Library
{
	class VertexShader final : public Shader
	{
		RTTI_DECLARATIONS(VertexShader, Shader)

	public:
		VertexShader() = default;
		VertexShader(const VertexShader&) = default;
		VertexShader& operator=(const VertexShader&) = default;
		VertexShader(VertexShader&&) = default;
		VertexShader& operator=(VertexShader&&) = default;
		~VertexShader() = default;

		const Vector<char>& CompiledShader() const;
		winrt::com_ptr<ID3D11VertexShader> Shader() const;
		winrt::com_ptr<ID3D11InputLayout> InputLayout() const;

		template <typename T>
		void CreateInputLayout(const gsl::not_null<ID3D11Device*> device, const bool releaseCompiledShader = false)
		{
			CreateInputLayout(device, T::InputElements, releaseCompiledShader);
		}

		void CreateInputLayout(gsl::not_null<ID3D11Device*> device, const gsl::span<const D3D11_INPUT_ELEMENT_DESC>& inputElementDescriptions, bool releaseCompiledShader = false);

	private:
		friend class VertexShaderReader;
		
		VertexShader(Vector<char> compiledShader, const winrt::com_ptr<ID3D11VertexShader>& vertexShader);

		Vector<char> mCompiledShader;
		winrt::com_ptr<ID3D11VertexShader> mShader;
		winrt::com_ptr<ID3D11InputLayout> mInputLayout;
	};
}
