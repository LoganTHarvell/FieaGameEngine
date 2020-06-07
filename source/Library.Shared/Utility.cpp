#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "Utility.h"

// Standard
#include <string>
#include <algorithm>
#pragma endregion Includes

namespace Library
{	
	namespace String
	{
		std::string ToLower(const std::string& str)
		{
			std::string lowercaseString = str;
			
			auto toLower = [](unsigned char c) { return static_cast<unsigned char>(std::tolower(c)); };
			std::transform(lowercaseString.begin(), lowercaseString.end(), lowercaseString.begin(), toLower);

			return lowercaseString;
		}

		std::string ToUpper(const std::string& str)
		{
			std::string uppercaseString = str;
			
			auto toUpper = [](unsigned char c) { return static_cast<unsigned char>(std::toupper(c)); };
			std::transform(uppercaseString.begin(), uppercaseString.end(), uppercaseString.begin(), toUpper);

			return uppercaseString;
		}
	}

	namespace Exception
	{
		AggregateException::AggregateException(const char* message) : std::exception(message)
		{
		}
		
		AggregateException::AggregateException(const char* message, const Vector<Entry>& exceptions) : std::exception(message),
			Exceptions(exceptions)
		{
		}

		AggregateException::AggregateException(const char* message, Vector<Entry>&& exceptions) : std::exception(message),
			Exceptions(std::move(exceptions))
		{
		}
	}
}
