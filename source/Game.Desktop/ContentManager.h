#pragma once

#include <memory>
#include <map>
#include <algorithm>
#include "RTTI.h"
#include "GameException.h"
#include "StringHelper.h"

namespace Library
{
	class Game;

	class ContentManager final
	{
	public:
		ContentManager(Library::Game& game, const std::wstring& rootDirectory = DefaultRootDirectory);
		ContentManager(ContentManager&&) = default;
		ContentManager(const ContentManager&) = delete;
		ContentManager& operator=(const ContentManager&) = delete;		
		ContentManager& operator=(ContentManager&&) = default;
		~ContentManager() = default;

		const std::map<std::wstring, std::shared_ptr<RTTI>>& LoadedAssets() const;
		const std::wstring& RootDirectory() const;
		void SetRootDirectory(const std::wstring& rootDirectory);

		template <typename T>
		std::shared_ptr<T> Load(const std::wstring& assetName, bool reload = false, std::function<std::shared_ptr<T>(std::wstring&)> customReader = nullptr);

		void AddAsset(const std::wstring& assetName, const std::shared_ptr<RTTI>& asset);
		void RemoveAsset(const std::wstring& assetName);
		void Clear();

	private:
		static const std::wstring DefaultRootDirectory;

		std::shared_ptr<RTTI> ReadAsset(const std::int64_t targetTypeId, const std::wstring& assetName);

		Library::Game& mGame;
		std::map<std::wstring, std::shared_ptr<RTTI>> mLoadedAssets;
		std::wstring mRootDirectory;
	};
}

#include "ContentManager.inl"