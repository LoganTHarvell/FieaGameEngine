#pragma once

#include <string>
#include <functional>

namespace Library
{
	/// <summary>
	/// Default callable equality functors for comparisons of various types.
	/// </summary>
	/// <returns>True on equal, false otherwise.</returns>
	template<typename T>
	struct DefaultEquality final
	{		
		bool operator()(const T& lhs, const T& rhs) const;
	};

#pragma region Default Equality String Specializations
	/// <summary>
	/// Default callable equality functor specialization for char pointers.
	/// </summary>
	/// <returns>True on equal, false otherwise.</returns>
	template<>
	struct DefaultEquality<char*> final
	{
		bool operator()(const char* const lhs, const char* const rhs) const;
	};

	/// <summary>
	/// Default callable equality functor specialization for pointers to constant char values.
	/// </summary>
	/// <returns>True on equal, false otherwise.</returns>
	template<>
	struct DefaultEquality<const char*> final
	{
		bool operator()(const char* const lhs, const char* const rhs) const;
	};

	/// <summary>
	/// Default callable equality functor specialization for constant pointers to char values.
	/// </summary>
	/// <returns>True on equal, false otherwise.</returns>
	template<>
	struct DefaultEquality<char* const> final
	{
		bool operator()(const char* const lhs, const char* const rhs) const;
	};

	/// <summary>
	/// Default callable equality functor specialization for constant value constant char pointers.
	/// </summary>
	/// <returns>True on equal, false otherwise.</returns>
	template<>
	struct DefaultEquality<const char* const> final
	{
		bool operator()(const char* const lhs, const char* const rhs) const;
	};
#pragma endregion Default Equality String Specializations
}

#include "DefaultEquality.inl"