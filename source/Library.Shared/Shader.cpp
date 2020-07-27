// Pre-compiled Header
#include "pch.h"

// Header
#include "Shader.h"

namespace Library
{
	const HashMap<ShaderStages, RTTI::IdType> Shader::StageTypeMap
	{
		{ ShaderStages::VS, VertexShader::TypeIdClass() },
		{ ShaderStages::HS, HullShader::TypeIdClass() },
		{ ShaderStages::DS, DomainShader::TypeIdClass() },
		{ ShaderStages::GS, GeometryShader::TypeIdClass() },
		{ ShaderStages::PS, PixelShader::TypeIdClass() },
		{ ShaderStages::CS, ComputeShader::TypeIdClass() },
	};

	const HashMap<RTTI::IdType, ShaderStages> Shader::TypeStageMap
	{
		{ VertexShader::TypeIdClass(), ShaderStages::VS },
		{ HullShader::TypeIdClass(), ShaderStages::HS },
		{ DomainShader::TypeIdClass(), ShaderStages::DS },
		{ GeometryShader::TypeIdClass(), ShaderStages::GS },
		{ PixelShader::TypeIdClass(), ShaderStages::PS },
		{ ComputeShader::TypeIdClass(), ShaderStages::CS },
	};
}
