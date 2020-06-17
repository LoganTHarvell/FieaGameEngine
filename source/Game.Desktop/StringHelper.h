#pragma once

#include <string>

namespace Library
{
	class StringHelper final
	{
	public:
		static bool EndsWith(const std::string& value, const std::string& ending);
		static bool EndsWith(const std::wstring& value, const std::wstring& ending);

		StringHelper() = delete;
		StringHelper(const StringHelper&) = delete;
		StringHelper& operator=(const StringHelper&) = delete;
		StringHelper(StringHelper&&) = delete;
		StringHelper& operator=(StringHelper&&) = delete;
		~StringHelper() = default;
	};
}