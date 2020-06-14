#pragma once

#include <cstdint>
#include <string>

namespace Library::Hash
{
#pragma region Hash Functions
	/// <summary>
	/// Simple additive hash function.
	/// </summary>
	std::size_t AdditiveHash(const std::uint8_t* data, const std::size_t byteCount, const std::size_t hashPrime=31);
#pragma endregion Hash Functions

	/// <summary>
	/// Default hash functor for computing hash codes for various types.
	/// </summary>
	/// <param name="key">Key used to compute the hashcode.</param>
	/// <returns>Hash code for the given key.</returns>
	template<typename T>
	struct DefaultHash final
	{
		std::size_t operator()(const T& key) const;
	};

#pragma region Integer Specializations
	/// <summary>
	/// Default hash functor for computing hash codes for int values.
	/// </summary>
	/// <param name="key">Key used to compute the hashcode.</param>
	/// <returns>Hash code for the given key.</returns>
	template<>
	struct DefaultHash<int> final
	{
		std::size_t operator()(const int& key) const;
	};

	/// <summary>
	/// Default hash functor for computing hash codes for const int values.
	/// </summary>
	/// <param name="key">Key used to compute the hashcode.</param>
	/// <returns>Hash code for the given key.</returns>
	template<>
	struct DefaultHash<const int> final
	{
		std::size_t operator()(const int& key) const;
	};
#pragma endregion Integer Specializations

#pragma region String Specializations
	/// <summary>
	/// Default hash functor for computing hash codes for std::string values.
	/// </summary>
	/// <param name="key">Key used to compute the hashcode.</param>
	/// <returns>Hash code for the given key.</returns>
	template<>
	struct DefaultHash<std::string> final
	{
		std::size_t operator()(const std::string& key) const;
	};

	/// <summary>
	/// Default hash functor for computing hash codes for constant std::string values.
	/// </summary>
	/// <param name="key">Key used to compute the hashcode.</param>
	/// <returns>Hash code for the given key.</returns>
	template<>
	struct DefaultHash<const std::string> final
	{
		std::size_t operator()(const std::string& key) const;
	};

	/// <summary>
	/// Default hash functor for computing hash codes for std::wstring values.
	/// </summary>
	/// <param name="key">Key used to compute the hashcode.</param>
	/// <returns>Hash code for the given key.</returns>
	template<>
	struct DefaultHash<std::wstring> final
	{
		std::size_t operator()(const std::wstring& key) const;
	};

	/// <summary>
	/// Default hash functor for computing hash codes for constant std::wstring values.
	/// </summary>
	/// <param name="key">Key used to compute the hashcode.</param>
	/// <returns>Hash code for the given key.</returns>
	template<>
	struct DefaultHash<const std::wstring> final
	{
		std::size_t operator()(const std::wstring& key) const;
	};

	/// <summary>
	/// Default hash functor for computing hash codes for character string values.
	/// </summary>
	/// <param name="key">Key used to compute the hashcode.</param>
	/// <returns>Hash code for the given key.</returns>
	template<>
	struct DefaultHash<char*> final
	{
		std::size_t operator()(const char* const key) const;
	};

	/// <summary>
	/// Default hash functor for computing hash codes for character string values.
	/// </summary>
	/// <param name="key">Key used to compute the hashcode.</param>
	/// <returns>Hash code for the given key.</returns>
	template<>
	struct DefaultHash<const char*> final
	{
		std::size_t operator()(const char* const key) const;
	};

	/// <summary>
	/// Default hash functor for computing hash codes for character string values.
	/// </summary>
	/// <param name="key">Key used to compute the hashcode.</param>
	/// <returns>Hash code for the given key.</returns>
	template<>
	struct DefaultHash<char* const> final
	{
		std::size_t operator()(const char* const key) const;
	};

	/// <summary>
	/// Default hash functor for computing hash codes for character string values.
	/// </summary>
	/// <param name="key">Key used to compute the hashcode.</param>
	/// <returns>Hash code for the given key.</returns>
	template<>
	struct DefaultHash<const char* const> final
	{
		std::size_t operator()(const char* const key) const;
	};
#pragma endregion String Specializations
}

using namespace Library::Hash;

// Inline File
#include "DefaultHash.inl"