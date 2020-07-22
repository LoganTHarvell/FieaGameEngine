#pragma once

#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include <gsl/gsl>

#include "RTTI.h"
#include "HashMap.h"

namespace Library
{
#pragma region Shaders
	/**
	 * @brief Shader stages enumeration.
	*/
	enum class ShaderStages
	{
		IA,
		VS,
		HS,
		DS,
		GS,
		SO,
		RS,
		PS,
		OM,

		CS
	};

	/**
	 * @brief Programmable shader stages listing.
	*/
	constexpr std::array<ShaderStages, 6> ProgrammableGraphicsShaderStages
	{
		ShaderStages::VS,
		ShaderStages::HS,
		ShaderStages::DS,
		ShaderStages::GS,
		ShaderStages::PS,
		ShaderStages::CS,
	};

	/**
	 * @brief Validates if a shader stage is programmable.
	 * @param shaderStage Shader stage to be validated.
	 * @return True, if shader stage is programmable. Otherwise, false.
	*/
	inline bool ShaderStageIsProgrammable(const ShaderStages shaderStage)
	{
		static const HashMap<ShaderStages, bool> isProgrammableMap
		{
			{ ShaderStages::IA, false },
			{ ShaderStages::VS, true },
			{ ShaderStages::HS, true },
			{ ShaderStages::DS, true },
			{ ShaderStages::GS, true },
			{ ShaderStages::SO, false },
			{ ShaderStages::RS, false },
			{ ShaderStages::PS, true },
			{ ShaderStages::OM, false },
			{ ShaderStages::CS, true },
		};

		return isProgrammableMap.At(shaderStage);
	}
#pragma endregion Shaders
	
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