#include "pch.h"
#include "StringHelper.h"

namespace Library
{
	bool StringHelper::EndsWith(const std::string& value, const std::string& ending)
	{
		if (ending.size() > value.size())
		{
			return false;
		}

		return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
	}

	bool StringHelper::EndsWith(const std::wstring& value, const std::wstring& ending)
	{
		if (ending.size() > value.size())
		{
			return false;
		}

		return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
	}
}