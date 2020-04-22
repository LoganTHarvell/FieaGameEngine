#pragma once

// First Party
#include "Vector.h"

namespace Library
{
	namespace TypeTraits
	{
		/// <summary>
		/// Represents whether a type is the same or convertible to another.
		/// </summary>
		/// <typeparam name="T">Type to be compared against.</typeparam>
		/// <typeparam name="T1">Type that is being evaluated.</typeparam>
		/// <typeparam name="R">List of remaining types that are to be evaluated.</typeparam>
		template <typename T, typename T1, typename ... R>
		struct SameOrConvertible
		{
			static const bool Value =	std::is_same<T, T1>::value
									||	std::is_convertible<T, T1>::value
									||	SameOrConvertible<T, R...>::value;
		};

		/// <summary>
		/// Represents whether a type is the same or convertible to another.
		/// </summary>
		/// <typeparam name="T">Type to be compared against.</typeparam>
		/// <typeparam name="T1">Type that is being evaluated.</typeparam>
		template <typename T, typename T1>
		struct SameOrConvertible<T, T1>
		{
			static const bool Value = std::is_same<T, T1>::value || std::is_convertible<T, T1>::value;
		};

		/// <summary>
		/// Wrapper for std::enable_if that will disable a function for the given types.
		/// </summary>
		/// <typeparam name="T">Type to be compared against.</typeparam>
		/// <typeparam name="R">List of types that are will be disabled.</typeparam>
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

	namespace Exception
	{
		/// <summary>
		/// Represents a collection of exceptions.
		/// </summary>
		struct AggregateException final : public std::exception
		{
#pragma region Exception Entry
		public:
			/// <summary>
			/// Wrapper for exception information.
			/// </summary>
			struct Entry final
			{
				/// <summary>
				/// Exception pointer handle.
				/// </summary>
				std::exception_ptr Pointer{ nullptr };

				/// <summary>
				/// File from which the exception was thrown.
				/// </summary>
				std::string File;

				/// <summary>
				/// Line of the file the exception was thrown.
				/// </summary>
				int Line{ -1 };

				/// <summary>
				/// Name of the function called when the exception was thrown.
				/// </summary>
				std::string Function;

				/// <summary>
				/// Extra info pertaining to the exception, if needed.
				/// </summary>
				std::string Info;
			};
#pragma endregion Exception Entry

#pragma region Data Members
		public:
			/// <summary>
			/// List of exception Entry values.
			/// </summary>
			Vector<Entry> Exceptions{ Vector<Entry>::EqualityFunctor() };
#pragma endregion Data Members

#pragma region Special Members
		public:
			/// <summary>
			/// Default constructor.
			/// </summary>
			/// <param name="message">String passed to std::exception base constructor.</param>
			explicit AggregateException(const char* message=nullptr);

			/// <summary>
			/// Default destructor.
			/// </summary>
			~AggregateException() = default;

			/// <summary>
			/// Default copy constructor.
			/// </summary>
			/// <param name="rhs">AggregateException to be copied.</param>
			AggregateException(const AggregateException& rhs) = default;

			/// <summary>
			/// Default copy assignment operator.
			/// </summary>
			/// <param name="rhs">AggregateException to be copied.</param>
			/// <returns>Reference to the newly modified AggregateException.</returns>
			AggregateException& operator=(const AggregateException& rhs) = default;

			/// <summary>
			/// Default move constructor.
			/// </summary>
			/// <param name="rhs">AggregateException to be moved.</param>
			AggregateException(AggregateException&& rhs) noexcept = default;

			/// <summary>
			/// Default move assignment operator.
			/// </summary>
			/// <param name="rhs">AggregateException to be moved.</param>
			/// <returns>Reference to the newly modified AggregateException.</returns>
			AggregateException& operator=(AggregateException&& rhs) noexcept = default;

			/// <summary>
			/// Specialized constructor that initialized the list of exceptions.
			/// </summary>
			/// <param name="message">String passed to std::exception base constructor.</param>
			/// <param name="exceptions">List of exception Entry values to initialize with.</param>
			explicit AggregateException(const char* message, const Vector<Entry>& exceptions);

			/// <summary>
			/// Specialized constructor that initialized the list of exceptions.
			/// </summary>
			/// <param name="message">String passed to std::exception base constructor.</param>
			/// <param name="exceptions">List of exception Entry values to initialize with.</param>
			explicit AggregateException(const char* message, Vector<Entry>&& exceptions);
#pragma endregion Special Members			
		};
	}
}
