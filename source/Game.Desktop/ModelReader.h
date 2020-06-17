#pragma once

#include "ContentTypeReader.h"
#include "Model.h"

namespace Library
{
	class ModelReader : public ContentTypeReader<Model>
	{
		RTTI_DECLARATIONS_ABSTRACT(ModelReader, AbstractContentTypeReader)

	public:
		ModelReader(Game& game);
		ModelReader(const ModelReader&) = default;
		ModelReader& operator=(const ModelReader&) = default;
		ModelReader(ModelReader&&) = default;
		ModelReader& operator=(ModelReader&&) = default;
		~ModelReader() = default;

	protected:
		virtual std::shared_ptr<Model> _Read(const std::wstring& assetName) override;
	};
}