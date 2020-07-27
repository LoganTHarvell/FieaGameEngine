#pragma once

// Third Party
#include <gsl/gsl>

// First Party
#include "RTTI.h"
#include "HashMap.h"
#include "VertexAttribute.h"

namespace Library
{
	// Forwarded Declarations
	class RenderingManager;
	
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

	/**
	 * @brief Shader abstraction layer
	*/
	class Shader : public RTTI
	{
		RTTI_DECLARATIONS_ABSTRACT(Shader, RTTI)

	public:
		static const HashMap<ShaderStages, IdType> StageTypeMap;
		static const HashMap<IdType, ShaderStages> TypeStageMap;
		
	public:		
		virtual ~Shader() = default;
		Shader(const Shader&) = default;
		Shader& operator=(const Shader&) = default;
		Shader(Shader&&) = default;
		Shader& operator=(Shader&&) = default;

	protected:
		Shader() = default;

	public:
		virtual ShaderStages ShaderStage() const = 0;
	};

	class VertexShader : public Shader
	{
		RTTI_DECLARATIONS_ABSTRACT(VertexShader, Shader)

	public:
		virtual ~VertexShader() = default;
		VertexShader(const VertexShader&) = default;
		VertexShader& operator=(const VertexShader&) = default;
		VertexShader(VertexShader&&) = default;
		VertexShader& operator=(VertexShader&&) = default;

	protected:
		VertexShader() = default;
		
	public:
		virtual ShaderStages ShaderStage() const override { return ShaderStages::VS; };

		/**
		 * @brief Creates an input layout based on the given vertex attributes
		 * @tparam T VertexDeclaration type with predefined vertex attributes
		 * @param renderingManager RenderingManager that is used to create the input layout
		 * @param releaseCompiledShader Boolean whether the compiled shader should be released
		*/
		template <typename T>
		void CreateInputLayout(const RenderingManager& renderingManager, const bool releaseCompiledShader = false)
		{
			CreateInputLayout(renderingManager, T::InputElements, releaseCompiledShader);
		}

		/**
		 * @brief Creates an input layout based on the given vertex attributes
		 * @param renderingManager RenderingManager that is used to create the input layout
		 * @param vertexAttributes VertexAttributes used to describe the input layout
		 * @param releaseCompiledShader Boolean whether the compiled shader should be released
		*/
		virtual void CreateInputLayout(const RenderingManager&, const gsl::span<const VertexAttribute>&, bool=false) {}
	};

	class HullShader : public Shader
	{
		RTTI_DECLARATIONS_ABSTRACT(HullShader, Shader)

	public:
		virtual ~HullShader() = default;
		HullShader(const HullShader&) = default;
		HullShader& operator=(const HullShader&) = default;
		HullShader(HullShader&&) = default;
		HullShader& operator=(HullShader&&) = default;

	protected:
		HullShader() = default;

	public:
		virtual ShaderStages ShaderStage() const override { return ShaderStages::HS; }
	};

	class DomainShader : public Shader
	{
		RTTI_DECLARATIONS_ABSTRACT(DomainShader, Shader)

	public:
		virtual ~DomainShader() = default;
		DomainShader(const DomainShader&) = default;
		DomainShader& operator=(const DomainShader&) = default;
		DomainShader(DomainShader&&) = default;
		DomainShader& operator=(DomainShader&&) = default;

	protected:
		DomainShader() = default;

	public:
		virtual ShaderStages ShaderStage() const override { return ShaderStages::DS; }
	};

	class GeometryShader : public Shader
	{
		RTTI_DECLARATIONS_ABSTRACT(GeometryShader, Shader)

	public:
		virtual ~GeometryShader() = default;
		GeometryShader(const GeometryShader&) = default;
		GeometryShader& operator=(const GeometryShader&) = default;
		GeometryShader(GeometryShader&&) = default;
		GeometryShader& operator=(GeometryShader&&) = default;

	protected:
		GeometryShader() = default;

	public:
		virtual ShaderStages ShaderStage() const override { return ShaderStages::GS; }
	};

	class PixelShader : public Shader
	{
		RTTI_DECLARATIONS_ABSTRACT(PixelShader, Shader)

	public:
		virtual ~PixelShader() = default;
		PixelShader(const PixelShader&) = default;
		PixelShader& operator=(const PixelShader&) = default;
		PixelShader(PixelShader&&) = default;
		PixelShader& operator=(PixelShader&&) = default;

	protected:
		PixelShader() = default;

	public:
		virtual ShaderStages ShaderStage() const override { return ShaderStages::PS; }
	};

	class ComputeShader : public Shader
	{
		RTTI_DECLARATIONS_ABSTRACT(ComputeShader, Shader)

	public:
		virtual ~ComputeShader() = default;
		ComputeShader(const ComputeShader&) = default;
		ComputeShader& operator=(const ComputeShader&) = default;
		ComputeShader(ComputeShader&&) = default;
		ComputeShader& operator=(ComputeShader&&) = default;

	protected:
		ComputeShader() = default;

	public:
		virtual ShaderStages ShaderStage() const override { return ShaderStages::CS; }
	};
}
