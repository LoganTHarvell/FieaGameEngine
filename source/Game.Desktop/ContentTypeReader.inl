#pragma once

namespace Library
{
	template<typename T>
	inline ContentTypeReader<T>::ContentTypeReader(Game& game, const std::uint64_t targetTypeId) :
		AbstractContentTypeReader(game, targetTypeId)
	{
	}

	template<typename T>
	inline std::shared_ptr<RTTI> ContentTypeReader<T>::Read(const std::wstring& assetName)
	{
		return _Read(assetName);
	}
}