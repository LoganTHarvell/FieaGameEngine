#pragma once

#pragma region Includes
// Standard
#include <string>
#include <type_traits>

// Third Party
#include <gsl/span>
#include <glm/glm.hpp>

#pragma warning(disable : 4201)
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#pragma warning(default : 4201)

// First Party
#include "RTTI.h"
#pragma endregion Includes

namespace Library
{
	class Scope;

	/// <summary>
	/// Runtime type polymorphic wrapper for data elements.
	/// </summary>
	class Datum final
	{
		friend class Attributed;

#pragma region Type Definitions and Constants
	public:
		/// <summary>
		/// Alias for a pointer to a Scope class instance.
		/// </summary>
		using ScopePointer = Scope*;

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
		enum class Types
		{
			Unknown = -1,
			
			Integer,
			Float,
			Vector,
			Matrix,
			String,
			Scope,
			Pointer,
			
			End
		};

	private:
		/// <summary>
		/// Wrapper for a pointer to data of any type that datum can contain.
		/// </summary>
		union Values
		{
			int* intPtr;
			float* floatPtr;
			glm::vec4* vectorPtr;
			glm::mat4* matrixPtr;
			std::string* stringPtr;
			ScopePointer* scopePtr;
			RTTIPointer* rttiPtr;

			// Convenience Pointers
			void* voidPtr;
			std::byte* bytePtr;
		};

		/// <summary>
		/// Datum type size look-up table.
		/// </summary>
		static constexpr std::size_t TypeSizeLUT[static_cast<std::size_t>(Types::End)] = 
		{ 
			sizeof(int), sizeof(float),
			sizeof(glm::vec4), sizeof(glm::mat4),
			sizeof(std::string), sizeof(ScopePointer), 
			sizeof(RTTIPointer) 
		};

		/// <summary>
		/// Datum equality function look-up table.
		/// </summary>
		using EqualityFunctor = std::function<bool(void*, void*, std::size_t)>;
		static const EqualityFunctor EqualityLUT[static_cast<std::size_t>(Types::End)];
		
		/// <summary>
		/// Datum single value equality look-up table.
		/// </summary>
		using ElementEqualityFunctor = std::function<bool(void*, void*)>;
		static const ElementEqualityFunctor ElementEqualityLUT[static_cast<std::size_t>(Types::End)];

		/// <summary>
		/// Create default value function look-up table.
		/// </summary>
		using CreateDefaultFunctor = std::function<void(void*, std::size_t)>;
		static const CreateDefaultFunctor CreateDefaultLUT[static_cast<std::size_t>(Types::End)];

		/// <summary>
		/// ToString function look-up table.
		/// </summary>
		using ToStringFunctor = std::function<std::string(void*, std::size_t)>;
		static const ToStringFunctor ToStringLUT[static_cast<std::size_t>(Types::End)];

		/// <summary>
		/// FromString function look-up table.
		/// </summary>
		using FromStringFunctor = std::function<void(std::string, void*, std::size_t)>;
		static const FromStringFunctor FromStringLUT[static_cast<std::size_t>(Types::End)];
#pragma endregion Type Definitions and Constants

#pragma region Default Functors
	public:
		/// <summary>
		/// Functor specifying the default strategy for incrementing the capacity of the Datum.
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
		static constexpr Types TypeOf();

		/// <summary>
		/// Returns the DatumTypes value associated with an int type.
		/// </summary>
		template<>
		static constexpr Types TypeOf<int>();

		/// <summary>
		/// Returns the DatumTypes value associated with a float type.
		/// </summary>
		template<>
		static constexpr Types TypeOf<float>();

		/// <summary>
		/// Returns the DatumTypes value associated with a glm::vec4 type.
		/// </summary>
		template<>
		static constexpr Types TypeOf<glm::vec4>();

		/// <summary>
		/// Returns the DatumTypes value associated with a glm::mat4 type.
		/// </summary>
		template<>
		static constexpr Types TypeOf<glm::mat4>();

		/// <summary>
		/// Returns the DatumTypes value associated with a std::string type.
		/// </summary>
		template<>
		static constexpr Types TypeOf<std::string>();

		/// <summary>
		/// Returns the DatumTypes value associated with a ScopePointer type.
		/// </summary>
		template<>
		static constexpr Types TypeOf<ScopePointer>();

		/// <summary>
		/// Returns the DatumTypes value associated with a RTTIPointer type.
		/// </summary>
		template<>
		static constexpr Types TypeOf<RTTIPointer>();
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

#pragma region Scalar/Initializer List Constructors
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
		/// Scalar constructor overloads for assigning Datum to a ScopePointer value.
		/// </summary>
		/// <param name="rhs">A ScopePointer value for initializing mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum(const ScopePointer& rhs);
		
		/// <summary>
		/// Scalar constructor overloads for assigning Datum to an RTTIPointer.
		/// </summary>
		/// <param name="rhs">An RTTIPointer for initializing mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum(const RTTIPointer& rhs);

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
		/// Initializer list constructor overloads for assigning Datum to a list of ScopePointer values.
		/// </summary>
		/// <param name="rhs">A list of ScopePointer values for initializing mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum(const std::initializer_list<ScopePointer> rhs);

		/// <summary>
		/// Initializer list constructor overloads for assigning Datum to a list of RTTIPointer values.
		/// </summary>
		/// <param name="rhs">A list of RTTIPointer values for initializing mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum(const std::initializer_list<RTTIPointer> rhs);
#pragma endregion Scalar/List Constructors

#pragma region Scalar/Initializer List Assignment
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
		/// Scalar assignment overloads for assigning Datum to a ScopePointer.
		/// </summary>
		/// <param name="rhs">A ScopePointer for assigning to mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum& operator=(const ScopePointer& rhs);

		/// <summary>
		/// Scalar assignment overloads for assigning Datum to an RTTIPointer.
		/// </summary>
		/// <param name="rhs">An RTTIPointer for assigning to mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum& operator=(const RTTIPointer& rhs);

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
		/// Initializer list assignment overloads for assigning Datum to a list of ScopePointer values.
		/// </summary>
		/// <param name="rhs">A list of ScopePointer values for assigning to mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum& operator=(const std::initializer_list<ScopePointer> rhs);

		/// <summary>
		/// Initializer list assignment overloads for assigning Datum to a list of RTTIPointer values.
		/// </summary>
		/// <param name="rhs">A list of RTTIPointer values for assigning to mData.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		/// <exception cref="runtime_error">External storage has insufficient memory.</exception>
		Datum& operator=(const std::initializer_list<RTTIPointer> rhs);
#pragma endregion Scalar/Initializer List Assignment
#pragma endregion Constructors, Destructor, Assignment

#pragma region Boolean Operators
 	public:
		/// <summary>
		/// Equals operator. 
		/// Checks if the type and size of the Datum and the data values are equal to the right hand side Datum.
		/// </summary>
		/// <param name="rhs">Datum on the right hand side to be compared to the left.</param>
		/// <returns>True when Datum values are equivalent, otherwise false.</returns>
		bool operator==(const Datum& rhs) const noexcept;

		/// <summary>
		/// Not equals operator. 
		/// Checks if the type and size of the Datum and the data values are not equal to the right hand side Datum.
		/// </summary>
		/// <param name="rhs">Datum on the right hand side to be compared to the left.</param>
		/// <returns>True when Datum values are not equivalent, otherwise false.</returns>
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
		bool operator==(const ScopePointer& rhs) const noexcept;

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
		bool operator!=(const ScopePointer& rhs) const noexcept;

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
		Types Type() const;

		/// <summary>
		/// Sets the DatumTypes value.
		/// </summary>
		/// <param name="type">DatumTypes value to set.</param>
		/// <exception cref="runtime_error">Type cannot be reassigned.</exception>
		/// <exception cref="runtime_error">Cannot assign to unknown type.</exception>
		void SetType(Types type);

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
		/// Gets the max number of elements for which memory is already allocated. 
		/// </summary>
		/// <returns>Max number of elements for which memory is already allocated.</returns>
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
		/// Scope subscript dereference operator.
		/// Gets a reference to the Scope located at the given index.
		/// </summary>
		/// <param name="index">Index in the Datum of the ScopePointer to be dereferenced.</param>
		/// <returns>Reference to the scope pointed at by a ScopePointer at the given index in the Datum.</returns>
		Scope& operator[](std::size_t index);

		/// <summary>
		/// Scope subscript dereference operator.
		/// Gets a reference to the Scope located at the given index.
		/// </summary>
		/// <param name="index">Index in the Datum of the ScopePointer to be dereferenced.</param>
		/// <returns>Reference to the scope pointed at by a ScopePointer at the given index in the Datum.</returns>
		const Scope& operator[](std::size_t index) const;
#pragma endregion Element Accessors

#pragma region Modifiers
	public:
		/// <summary>
		/// Sets the storage to the external data at the given address with the given size.
		/// </summary>
		/// <param name="storage">External data storage to be wrapped by the Datum.</param>
		/// <param name="size">Size of the external data. Must be greater than zero.</param>
		/// <typeparam name="T">Type of elements in the Datum. A compiler error is thrown on invalid types.</typeparam>
		/// <exception cref="runtime_error">External storage size must be greater than zero.</exception>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		template<typename T>
		void SetStorage(const gsl::span<T> storage);

	private:
		/// <summary>
		/// Sets the storage to the external data at the given address with the given size.
		/// </summary>
		/// <param name="storage">External data storage to be wrapped by the Datum, as a byte pointer.</param>
		/// <param name="size">Size of the external data. Must be greater than zero.</param>
		/// <exception cref="runtime_error">Mismatched types.</exception>
		void SetStorage(const Types type, const gsl::span<std::byte> storage);

	public:
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
		void SetFromString(const std::string& str, const std::size_t index=0);
#pragma endregion String Conversion

#pragma region Helper Methods
	private:
		/// <summary>
		/// Constructor and assignment helper that initializes Datum with values from an initializer list.
		/// </summary>
		/// <param name="rhs">List of values to fill the Datum.</param>
		/// <typeparam name="T">Type of elements to fill the Datum.</typeparam>
		template<typename T>
		Datum& ConstructorAssignmentHelper(const std::initializer_list<T> rhs);
#pragma endregion Helper Methods

#pragma region Data Members
	private:
		/// <summary>
		/// Pointer to the data in the Datum.
		/// </summary>
		Values mData{ nullptr };

		/// <summary>
		/// Enum specifying the type of the data in the Datum.
		/// </summary>
		Types mType{ Types::Unknown };

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
