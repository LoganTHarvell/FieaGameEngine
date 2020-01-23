#pragma once

#include "DefaultEquality.h"

namespace Library
{
	template<typename T>
	inline const bool DefaultEquality<T>::operator()(const T& lhs, const T& rhs) const
	{
		return lhs == rhs;
	}

#pragma region Default Equality String Specializations
	inline const bool DefaultEquality<char*>::operator()(const char* const lhs, const char* const rhs) const
	{
		return strcmp(lhs, rhs) == 0;
	}
	
	inline const bool DefaultEquality<const char*>::operator()(const char* const lhs, const char* const rhs) const
	{
		return strcmp(lhs, rhs) == 0;
	}

	inline const bool DefaultEquality<char* const>::operator()(const char* const lhs, const char* const rhs) const
	{
		return strcmp(lhs, rhs) == 0;
	}
	
	inline const bool DefaultEquality<const char* const>::operator()(const char* const lhs, const char* const rhs) const
	{
		return strcmp(lhs, rhs) == 0;
	}
#pragma endregion Default Equality String Specializations
}