#pragma once

namespace Library
{
	namespace String
	{
		/// <summary>
		/// Converts a string to all lowercase characters.
		/// </summary>
		/// <param name="str">String to be converted to lowercase.</param>
		/// <returns>Lowercase copy of the given string.</returns>
		std::string ToLower(const std::string& str);

		/// <summary>
		/// Converts a string to all uppercase characters.
		/// </summary>
		/// <param name="str">String to be converted to uppercase.</param>
		/// <returns>Uppercase copy of the given string.</returns>
		std::string ToUpper(const std::string& str);
	}

	namespace Math
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
}