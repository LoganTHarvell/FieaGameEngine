#pragma once

#include <memory>
#include <gsl\gsl>
#include "RTTI.h"

namespace Library
{
	class Game;

	class AbstractContentTypeReader : public RTTI
	{
		RTTI_DECLARATIONS_ABSTRACT(AbstractContentTypeReader, RTTI)

	public:
		AbstractContentTypeReader() = delete;
		AbstractContentTypeReader(const AbstractContentTypeReader&) = default;
		AbstractContentTypeReader& operator=(const AbstractContentTypeReader&) = delete;
		AbstractContentTypeReader(AbstractContentTypeReader&&) = default;
		AbstractContentTypeReader& operator=(AbstractContentTypeReader&&) = delete;
		virtual ~AbstractContentTypeReader() = default;

		std::uint64_t TargetTypeId() const;
		virtual std::shared_ptr<RTTI> Read(const std::wstring& assetName) = 0;

	protected:
		AbstractContentTypeReader(Game& game, const std::uint64_t targetTypeId);

		gsl::not_null<Game*> mGame;
		const std::uint64_t mTargetTypeId;
	};

	template <typename T>
	class ContentTypeReader : public AbstractContentTypeReader
	{
	public:
		ContentTypeReader() = delete;
		ContentTypeReader(const ContentTypeReader&) = default;
		ContentTypeReader& operator=(const ContentTypeReader&) = delete;
		ContentTypeReader(ContentTypeReader&&) = default;
		ContentTypeReader& operator=(ContentTypeReader&&) = delete;
		virtual ~ContentTypeReader() = default;

		virtual std::shared_ptr<RTTI> Read(const std::wstring& assetName) override;

	protected:
		ContentTypeReader(Game& game, const std::uint64_t targetTypeId);

		virtual std::shared_ptr<T> _Read(const std::wstring& assetName) = 0;
	};
}

#include "ContentTypeReader.inl"