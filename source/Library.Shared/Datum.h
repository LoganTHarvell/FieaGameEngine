#pragma once

#pragma region Includes
// Standard
#include <string>
#include <type_traits>

// Third Party
#include <gsl/span>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// First Party
#include "RTTI.h"
#pragma endregion Includes

namespace Library
{
	/// <summary>
	/// Runtime type polymorphic wrapper for data elements.
	/// </summary>
	class Datum final
	{
#pragma region Type Definitions and Constants
	public:
		/// <summary>
		/// Alias for a pointer to an RTTI class instance.
		/// </summary>
		using RTTIPointer = RTTI*;

		/// <summary>
		/// Reserve strategy functor type used during resize on insertion.
		/// </summary>
		using ReserveFunctor = std::function<std::size_t(const std::size_t, const std::size_t)>;

		/// <summary>
		/// Represents one of the valid types that datum can contain.
		/// </summary>
		enum class DatumTypes
		{
			Unknown = -1,
			
			Integer,
			Float,
			Vector,
			Matrix,
			String,
			Pointer,
			
			End
		};

	private:
		/// <summary>
		/// Wrapper for a pointer to data of any type that datum can contain.
		/// </summary>
		union DatumValue
		{
			int* intPtr;
			float* floatPtr;
			glm::vec4* vectorPtr;
			glm::mat4* matrixPtr;
			std::string* stringPtr;
			RTTIPointer* rttiPtr;

			// Convenience Pointers
			void* voidPtr;
			std::uint8_t* bytePtr;
		};

		/// <summary>
		/// Datum type size look-up table.
		/// </summary>
		inline static constexpr std::size_t DatumSizeLUT[static_cast<std::size_t>(DatumTypes::End)] = 
		{ 
			sizeof(int), sizeof(float),
			sizeof(glm::vec4), sizeof(glm::mat4),
			sizeof(std::string), sizeof(RTTIPointer) 
		};

		/// <summary>
		/// Datum ToString look-up table.
		/// </summary>
		inline static const std::function<std::string(void*, std::size_t)> DatumToStringLUT[static_cast<std::size_t>(DatumTypes::End)] =
		{
			[](void* data, std::size_t index) { return std::to_string(reinterpret_cast<int*>(data)[index]); },
			[](void* data, std::size_t index) { return std::to_string(reinterpret_cast<float*>(data)[index]); },
			[](void* data, std::size_t index) { return glm::to_string(reinterpret_cast<glm::vec4*>(data)[index]); },
			[](void* data, std::size_t index) { return glm::to_string(reinterpret_cast<glm::mat4*>(data)[index]); },
			[](void* data, std::size_t index) { return reinterpret_cast<std::string*>(data)[index]; },
			[](void* data, std::size_t index) { RTTIPointer ptr = reinterpret_cast<RTTIPointer*>(data)[index]; return ptr ? ptr->ToString() : "nullptr";  }
		};

		/// <summary>
		/// Datum FromString conversion look-up table.
		/// </summary>
		inline static const std::function<void(std::string, void*, std::size_t)> DatumFromStringLUT[static_cast<std::size_t>(DatumTypes::End)] =
		{
			[](std::string str, void* data, std::size_t index) { sscanf_s(str.c_str(), "%d", &reinterpret_cast<int*>(data)[index]); },
			[](std::string str, void* data, std::size_t index) { sscanf_s(str.c_str(), "%f", &reinterpret_cast<float*>(data)[index]); },
			[](std::string str, void* data, std::size_t index) 
			{ 
				float* vector = glm::value_ptr(reinterpret_cast<glm::vec4*>(data)[index]);
				sscanf_s(str.c_str(), "vec4(%f,%f,%f,%f)", &vector[0], &vector[1], &vector[2], &vector[3]); 
			},
			[](std::string str, void* data, std::size_t index)
			{
				float* matrix = glm::value_ptr(reinterpret_cast<glm::mat4*>(data)[index]);
				sscanf_s(	str.c_str(), "mat4x4((%f,%f,%f,%f), (%f,%f,%f,%f), (%f,%f,%f,%f), (%f,%f,%f,%f))",
							&matrix[0], &matrix[1], &matrix[2], &matrix[3],
							&matrix[4], &matrix[5], &matrix[6], &matrix[7],
							&matrix[8], &matrix[9], &matrix[10], &matrix[11],
							&matrix[12], &matrix[13], &matrix[14], &matrix[15]);
			},
			[](std::string str, void* data, std::size_t index) { reinterpret_cast<std::string*>(data)[index] = str; },
			[](std::string, void*, std::size_t) { }
		};
#pragma endregion Type Definitions and Constants

#pragma region Default Functors
	public:
		/// <summary>
		/// Functor specifying the default strategy for incrementing the capacity of the Vector.
		/// </summary>
		struct DefaultReserveFunctor final
		{
			/// <summary>
			/// Function call operator.
			/// </summary>
			/// <param name="">Placeholder argument for the Datum size.</param>
			/// <param name="capacity">Datum capacity.</param>
			/// <returns>New capacity.</returns>
			constexpr std::size_t operator()(const std::size_t, const std::size_t capacity) const;
		};
#pragma endregion Default Functors
	
#pragma region TypeOf Static Method
	public:
		/// <summary>
		/// Returns the DatumTypes value associated with the template typename T.
		/// </summary>
		/// <typeparam name="T">Type to check for an associated DatumTypes value.</typeparam>
		template<typename T>
		static constexpr DatumTypes TypeOf();

		/// <summary>
		/// Returns the DatumTypes value associated with an int type.
		/// </summary>
		template<>
		static constexpr DatumTypes TypeOf<int>();

		/// <summary>
		/// Returns the DatumTypes value associated with a float type.
		/// </summary>
		template<>
		static constexpr DatumTypes TypeOf<float>();

		/// <summary>
		/// Returns the DatumTypes value associated with a glm::vec4 type.
		/// </summary>
		template<>
		static constexpr DatumTypes TypeOf<glm::vec4>();

		/// <summary>
		/// Returns the DatumTypes value associated with a glm::mat4 type.
		/// </summary>
		template<>
		static constexpr DatumTypes TypeOf<glm::mat4>();

		/// <summary>
		/// Returns the DatumTypes value associated with a std::string type.
		/// </summary>
		template<>
		static constexpr DatumTypes TypeOf<std::string>();

		/// <summary>
		/// Returns the DatumTypes value associated with a RTTIPointer type.
		/// </summary>
		template<>
		static constexpr DatumTypes TypeOf<RTTIPointer>();
#pragma endregion TypeOf Static Method

#pragma region Constructors, Destructor, Assignment
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Datum() = default;

		/// <summary>
		/// Destructor.
		/// </summary>
		~Datum();
		
		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">Datum to be copied.</param>
		Datum(const Datum& rhs);
		
		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">Datum to be copied.</param>
		/// <returns>Reference to the newly modified Datum.</returns>
		Datum& operator=(const Datum& rhs);
		
		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">Datum to be moved</param>
		Datum(Datum&& rhs) noexcept;
		
		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">Datum to be moved.</param>
		/// <returns>Reference to the newly modified Datum.</returns>
		Datum& operator=(Datum&& rhs) noexcept;

#pragma region Constructors
#pragma region Scalar Constructor Overloads
	public:
		/// <summary>
		/// Scalar constructor overloads for assigning Datum to an int value.
		/// </summary>
		/// <param name="rhs">An int value for intializing mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum(const int rhs);

		/// <summary>
		/// Scalar constructor overloads for assigning Datum to a float value.
		/// </summary>
		/// <param name="rhs">A float value for initializing mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum(const float rhs);

		/// <summary>
		/// Scalar constructor overloads for assigning Datum to a glm::vec4 vector.
		/// </summary>
		/// <param name="rhs">A glm::vec4 value for initializing mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum(const glm::vec4& rhs);

		/// <summary>
		/// Scalar constructor overloads for assigning Datum to a glm::mat4 matrix.
		/// </summary>
		/// <param name="rhs">A glm::mat4 value for initializing mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum(const glm::mat4& rhs);

		/// <summary>
		/// Scalar constructor overloads for assigning Datum to a std::string value.
		/// </summary>
		/// <param name="rhs">A std::string value for initializing mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum(const std::string& rhs);
		
		/// <summary>
		/// Scalar constructor overloads for assigning Datum to a RTTI pointer.
		/// </summary>
		/// <param name="rhs">A RTTI pointer for initializing mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum(RTTIPointer const& rhs);
#pragma endregion Scalar Constructor Overloads

#pragma region Initializer List Constructor Overloads
	public:
		/// <summary>
		/// Initializer list constructor overloads for assigning Datum to a list of int values.
		/// </summary>
		/// <param name="rhs">A list of int values for intializing mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum(const std::initializer_list<int> rhs);

		/// <summary>
		/// Initializer list constructor overloads for assigning Datum to a list of float values.
		/// </summary>
		/// <param name="rhs">A list of float values for initializing mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum(const std::initializer_list<float> rhs);

		/// <summary>
		/// Initializer list constructor overloads for assigning Datum to a list of glm::vec4 vectors.
		/// </summary>
		/// <param name="rhs">A list of glm::vec4 values for initializing mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum(const std::initializer_list<glm::vec4> rhs);

		/// <summary>
		/// Initializer list constructor overloads for assigning Datum to a list of glm::mat4 matrices.
		/// </summary>
		/// <param name="rhs">A list of glm::mat4 values for initializing mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum(const std::initializer_list<glm::mat4> rhs);

		/// <summary>
		/// Initializer list constructor overloads for assigning Datum to a list of std::string values.
		/// </summary>
		/// <param name="rhs">A list of std::string values for initializing mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum(const std::initializer_list<std::string> rhs);

		/// <summary>
		/// Initializer list constructor overloads for assigning Datum to a list of RTTI pointers.
		/// </summary>
		/// <param name="rhs">A list of RTTI pointers for initializing mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum(const std::initializer_list<RTTIPointer> rhs);
#pragma endregion Initializer List Constructor Overloads

#pragma endregion Constructors

#pragma region Assignment
#pragma region Scalar Assignment Overloads
	public:
		/// <summary>
		/// Scalar assignment overloads for assigning Datum to an int value.
		/// </summary>
		/// <param name="rhs">An int value for assigning to mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum& operator=(const int rhs);

		/// <summary>
		/// Scalar assignment overloads for assigning Datum to a float value.
		/// </summary>
		/// <param name="rhs">A float value for assigning to mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum& operator=(const float rhs);

		/// <summary>
		/// Scalar assignment overloads for assigning Datum to a glm::vec4 vector.
		/// </summary>
		/// <param name="rhs">A glm::vec4 value for assigning to mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum& operator=(const glm::vec4& rhs);

		/// <summary>
		/// Scalar assignment overloads for assigning Datum to a glm::mat4 matrix.
		/// </summary>
		/// <param name="rhs">A glm::mat4 value for assigning to mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum& operator=(const glm::mat4& rhs);

		/// <summary>
		/// Scalar assignment overloads for assigning Datum to a std:string value.
		/// </summary>
		/// <param name="rhs">A std::string value for assigning to mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum& operator=(const std::string& rhs);

		/// <summary>
		/// Scalar assignment overloads for assigning Datum to a RTTI pointer.
		/// </summary>
		/// <param name="rhs">A RTTI pointer for assigning to mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum& operator=(RTTIPointer const& rhs);
#pragma endregion Scalar Assignment Overloads

#pragma region Initializer List Assignment Overloads
	public:
		/// <summary>
		/// Initializer list assignment overloads for assigning Datum to a list of int values.
		/// </summary>
		/// <param name="rhs">A list of int values for assigning to mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum& operator=(const std::initializer_list<int> rhs);

		/// <summary>
		/// Initializer list assignment overloads for assigning Datum to a list of float values.
		/// </summary>
		/// <param name="rhs">A list of float values for assigning to mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum& operator=(const std::initializer_list<float> rhs);

		/// <summary>
		/// Initializer list assignment overloads for assigning Datum to a list of glm::vec4 vectors.
		/// </summary>
		/// <param name="rhs">A list of glm::vec4 values for assigning to mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum& operator=(const std::initializer_list<glm::vec4> rhs);

		/// <summary>
		/// Initializer list assignment overloads for assigning Datum to a list of glm::mat4 matrices.
		/// </summary>
		/// <param name="rhs">A list of glm::mat4 values for assigning to mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum& operator=(const std::initializer_list<glm::mat4> rhs);

		/// <summary>
		/// Initializer list assignment overloads for assigning Datum to a list of std::string values.
		/// </summary>
		/// <param name="rhs">A list of std::string values for assigning to mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum& operator=(const std::initializer_list<std::string> rhs);

		/// <summary>
		/// Initializer list assignment overloads for assigning Datum to a list of RTTI pointers.
		/// </summary>
		/// <param name="rhs">A list of RTTI pointers for assigning to mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum& operator=(const std::initializer_list<RTTIPointer> rhs);
#pragma endregion Initializer List Assignment Overloads
#pragma endregion Assignment
#pragma endregion Constructors, Destructor, Assignment

#pragma region Boolean Operators
 	public:
		/// <summary>
		/// Equals operator. 
		/// Checks if the type and size of the Datum and the data values are equal to the right hand side Datum.
		/// </summary>
		/// <param name="rhs">Datum on the right hand side to be compared to the left.</param>
		/// <returns>True when lists are equivalent, otherwise false.</returns>
		bool operator==(const Datum& rhs) const noexcept;

		/// <summary>
		/// Not equals operator. 
		/// Checks if the type and size of the Datum and the data values are not equal to the right hand side Datum.
		/// </summary>
		/// <param name="rhs">Datum on the right hand side to be compared to the left.</param>
		/// <returns>True when lists are not equivalent, otherwise false.</returns>
		bool operator!=(const Datum& rhs) const noexcept;

#pragma region Equals Scalar
		/// <summary>
		/// Scalar equals operator. 
		/// Checks if the Datum contains only one element that is equal to the right hand side value.
		/// </summary>
		/// <param name="rhs">Scalar value on the right hand side to be compared to the Datum value.</param>
		/// <returns>True when Datum and scalar are equivalent, otherwise false.</returns>
		bool operator==(const int rhs) const noexcept;

		/// <summary>
		/// Scalar equals operator. 
		/// Checks if the Datum contains only one element that is equal to the right hand side value.
		/// </summary>
		/// <param name="rhs">Scalar value on the right hand side to be compared to the Datum value.</param>
		/// <returns>True when Datum and scalar are equivalent, otherwise false.</returns>
		bool operator==(const float rhs) const noexcept;

		/// <summary>
		/// Scalar equals operator. 
		/// Checks if the Datum contains only one element that is equal to the right hand side value.
		/// </summary>
		/// <param name="rhs">Scalar value on the right hand side to be compared to the Datum value.</param>
		/// <returns>True when Datum and scalar are equivalent, otherwise false.</returns>
		bool operator==(const glm::vec4& rhs) const noexcept;

		/// <summary>
		/// Scalar equals operator. 
		/// Checks if the Datum contains only one element that is equal to the right hand side value.
		/// </summary>
		/// <param name="rhs">Scalar value on the right hand side to be compared to the Datum value.</param>
		/// <returns>True when Datum and scalar are equivalent, otherwise false.</returns>
		bool operator==(const glm::mat4& rhs) const noexcept;

		/// <summary>
		/// Scalar equals operator. 
		/// Checks if the Datum contains only one element that is equal to the right hand side value.
		/// </summary>
		/// <param name="rhs">Scalar value on the right hand side to be compared to the Datum value.</param>
		/// <returns>True when Datum and scalar are equivalent, otherwise false.</returns>
		bool operator==(const std::string& rhs) const noexcept;

		/// <summary>
		/// Scalar equals operator. 
		/// Checks if the Datum contains only one element that is equal to the right hand side value.
		/// </summary>
		/// <param name="rhs">Scalar value on the right hand side to be compared to the Datum value.</param>
		/// <returns>True when Datum and scalar are equivalent, otherwise false.</returns>
		bool operator==(const RTTIPointer& rhs) const noexcept;
#pragma endregion Equals Scalar

#pragma region Not Equals Scalar
		/// <summary>
		/// Scalar not equals operator. 
		/// Checks if the Datum contains only one element that is not equal to the right hand side value.
		/// </summary>
		/// <param name="rhs">Scalar value on the right hand side to be compared to the Datum value.</param>
		/// <returns>True when Datum and scalar are not equivalent, otherwise false.</returns>
		bool operator!=(const int rhs) const noexcept;

		/// <summary>
		/// Scalar not equals operator. 
		/// Checks if the Datum contains only one element that is not equal to the right hand side value.
		/// </summary>
		/// <param name="rhs">Scalar value on the right hand side to be compared to the Datum value.</param>
		/// <returns>True when Datum and scalar are not equivalent, otherwise false.</returns>
		bool operator!=(const float rhs) const noexcept;

		/// <summary>
		/// Scalar not equals operator. 
		/// Checks if the Datum contains only one element that is not equal to the right hand side value.
		/// </summary>
		/// <param name="rhs">Scalar value on the right hand side to be compared to the Datum value.</param>
		/// <returns>True when Datum and scalar are not equivalent, otherwise false.</returns>
		bool operator!=(const glm::vec4& rhs) const noexcept;

		/// <summary>
		/// Scalar not equals operator. 
		/// Checks if the Datum contains only one element that is not equal to the right hand side value.
		/// </summary>
		/// <param name="rhs">Scalar value on the right hand side to be compared to the Datum value.</param>
		/// <returns>True when Datum and scalar are not equivalent, otherwise false.</returns>
		bool operator!=(const glm::mat4& rhs) const noexcept;

		/// <summary>
		/// Scalar not equals operator. 
		/// Checks if the Datum contains only one element that is not equal to the right hand side value.
		/// </summary>
		/// <param name="rhs">Scalar value on the right hand side to be compared to the Datum value.</param>
		/// <returns>True when Datum and scalar are not equivalent, otherwise false.</returns>
		bool operator!=(const std::string& rhs) const noexcept;

		/// <summary>
		/// Scalar not equals operator. 
		/// Checks if the Datum contains only one element that is not equal to the right hand side value.
		/// </summary>
		/// <param name="rhs">Scalar value on the right hand side to be compared to the Datum value.</param>
		/// <returns>True when Datum and scalar are not equivalent, otherwise false.</returns>
		bool operator!=(const RTTIPointer& rhs) const noexcept;
#pragma endregion Not Equals Scalar
#pragma endregion Boolean Operators

#pragma region Type, Size, Capacity
	public:
		/// <summary>
		/// Gets the DatumTypes value.
		/// </summary>
		/// <returns>Current DatumTypes value.</returns>
		DatumTypes Type() const;

		/// <summary>
		/// Sets the DatumTypes value.
		/// </summary>
		/// <param name="type">DatumTypes value to set.</param>
		/// <exception cref="runtime_error">Type cannot be reassigned.</exception>
		/// <exception cref="runtime_error">Cannot assign to unknown type.</exception>
		void SetType(DatumTypes type);

		/// <summary>
		/// Gets the number of elements.
		/// </summary>
		/// <returns>Current number of elements.</returns>
		std::size_t Size() const;

		/// <summary>
		/// Determines if the Datum is empty.
		/// </summary>
		/// <returns>True if the Datum contains no elements, otherwise false.</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Gets the max number of elements for which memory is available. 
		/// </summary>
		/// <returns>Max number of elements for which memory is available.</returns>
		std::size_t Capacity() const;

		/// <summary>
		/// Allocates memory for the number of elements specified.
		/// </summary>
		/// <param name="capacity">Number of elements for for which to allocate memory.</param>
		/// <exception cref="runtime_error">Data type unknown.</exception>
		/// <exception cref="runtime_error">Cannot modify external storage.</exception>
		void Reserve(std::size_t capacity);

		/// <summary>
		/// Inserts or destroys elements so the Datum contains only the specified number of elements.
		/// </summary>
		/// <param name="size">New size for the Datum.</param>
		/// <exception cref="runtime_error">Data type unknown.</exception>
		/// <exception cref="runtime_error">Cannot modify external storage.</exception>
		void Resize(std::size_t size);

		/// <summary>
		/// Shrinks any excess capacity to match the number of elements the Datum contains.
		/// </summary>
		/// <exception cref="runtime_error">Data type unknown.</exception>
		/// <exception cref="runtime_error">Cannot modify external storage.</exception>
		void ShrinkToFit();
#pragma endregion Type, Size, Capacity

#pragma region Element Accessors
		/// <summary>
		/// Gets the first element of the Datum.
		/// </summary>
		/// <returns>Reference to the first element.</returns>
		/// <typeparam name="T">Type of elements in the Datum. A compiler error is thrown on invalid types.</typeparam>
		template<typename T>
		T& Front();

		/// <summary>
		/// Gets the first element of the Datum.
		/// </summary>
		/// <returns>Constant reference to the first element.</returns>
		/// <typeparam name="T">Type of elements in the Datum. A compiler error is thrown on invalid types.</typeparam>
		template<typename T>
		const T& Front() const;

		/// <summary>
		/// Gets the last element of the Datum.
		/// </summary>
		/// <returns>Reference to the last element.</returns>
		/// <typeparam name="T">Type of elements in the Datum. A compiler error is thrown on invalid types.</typeparam>
		template<typename T>
		T& Back();

		/// <summary>
		/// Gets the first element of the Datum.
		/// </summary>
		/// <returns>Constant reference to the first element.</returns>
		/// <typeparam name="T">Type of elements in the Datum. A compiler error is thrown on invalid types.</typeparam>
		template<typename T>
		const T& Back() const;

		/// <summary>
		/// Sets the element of the Datum at a given index, defaults to the first element.
		/// </summary>
		/// <param name="value">Value to set the element.</param>
		/// <param name="index">Index of the element to be set with the value.</param>
		/// <returns>Reference to the set element.</returns>
		/// <typeparam name="T">Type of elements in the Datum. A compiler error is thrown on invalid types.</typeparam>
		template<typename T>
		T& Set(const T& value, std::size_t index=0);

		/// <summary>
		/// Gets the element of the Datum at a given index, defaults to the first element.
		/// </summary>
		/// <param name="index">Index of the element.</param>
		/// <returns>Reference to the element.</returns>
		/// <typeparam name="T">Type of elements in the Datum. A compiler error is thrown on invalid types.</typeparam>
		template<typename T>
		T& Get(std::size_t index=0);

		/// <summary>
		/// Gets the element of the Datum at a given index, defaults to the first element.
		/// </summary>
		/// <param name="index">Index of the element.</param>
		/// <returns>Constant reference to the element.</returns>
		/// <typeparam name="T">Type of elements in the Datum. A compiler error is thrown on invalid types.</typeparam>
		template<typename T>
		const T& Get(std::size_t index=0) const;

		/// <summary>
		/// Finds an element in the Datum given the corresponding value.
		/// </summary>
		/// <param name="value">Value to be searched for in the Datum.</param>
		/// <returns>Pointer to the value if found, otherwise nullptr.</returns>
		/// <typeparam name="T">Type of elements in the Datum. A compiler error is thrown on invalid types.</typeparam>
		template<typename T>
		T* Find(const T& value);

		/// <summary>
		/// Finds an element in the Datum given the corresponding value.
		/// </summary>
		/// <param name="value">Value to be searched for in the Datum.</param>
		/// <returns>Pointer to the value if found, otherwise nullptr.</returns>
		/// <typeparam name="T">Type of elements in the Datum. A compiler error is thrown on invalid types.</typeparam>
		template<typename T>
		const T* Find(const T& value) const;

		/// <summary>
		/// Finds the index of an element in the Datum given the corresponding value.
		/// </summary>
		/// <param name="value">Value to be searched for in the Datum.</param>
		/// <returns>Index of the value if found, otherwise one past the last index, a.k.a. the size.</returns>
		/// <typeparam name="T">Type of elements in the Datum. A compiler error is thrown on invalid types.</typeparam>
		template<typename T>
		std::size_t IndexOf(const T& value) const;

		/// <summary>
		/// Template specialization that finds the index of an RTTIPointer in the Datum given an equal RTTIPointer.
		/// </summary>
		/// <param name="value">Value to be searched for in the Datum.</param>
		/// <returns>Index of the value if found, otherwise one past the last index, a.k.a. the size.</returns>
		/// <typeparam name="T">Type of elements in the Datum. A compiler error is thrown on invalid types.</typeparam>
		template<>
		std::size_t IndexOf(const RTTIPointer& value) const;
#pragma endregion Element Accessors

#pragma region Modifiers
	public:
		/// <summary>
		/// Sets the storage to the external data at the given address with the given size.
		/// </summary>
		/// <param name="data">External data storage to be wrapped by the Datum.</param>
		/// <param name="size">Size of the external data. Must be greater than zero.</param>
		/// <typeparam name="T">Type of elements in the Datum. A compiler error is thrown on invalid types.</typeparam>
		/// <exception cref="runtime_error">External storage size must be greater than zero.</exception>
		template<typename T>
		void SetStorage(gsl::span<T> storage);

		/// <summary>
		/// Inserts data into the end of the Datum, incrementing capacity if needed.
		/// </summary>
		/// <param name="size">New size for the Datum.</param>
		/// <param name="mDataPtr">Pointer alias for mData for the DatumTypes value.</param>
		/// <typeparam name="T">Type of elements in the Datum. A compiler error is thrown on invalid types.</typeparam>
		/// <exception cref="runtime_error">Cannot modify external storage.</exception>
		template<typename T>
		void PushBack(const T& data);

		/// <summary>
		/// Removes the last element of the Datum.
		/// </summary>
		/// <exception cref="runtime_error">Data type unknown.</exception>
		/// <exception cref="runtime_error">Cannot modify external storage.</exception>
		void PopBack();

		/// <summary>
		/// Removes a single element from the Datum given the corresponding value.
		/// </summary>
		/// <param name="value">Value to be searched for in the Datum to be removed.</param>
		/// <returns>True on successful remove, false otherwise.</returns>
		/// <typeparam name="T">Type of elements in the Datum. A compiler error is thrown on invalid types.</typeparam>
		/// <exception cref="runtime_error">Cannot modify external storage.</exception>
		template<typename T>
		bool Remove(const T& value);

		/// <summary>
		/// Removes a single element from the Datum given the corresponding value.
		/// </summary>
		/// <param name="index">Index of the element.</param>
		/// <exception cref="runtime_error">Data type unknown.</exception>
		/// <exception cref="runtime_error">Cannot modify external storage.</exception>
		/// <exception cref="out_of_range">Index out of bounds.</exception>
		void RemoveAt(const std::size_t index);

		/// <summary>
		/// Destroys all elements in the Datum. Sets size to zero, but does not change capacity.
		/// </summary>
		void Clear();

		/// <summary>
		/// Sets the reserve strategy for incrementing the capacity during a PushBack call at full capacity.
		/// </summary>
		/// <param name="reserveFunctor">New reserve strategy functor.</param>
		void SetReserveStrategy(ReserveFunctor reserveFunctor);
#pragma endregion Modifiers

#pragma region String Conversion
	public:
		/// <summary>
		/// Converts a value in the Datum into a string representation.
		/// </summary>
		/// <param name="index">Index of the value to convert to a string. Defaults to the first element.</param>
		/// <returns>String representation of the Datum value.</returns>
		/// <exception cref="runtime_error">Data type unknown.</exception>
		std::string ToString(const std::size_t index=0) const;

		/// <summary>
		/// Sets a value in the Datum from a string representation.
		/// </summary>
		/// <param name="index">Index of the value to set. Defaults to the first element.</param>
		/// <returns>String representation of the Datum value.</returns>
		/// <exception cref="runtime_error">Data type unknown.</exception>
		void SetFromString(const std::string str, const std::size_t index=0);
#pragma endregion String Conversion

#pragma region HelperMethods
	private:
		/// <summary>
		/// Constructor and assignment helper that initializes Datum with values from an initializer list.
		/// </summary>
		/// <param name="rhs">List of values to fill the Datum.</param>
		/// <typeparam name="T">Type of elements to fill the Datum.</typeparam>
		template<typename T>
		Datum& ConstructorAssignmentHelper(const std::initializer_list<T> rhs);

		/// <summary>
		/// Places a new empty element at the given index without allocating memory.
		/// </summary>
		/// <param name="index">Index in which to place the element.</param>
		void ResizeHelper(std::size_t index);
#pragma endregion HelperMethods

#pragma region Data Members
	private:
		/// <summary>
		/// Pointer to the data in the Datum.
		/// </summary>
		DatumValue mData{ nullptr };

		/// <summary>
		/// Enum specifying the type of the data in the Datum.
		/// </summary>
		DatumTypes mType{ DatumTypes::Unknown };

		/// <summary>
		/// Number of elements in the Datum.
		/// </summary>
		std::size_t mSize{ 0 };

		/// <summary>
		/// Max number of elements for which memory is allocated.
		/// </summary>
		std::size_t mCapacity{ 0 };

		/// <summary>
		/// Represents whether the Datum owns the data or if it is owned externally.
		/// </summary>
		bool mInternalStorage{ true };

		/// <summary>
		/// Reserve strategy functor denoting how to increment capacity during insertion.
		/// </summary>
		ReserveFunctor mReserveFunctor{ DefaultReserveFunctor() };
#pragma endregion Data Members
	};
}

#include "Datum.inl"
