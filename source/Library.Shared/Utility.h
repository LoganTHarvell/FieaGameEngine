#pragma once

namespace Library::Math
{
	/// <summary>
	/// Checks if the given value is a prime number.
	/// </summary>
	/// <param name="value">Number to check for prime status.</param>
	bool IsPrime(std::size_t value);

	/// <summary>
	/// Finds the next prime number greater than the given value.
	/// </summary>
	/// <param name="value">Number to start from when finding the next prime number.</param>
	/// <returns>Prime number greater than the given value.</returns>
	std::size_t FindNextPrime(const std::size_t value);
}