#pragma once

#include "ContentTypeReader.h"
#include "GeometryShader.h"

namespace Library
{
	class GeometryShaderReader : public ContentTypeReader<GeometryShader>
	{
		RTTI_DECLARATIONS_ABSTRACT(GeometryShaderReader, AbstractContentTypeReader)

	public:
		GeometryShaderReader(Game& game);
		GeometryShaderReader(const GeometryShaderReader&) = default;
		GeometryShaderReader& operator=(const GeometryShaderReader&) = default;
		GeometryShaderReader(GeometryShaderReader&&) = default;
		GeometryShaderReader& operator=(GeometryShaderReader&&) = default;
		~GeometryShaderReader() = default;

	protected:
		virtual std::shared_ptr<GeometryShader> _Read(const std::wstring& assetName) override;
	};
}