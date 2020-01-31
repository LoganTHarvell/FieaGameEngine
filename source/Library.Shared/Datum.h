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

		static constexpr std::size_t DatumTypeSizes[static_cast<std::size_t>(DatumTypes::End)] = { sizeof(int), sizeof(float),
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
			/// <param name="">Placeholder argument for the Vector size.</param>
			/// <param name="capacity">Vector capacity.</param>
			/// <returns>New capacity.</returns>
			std::size_t operator()(const std::size_t, const std::size_t capacity) const;
		};
#pragma endregion Default Functors

#pragma region Constructors, Destructor, Assignment
	public:
		explicit Datum(ReserveFunctor reserveFunctor=DefaultReserveFunctor());
		~Datum();
		Datum(const Datum& rhs);
		Datum& operator=(const Datum& rhs);
		Datum(Datum&& rhs) noexcept;
		Datum& operator=(Datum&& rhs);

#pragma region Scalar Assignment
		Datum& operator=(const int rhs);
		Datum& operator=(const float rhs);
		Datum& operator=(const glm::vec4& rhs);
		Datum& operator=(const glm::mat4& rhs);
		Datum& operator=(const std::string& rhs);
		Datum& operator=(const RTTI*& rhs);
#pragma endregion Scalar Assignment

#pragma region Initializer List Constructors
		Datum(const std::initializer_list<int> rhs);
		Datum(const std::initializer_list<float> rhs);
		Datum(const std::initializer_list<glm::vec4> rhs);
		Datum(const std::initializer_list<glm::mat4> rhs);
		Datum(const std::initializer_list<std::string> rhs);
		Datum(const std::initializer_list<RTTI*> rhs);
#pragma endregion Initializer List Constructors

#pragma region Initializer List Assignment
		Datum& operator=(const std::initializer_list<int> rhs);
		Datum& operator=(const std::initializer_list<float> rhs);
		Datum& operator=(const std::initializer_list<glm::vec4> rhs);
		Datum& operator=(const std::initializer_list<glm::mat4> rhs);
		Datum& operator=(const std::initializer_list<std::string> rhs);
		Datum& operator=(const std::initializer_list<RTTI*> rhs);
#pragma endregion Initializer List Assignment
#pragma endregion Constructors, Destructor, Assignment

#pragma region Boolean Operators
		bool operator==(const Datum& rhs);
		bool operator!=(const Datum& rhs);

		bool operator==(const int rhs);
		bool operator==(const float rhs);
		bool operator==(const glm::vec4& rhs);
		bool operator==(const glm::mat4& rhs);
		bool operator==(const std::string& rhs);
		bool operator==(const RTTI*& rhs);

		bool operator!=(const int rhs);
		bool operator!=(const float rhs);
		bool operator!=(const glm::vec4& rhs);
		bool operator!=(const glm::mat4& rhs);
		bool operator!=(const std::string& rhs);
		bool operator!=(const RTTI*& rhs);
#pragma endregion Boolean Operators

		void Reserve(std::size_t capacity);

#pragma region Modifiers
	private:
		template<typename T>
		void PushBack(const T& data, T* mDataPtr);

	public:
		void PushBack(const int data);
		void PushBack(const float data);
		void PushBack(const glm::vec4& data);
		void PushBack(const glm::mat4& data);
		void PushBack(const std::string& data);
		void PushBack(const RTTI* const& data);

	private:
		template<typename T>
		void Clear(const T* mDataPtr);

	public:
		void Clear();
#pragma endregion Modifiers

	private:
		DatumValue mData{ nullptr };

		DatumTypes mType{ DatumTypes::Unknown };

		std::size_t mSize{ 0 };

		std::size_t mCapacity{ 0 };

		bool mInternalStorage{ true };

		ReserveFunctor mReserveFunctor;
	};
}

