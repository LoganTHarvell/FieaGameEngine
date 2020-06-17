#pragma once

#include <vector>
#include "Shader.h"

namespace Library
{
	class GeometryShader final : public Shader
	{
		RTTI_DECLARATIONS(GeometryShader, Shader)

	public:
		GeometryShader(const winrt::com_ptr<ID3D11GeometryShader>& geometryShader=winrt::com_ptr<ID3D11GeometryShader>());
		GeometryShader(const GeometryShader&) = default;
		GeometryShader& operator=(const GeometryShader&) = default;
		GeometryShader(GeometryShader&&) = default;
		GeometryShader& operator=(GeometryShader&&) = default;
		~GeometryShader() = default;

		winrt::com_ptr<ID3D11GeometryShader> Shader() const;

	private:
		winrt::com_ptr<ID3D11GeometryShader> mShader;
	};
}
