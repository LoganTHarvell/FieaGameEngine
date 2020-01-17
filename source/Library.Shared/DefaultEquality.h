#pragma once

#include <string>
#include <functional>

namespace Library
{
	/// <summary>
	/// Defines default callable equality functors for comparisons of various types.
	/// </summary>
	/// <returns>True on equal, false otherwise.</returns>
	template<typename T>
	struct DefaultEquality
	{		
		const bool operator()(const T& a, const T& b) const
		{
			return a == b;
		}
	};

	template<>
	struct DefaultEquality<char*>
	{
		const bool operator()(char* a, char* b) const
		{
			return strcmp(a, b) == 0;
		}
	};

	template<>
	struct DefaultEquality<const char*>
	{
		const bool operator()(const char* a, const char* b) const
		{
			return strcmp(a, b) == 0;
		}
	};

	template<>
	struct DefaultEquality<char* const>
	{
		const bool operator()(char* const a, char* const b) const
		{
			return strcmp(a, b) == 0;
		}
	};

	template<>
	struct DefaultEquality<const char* const>
	{
		const bool operator()(const char* const a, const char* const b) const
		{
			return strcmp(a, b) == 0;
		}
	};
}