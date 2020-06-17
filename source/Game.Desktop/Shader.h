#pragma once

#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include <gsl/gsl>
#include "RTTI.h"

namespace Library
{
	class Shader : public RTTI
	{
		RTTI_DECLARATIONS(Shader, RTTI)

	public:		
		Shader(const Shader&) = default;
		Shader& operator=(const Shader&) = default;
		Shader(Shader&&) = default;
		Shader& operator=(Shader&&) = default;
		virtual ~Shader() = default;

		static winrt::com_ptr<ID3D11ClassLinkage> CreateClassLinkage(gsl::not_null<ID3D11Device*> device);

	protected:
		Shader() = default;
	};
}