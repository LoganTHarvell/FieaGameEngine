#pragma once

#include <map>
#include "ContentTypeReader.h"

namespace Library
{
	class ContentTypeReaderManager final
	{
	public:
		ContentTypeReaderManager() = delete;
		ContentTypeReaderManager(const ContentTypeReaderManager&) = delete;
		ContentTypeReaderManager& operator=(const ContentTypeReaderManager&) = delete;
		ContentTypeReaderManager(ContentTypeReaderManager&&) = delete;
		ContentTypeReaderManager& operator=(ContentTypeReaderManager&&) = delete;
		~ContentTypeReaderManager() = default;

		static const std::map<std::uint64_t, std::shared_ptr<AbstractContentTypeReader>>& ContentTypeReaders();
		static bool AddContentTypeReader(std::shared_ptr<AbstractContentTypeReader> reader);
		
		static void Initialize(Game& game);
		static void Shutdown();

	private:
		static std::map<std::uint64_t, std::shared_ptr<AbstractContentTypeReader>> sContentTypeReaders;
		static bool sInitialized;
	};
}