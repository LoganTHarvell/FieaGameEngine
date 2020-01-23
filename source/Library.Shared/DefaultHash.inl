#pragma once

#include "DefaultHash.h"

namespace Library
{
#pragma region Hash Functions
	inline const std::size_t AdditiveHash(const std::uint8_t* data, const std::size_t byteCount, const std::size_t hashPrime)
	{
		std::size_t hashValue = 0;

		for (std::size_t i = 0; i < byteCount; ++i)
		{
			hashValue += hashPrime * data[i];
		}

		return hashValue;
	}
#pragma endregion Hash Functions

	template<typename T>
	inline const std::size_t DefaultHash<T>::operator()(const T& key) const
	{
		const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(&key);
		return AdditiveHash(data, sizeof(T));
	}

#pragma region Default Hash String Specializations
	inline const std::size_t DefaultHash<std::string>::operator()(const std::string& key) const
	{
		const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(key.c_str());
		return AdditiveHash(data, key.length());
	}

	inline const std::size_t DefaultHash<const std::string>::operator()(const std::string& key) const
	{
		const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(key.c_str());
		return AdditiveHash(data, key.length());
	}

	inline const std::size_t DefaultHash<std::wstring>::operator()(const std::wstring& key) const
	{
		const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(key.c_str());
		return AdditiveHash(data, key.length() * sizeof(wchar_t));
	}

	inline const std::size_t DefaultHash<const std::wstring>::operator()(const std::wstring& key) const
	{
		const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(key.c_str());
		return AdditiveHash(data, key.length() * sizeof(wchar_t));
	}

	inline const std::size_t DefaultHash<char*>::operator()(const char* const key) const
	{
		const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(key);
		return AdditiveHash(data, strlen(key));
	}

	inline const std::size_t DefaultHash<const char*>::operator()(const char* const key) const
	{
		const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(key);
		return AdditiveHash(data, strlen(key));
	}

	inline const std::size_t DefaultHash<char* const>::operator()(const char* const key) const
	{
		const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(key);
		return AdditiveHash(data, strlen(key));
	}

	inline const std::size_t DefaultHash<const char* const>::operator()(const char* const key) const
	{
		const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(key);
		return AdditiveHash(data, strlen(key));
	}
#pragma endregion Default Hash String Specializations
}