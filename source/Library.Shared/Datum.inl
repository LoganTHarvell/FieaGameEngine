#pragma once

#include "Datum.h"

namespace Library
{
#pragma region Default Functors
	inline constexpr std::size_t Datum::DefaultReserveFunctor::operator()(const std::size_t, const std::size_t capacity) const
	{
		return static_cast<std::size_t>(capacity * 1.5);
	}
#pragma endregion Default Functors

#pragma region TypeOf Static Method
	template<typename T>
	inline constexpr Datum::DatumTypes Datum::TypeOf()
	{
		return DatumTypes::Unknown;
	}

	template<>
	inline constexpr Datum::DatumTypes Datum::TypeOf<int>()
	{
		return DatumTypes::Integer;
	}

	template<>
	inline constexpr Datum::DatumTypes Datum::TypeOf<float>()
	{
		return DatumTypes::Float;
	}

	template<>
	inline constexpr Datum::DatumTypes Datum::TypeOf<glm::vec4>()
	{
		return DatumTypes::Vector;
	}

	template<>
	inline constexpr Datum::DatumTypes Datum::TypeOf<glm::mat4>()
	{
		return DatumTypes::Matrix;
	}

	template<>
	inline constexpr Datum::DatumTypes Datum::TypeOf<std::string>()
	{
		return DatumTypes::String;
	}

	template<>
	inline constexpr Datum::DatumTypes Datum::TypeOf<RTTI*>()
	{
		return DatumTypes::Pointer;
	}
#pragma endregion TypeOf Static Method

#pragma region Pointer Accessors
	template<>
	inline constexpr Datum::DatumPointer<int>::Type Datum::GetPointer<int>()
	{
		return mData.intPtr;
	}

	template<>
	inline constexpr Datum::DatumPointer<float>::Type Datum::GetPointer<float>()
	{
		return mData.floatPtr;
	}

	template<>
	inline constexpr Datum::DatumPointer<glm::vec4>::Type Datum::GetPointer<glm::vec4>()
	{
		return mData.vectorPtr;
	}

	template<>
	inline constexpr Datum::DatumPointer<glm::mat4>::Type Datum::GetPointer<glm::mat4>()
	{
		return mData.matrixPtr;
	}

	template<>
	inline constexpr Datum::DatumPointer<std::string>::Type Datum::GetPointer<std::string>()
	{
		return mData.stringPtr;
	}

	template<>
	inline constexpr Datum::DatumPointer<RTTI*>::Type Datum::GetPointer<RTTI*>()
	{
		return mData.rttiPtr;
	}

	template<>
	inline constexpr typename Datum::DatumPointer<const int>::Type Datum::GetPointer<int>() const
	{
		return mData.intPtr;
	}

	template<>
	inline constexpr typename Datum::DatumPointer<const float>::Type Datum::GetPointer<float>() const
	{
		return mData.floatPtr;
	}

	template<>
	inline constexpr typename Datum::DatumPointer<const glm::vec4>::Type Datum::GetPointer<glm::vec4>() const
	{
		return mData.vectorPtr;
	}

	template<>
	inline constexpr typename Datum::DatumPointer<const glm::mat4>::Type Datum::GetPointer<glm::mat4>() const
	{
		return mData.matrixPtr;
	}

	template<>
	inline constexpr typename Datum::DatumPointer<const std::string>::Type Datum::GetPointer<std::string>() const
	{
		return mData.stringPtr;
	}

	template<>
	inline constexpr typename Datum::DatumPointer<RTTI* const>::Type Datum::GetPointer<RTTI*>() const
	{
		return mData.rttiPtr;
	}
#pragma endregion Pointer Accessors

#pragma region Size and Capacity
	inline Datum::DatumTypes Datum::Type() const
	{
		return mType;
	}

	inline void Datum::SetType(DatumTypes type)
	{
		if (mType != DatumTypes::Unknown) throw std::runtime_error("Type reassignment.");

		mType = type;
	}

	inline std::size_t Datum::Size() const
	{
		return mSize;
	}

	inline bool Datum::IsEmpty() const
	{
		return mSize == 0;
	}

	inline std::size_t Datum::Capacity() const
	{
		return mCapacity;
	}
#pragma endregion Size and Capacity
}