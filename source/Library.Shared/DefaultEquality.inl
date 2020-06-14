#pragma once

// Header
#include "DefaultEquality.h"

namespace Library::Equality
{
	template<typename T>
	inline bool DefaultEquality<T>::operator()(const T& lhs, const T& rhs) const
	{
		return lhs == rhs;
	}

#pragma region Default Equality String Specializations
	inline bool DefaultEquality<char*>::operator()(const char* const lhs, const char* const rhs) const
	{
		return strcmp(lhs, rhs) == 0;
	}
	
	inline bool DefaultEquality<const char*>::operator()(const char* const lhs, const char* const rhs) const
	{
		return strcmp(lhs, rhs) == 0;
	}

	inline bool DefaultEquality<char* const>::operator()(const char* const lhs, const char* const rhs) const
	{
		return strcmp(lhs, rhs) == 0;
	}
	
	inline bool DefaultEquality<const char* const>::operator()(const char* const lhs, const char* const rhs) const
	{
		return strcmp(lhs, rhs) == 0;
	}
#pragma endregion Default Equality String Specializations
}