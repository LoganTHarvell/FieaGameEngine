#pragma once

#include "ContentTypeReader.h"
#include "Shader.h"

namespace Library
{
	class HullShaderReader : public ContentTypeReader<HullShader>
	{
		RTTI_DECLARATIONS_ABSTRACT(HullShaderReader, AbstractContentTypeReader)

	public:
		HullShaderReader(Game& game);
		HullShaderReader(const HullShaderReader&) = default;
		HullShaderReader& operator=(const HullShaderReader&) = default;
		HullShaderReader(HullShaderReader&&) = default;
		HullShaderReader& operator=(HullShaderReader&&) = default;
		~HullShaderReader() = default;

	protected:
		virtual std::shared_ptr<HullShader> _Read(const std::wstring& assetName) override;
	};
}