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
	namespace TypeTraits
	{	
		template <typename T, typename T1, typename ... R>
		struct SameOrConvertible
		{
			static const bool Value =	std::is_same<T, T1>::value
									||	std::is_convertible<T, T1>::value
									||	SameOrConvertible<T, R...>::value;
		};

		template <typename T, typename T1>
		struct SameOrConvertible<T, T1>
		{
			static const bool Value = std::is_same<T, T1>::value || std::is_convertible<T, T1>::value;
		};

		template <typename T, typename ... R>
		struct DisableForward
		{
		private:
			using RemovedRefT = typename std::remove_reference<typename std::remove_cv<T>::type>::type;

		public:
			static const bool Value = SameOrConvertible<RemovedRefT, R...>::value;
			using Type = std::enable_if<Value, int>;
		};
	}
	
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

	namespace Math
	{
		bool IsPrime(std::size_t value)
		{
			if (value <= 1)							return false;
			if (value <= 3)							return true;
			if (value % 2 == 0 || value % 3 == 0)	return false;
	
			std::size_t divisor = 6;
			while ((divisor * divisor) - (2 * divisor) + std::size_t(1) <= value)
			{
				if (value % (divisor - std::size_t(1)) == 0) return false;
				if (value % (divisor + std::size_t(1)) == 0) return false;
	
				divisor += 6;
			}
	
			return true;
		}
	
		std::size_t FindNextPrime(const std::size_t value)
		{
			std::size_t prime = value;
			while (!IsPrime(++prime));
			return prime;
		}
	}
}
