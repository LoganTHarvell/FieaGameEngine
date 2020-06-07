#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "MathUtility.h"
#pragma endregion Includes

namespace Library::Math
{
	bool IsPrime(const std::size_t value)
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
