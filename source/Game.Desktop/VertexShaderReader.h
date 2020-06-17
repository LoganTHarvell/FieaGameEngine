#pragma once

#include "ContentTypeReader.h"
#include "VertexShader.h"

namespace Library
{
	class VertexShaderReader : public ContentTypeReader<VertexShader>
	{
		RTTI_DECLARATIONS_ABSTRACT(VertexShaderReader, AbstractContentTypeReader)

	public:
		VertexShaderReader(Game& game);
		VertexShaderReader(const VertexShaderReader&) = default;
		VertexShaderReader& operator=(const VertexShaderReader&) = default;
		VertexShaderReader(VertexShaderReader&&) = default;
		VertexShaderReader& operator=(VertexShaderReader&&) = default;
		~VertexShaderReader() = default;

	protected:
		virtual std::shared_ptr<VertexShader> _Read(const std::wstring& assetName) override;
	};
}