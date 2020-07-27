#pragma once

#include "ContentTypeReader.h"
#include "Shader.h"

namespace Library
{
	class ComputeShaderReader : public ContentTypeReader<ComputeShader>
	{
		RTTI_DECLARATIONS_ABSTRACT(ComputeShaderReader, AbstractContentTypeReader)

	public:
		explicit ComputeShaderReader(Game& game);
		ComputeShaderReader(const ComputeShaderReader&) = default;
		ComputeShaderReader& operator=(const ComputeShaderReader&) = default;
		ComputeShaderReader(ComputeShaderReader&&) = default;
		ComputeShaderReader& operator=(ComputeShaderReader&&) = default;
		~ComputeShaderReader() = default;

	protected:
		virtual std::shared_ptr<ComputeShader> _Read(const std::wstring& assetName) override;
	};
}