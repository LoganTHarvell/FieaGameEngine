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

		void ToWideString(const std::string& source, std::wstring& dest);
		std::wstring ToWideString(const std::string& source);
		void ToString(const std::wstring& source, std::string& dest);
		std::string ToString(const std::wstring& source);

		bool EndsWith(const std::string& value, const std::string& ending);
		bool EndsWith(const std::wstring& value, const std::wstring& ending);
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

#pragma region Special Members
		public:
			/// <summary>
			/// Default constructor.
			/// </summary>
			explicit AggregateException() = default;

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
			/// <param name="exceptions">List of exception Entry values to initialize with.</param>
			explicit AggregateException(const Vector<Entry>& exceptions);

			/// <summary>
			/// Specialized constructor that initialized the list of exceptions.
			/// </summary>
			/// <param name="exceptions">List of exception Entry values to initialize with.</param>
			explicit AggregateException(Vector<Entry>&& exceptions);
#pragma endregion Special Members		

#pragma region Data Members
		public:
			/// <summary>
			/// List of exception Entry values.
			/// </summary>
			Vector<Entry> Exceptions{ Vector<Entry>::EqualityFunctor() };
#pragma endregion Data Members	
		};
	}

	namespace File
	{	
		void GetName(const std::string& inputPath, std::string& filename);
		void GetDirectory(const std::string& inputPath, std::string& directory);
		std::tuple<std::string, std::string> GetNameAndDirectory(const std::string& inputPath);
		void LoadBinary(const std::string& filename, Vector<char>& data);
	}

	template <typename T>
	void UpdateValue(const std::function<bool()>& increasePredicate, const std::function<bool()>& decreasePredicate, T& value, const T& delta, std::function<void(const T&)> updateFunc = nullptr, const T& minValue = std::numeric_limits<T>::min(), const T& maxValue = std::numeric_limits<T>::max())
	{
		bool update = false;
		if (increasePredicate() && value < maxValue)
		{
			value += delta;
			value = std::min(value, maxValue);
			update = true;
		}
		else if (decreasePredicate() && value > minValue)
		{
			value -= delta;
			value = std::max(value, minValue);
			update = true;
		}

		if (update && updateFunc != nullptr)
		{
			updateFunc(value);
		}
	}
}
