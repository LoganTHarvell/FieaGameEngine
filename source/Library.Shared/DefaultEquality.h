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
	struct DefaultEquality final
	{		
		const bool operator()(const T& a, const T& b) const
		{
			return a == b;
		}
	};

	/// <summary>
	/// Defines default callable equality functors for char pointers.
	/// </summary>
	/// <returns>True on equal, false otherwise.</returns>
	template<>
	struct DefaultEquality<char*> final
	{
		const bool operator()(char* a, char* b) const
		{
			return strcmp(a, b) == 0;
		}
	};

	/// <summary>
	/// Defines default callable equality functors for pointers to constant char values.
	/// </summary>
	/// <returns>True on equal, false otherwise.</returns>
	template<>
	struct DefaultEquality<const char*> final
	{
		const bool operator()(const char* a, const char* b) const
		{
			return strcmp(a, b) == 0;
		}
	};


	/// <summary>
	/// Defines default callable equality functors for constant pointers to char values.
	/// </summary>
	/// <returns>True on equal, false otherwise.</returns>
	template<>
	struct DefaultEquality<char* const> final
	{
		const bool operator()(char* const a, char* const b) const
		{
			return strcmp(a, b) == 0;
		}
	};

	/// <summary>
	/// Defines default callable equality functors for constant value constant char pointers.
	/// </summary>
	/// <returns>True on equal, false otherwise.</returns>
	template<>
	struct DefaultEquality<const char* const> final
	{
		const bool operator()(const char* const a, const char* const b) const
		{
			return strcmp(a, b) == 0;
		}
	};
}