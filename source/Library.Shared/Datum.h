#pragma once

#pragma region Includes
// Standard
#include <string>

// Third Party
#include <glm/glm.hpp>

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
			void* voidPtr;
			int* intPtr;
			float* floatPtr;
			glm::vec4* vectorPtr;
			glm::mat4* matrixPtr;
			std::string* stringPtr;
			RTTI** rttiPtr;
		};

		/// <summary>
		/// Templated struct defining the return type pointer for the given type.
		/// </summary>
		template<typename T>
		struct DatumPointer { typedef T& Type; };

		template<>
		struct DatumPointer<int> { typedef int*& Type; };

		template<>
		struct DatumPointer<float> { typedef float*& Type; };

		template<>
		struct DatumPointer<glm::vec4> { typedef glm::vec4*& Type; };

		template<>
		struct DatumPointer<glm::mat4> { typedef glm::mat4*& Type; };

		template<>
		struct DatumPointer<std::string> { typedef std::string*& Type; };

		template<>
		struct DatumPointer<RTTI*> { typedef RTTI**& Type; };

		template<>
		struct DatumPointer<const int> { typedef int* const Type; };

		template<>
		struct DatumPointer<const float> { typedef float* const Type; };

		template<>
		struct DatumPointer<const glm::vec4> { typedef glm::vec4* const Type; };

		template<>
		struct DatumPointer<const glm::mat4> { typedef glm::mat4* const Type; };

		template<>
		struct DatumPointer<const std::string> { typedef std::string* const Type; };

		template<>
		struct DatumPointer<RTTI* const> { typedef RTTI** const Type; };

		/// <summary>
		/// Datum type size look-up table.
		/// </summary>
		static constexpr std::size_t DatumSizeLUT[static_cast<std::size_t>(DatumTypes::End)] = { sizeof(int), sizeof(float),
																								 sizeof(glm::vec4), sizeof(glm::mat4),
																								 sizeof(std::string), sizeof(RTTI*) };
#pragma endregion Type Definitions and Constants

#pragma region Default Functors
	private:
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
		/// Returns the DatumTypes value associated with a RTTI pointer type.
		/// </summary>
		template<>
		static constexpr DatumTypes TypeOf<RTTI*>();
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
		Datum(const int rhs);

		/// <summary>
		/// Scalar constructor overloads for assigning Datum to a float value.
		/// </summary>
		/// <param name="rhs">A float value for initializing mData.</param>
		Datum(const float rhs);

		/// <summary>
		/// Scalar constructor overloads for assigning Datum to a glm::vec4 vector.
		/// </summary>
		/// <param name="rhs">A glm::vec4 value for initializing mData.</param>
		Datum(const glm::vec4& rhs);

		/// <summary>
		/// Scalar constructor overloads for assigning Datum to a glm::mat4 matrix.
		/// </summary>
		/// <param name="rhs">A glm::mat4 value for initializing mData.</param>
		Datum(const glm::mat4& rhs);

		/// <summary>
		/// Scalar constructor overloads for assigning Datum to a std::string value.
		/// </summary>
		/// <param name="rhs">A std::string value for initializing mData.</param>
		Datum(const std::string& rhs);
		
		/// <summary>
		/// Scalar constructor overloads for assigning Datum to a RTTI pointer.
		/// </summary>
		/// <param name="rhs">A RTTI pointer for initializing mData.</param>
		Datum(RTTI* const& rhs);
#pragma endregion Scalar Constructor Overloads

#pragma region Initializer List Constructor Overloads
	public:
		/// <summary>
		/// Initializer list constructor overloads for assigning Datum to a list of int values.
		/// </summary>
		/// <param name="rhs">A list of int values for intializing mData.</param>
		Datum(const std::initializer_list<int> rhs);

		/// <summary>
		/// Initializer list constructor overloads for assigning Datum to a list of float values.
		/// </summary>
		/// <param name="rhs">A list of float values for initializing mData.</param>
		Datum(const std::initializer_list<float> rhs);

		/// <summary>
		/// Initializer list constructor overloads for assigning Datum to a list of glm::vec4 vectors.
		/// </summary>
		/// <param name="rhs">A list of glm::vec4 values for initializing mData.</param>
		Datum(const std::initializer_list<glm::vec4> rhs);

		/// <summary>
		/// Initializer list constructor overloads for assigning Datum to a list of glm::mat4 matrices.
		/// </summary>
		/// <param name="rhs">A list of glm::mat4 values for initializing mData.</param>
		Datum(const std::initializer_list<glm::mat4> rhs);

		/// <summary>
		/// Initializer list constructor overloads for assigning Datum to a list of std::string values.
		/// </summary>
		/// <param name="rhs">A list of std::string values for initializing mData.</param>
		Datum(const std::initializer_list<std::string> rhs);

		/// <summary>
		/// Initializer list constructor overloads for assigning Datum to a list of RTTI pointers.
		/// </summary>
		/// <param name="rhs">A list of RTTI pointers for initializing mData.</param>
		Datum(const std::initializer_list<RTTI*> rhs);
#pragma endregion Initializer List Constructor Overloads

#pragma endregion Constructors

#pragma region Assignment
#pragma region Scalar Assignment Overloads
	public:
		/// <summary>
		/// Scalar assignment overloads for assigning Datum to an int value.
		/// </summary>
		/// <param name="rhs">An int value for assigning to mData.</param>
		Datum& operator=(const int rhs);

		/// <summary>
		/// Scalar assignment overloads for assigning Datum to a float value.
		/// </summary>
		/// <param name="rhs">A float value for assigning to mData.</param>
		Datum& operator=(const float rhs);

		/// <summary>
		/// Scalar assignment overloads for assigning Datum to a glm::vec4 vector.
		/// </summary>
		/// <param name="rhs">A glm::vec4 value for assigning to mData.</param>
		Datum& operator=(const glm::vec4& rhs);

		/// <summary>
		/// Scalar assignment overloads for assigning Datum to a glm::mat4 matrix.
		/// </summary>
		/// <param name="rhs">A glm::mat4 value for assigning to mData.</param>
		Datum& operator=(const glm::mat4& rhs);

		/// <summary>
		/// Scalar assignment overloads for assigning Datum to a std:string value.
		/// </summary>
		/// <param name="rhs">A std::string value for assigning to mData.</param>
		Datum& operator=(const std::string& rhs);

		/// <summary>
		/// Scalar assignment overloads for assigning Datum to a RTTI pointer.
		/// </summary>
		/// <param name="rhs">A RTTI pointer for assigning to mData.</param>
		Datum& operator=(RTTI* const& rhs);
#pragma endregion Scalar Assignment Overloads

#pragma region Initializer List Assignment Overloads
	public:
		/// <summary>
		/// Initializer list assignment overloads for assigning Datum to a list of int values.
		/// </summary>
		/// <param name="rhs">A list of int values for assigning to mData.</param>
		Datum& operator=(const std::initializer_list<int> rhs);

		/// <summary>
		/// Initializer list assignment overloads for assigning Datum to a list of float values.
		/// </summary>
		/// <param name="rhs">A list of float values for assigning to mData.</param>
		Datum& operator=(const std::initializer_list<float> rhs);

		/// <summary>
		/// Initializer list assignment overloads for assigning Datum to a list of glm::vec4 vectors.
		/// </summary>
		/// <param name="rhs">A list of glm::vec4 values for assigning to mData.</param>
		Datum& operator=(const std::initializer_list<glm::vec4> rhs);

		/// <summary>
		/// Initializer list assignment overloads for assigning Datum to a list of glm::mat4 matrices.
		/// </summary>
		/// <param name="rhs">A list of glm::mat4 values for assigning to mData.</param>
		Datum& operator=(const std::initializer_list<glm::mat4> rhs);

		/// <summary>
		/// Initializer list assignment overloads for assigning Datum to a list of std::string values.
		/// </summary>
		/// <param name="rhs">A list of std::string values for assigning to mData.</param>
		Datum& operator=(const std::initializer_list<std::string> rhs);

		/// <summary>
		/// Initializer list assignment overloads for assigning Datum to a list of RTTI pointers.
		/// </summary>
		/// <param name="rhs">A list of RTTI pointers for assigning to mData.</param>
		Datum& operator=(const std::initializer_list<RTTI*> rhs);
#pragma endregion Initializer List Assignment Overloads
#pragma endregion Assignment
#pragma endregion Constructors, Destructor, Assignment

#pragma region Pointer Accessors
	private:
		template<typename T>
		constexpr typename DatumPointer<T>::Type GetPointer();

		template<>
		constexpr typename DatumPointer<int>::Type GetPointer<int>();

		template<>
		constexpr typename DatumPointer<float>::Type GetPointer<float>();

		template<>
		constexpr typename DatumPointer<glm::vec4>::Type GetPointer<glm::vec4>();

		template<>
		constexpr typename DatumPointer<glm::mat4>::Type GetPointer<glm::mat4>();

		template<>
		constexpr typename DatumPointer<std::string>::Type GetPointer<std::string>();

		template<>
		constexpr typename DatumPointer<RTTI*>::Type GetPointer<RTTI*>();

		template<typename T>
		constexpr typename DatumPointer<const T>::Type GetPointer() const;
							  
		template<>			  
		constexpr typename DatumPointer<const int>::Type GetPointer<int>() const;
							  
		template<>			  
		constexpr typename DatumPointer<const float>::Type GetPointer<float>() const;
							  
		template<>			  
		constexpr typename DatumPointer<const glm::vec4>::Type GetPointer<glm::vec4>() const;
							  
		template<>			  
		constexpr typename DatumPointer<const glm::mat4>::Type GetPointer<glm::mat4>() const;
							  
		template<>			  
		constexpr typename DatumPointer<const std::string>::Type GetPointer<std::string>() const;
							  
		template<>			  
		constexpr typename DatumPointer<RTTI* const>::Type GetPointer<RTTI*>() const;
#pragma endregion Pointer Accessors

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
		bool operator==(const RTTI*& rhs) const noexcept;
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
		bool operator!=(const RTTI*& rhs) const noexcept;
#pragma endregion Not Equals Scalar
#pragma endregion Boolean Operators

#pragma region Type, Size, Capacity
	public:
		DatumTypes Type() const;
		void SetType(DatumTypes type);
		std::size_t Size() const;
		bool IsEmpty() const;
		std::size_t Capacity() const;
		void Reserve(std::size_t capacity);
		void Resize(std::size_t capacity);
		void ShrinkToFit();
#pragma endregion Type, Size Capacity

#pragma region Modifiers
	public:
#pragma region Push Back Overloads
		void PushBack(const int data);
		void PushBack(const float data);
		void PushBack(const glm::vec4& data);
		void PushBack(const glm::mat4& data);
		void PushBack(const std::string& data);
		void PushBack(RTTI* const& data);
#pragma endregion Push Back Overloads

		/// <summary>
		/// Destroys all elements in the Datum. Sets size to zero, but does not change capacity.
		/// </summary>
		void Clear();
#pragma endregion Modifiers

#pragma region HelperMethods
	private:
		/// <summary>
		/// Constructor helper that initializes Datum with values passed from scalar and initializer list constructor overloads.
		/// </summary>
		/// <param name="rhs">List of scalar values to fill the Datum.</param>
		/// <param name="mDataPtr">The corresponding pointer from mData for the current type.</param>
		/// <typeparam name="T">Type of elements to fill the Datum.</typeparam>
		template<typename T>
		void ConstructorHelper(const std::initializer_list<T> rhs);

		/// <summary>
		/// Scalar assignment helper that initializes Datum with scalar values passed from scalar assignment overloads.
		/// </summary>
		/// <param name="rhs">List of scalar values to fill the Datum.</param>
		/// <param name="mDataPtr">The corresponding pointer from mData for the current type.</param>
		/// <typeparam name="T">Type of elements to fill the Datum.</typeparam>
		template<typename T>
		Datum& AssignmentHelper(const std::initializer_list<T> rhs);

		template<typename T>
		bool EqualsScalarHelper(const T& rhs) const;

		template<>
		bool EqualsScalarHelper<RTTI>(const RTTI& rhs) const;

		/// <summary>
		/// Resize helper that inserts or destroys data to resize the Datum to contain the given size.
		/// </summary>
		/// <param name="size">New size for the Datum.</param>
		/// <param name="mDataPtr">Pointer alias for mData for the DatumTypes value.</param>
		/// <typeparam name="T">Type of elements in the Datum.</typeparam>
		template<typename T>
		void ResizeHelper(std::size_t size, T*& mDataPtr);

		/// <summary>
		/// PushBack helper that inserts data into the end of the Datum, incrementing capacity if needed.
		/// </summary>
		/// <param name="size">New size for the Datum.</param>
		/// <param name="type">DatumTypes value of the data to insert.</param>
		/// <typeparam name="T">Type of elements in the Datum.</typeparam>
		template<typename T>
		void PushBackHelper(const T& data, DatumTypes type);
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
