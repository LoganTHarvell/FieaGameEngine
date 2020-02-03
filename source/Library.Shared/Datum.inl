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
	inline constexpr Datum::DatumTypes Datum::TypeOf<Datum::RTTIPointer>()
	{
		return DatumTypes::Pointer;
	}
#pragma endregion TypeOf Static Method

#pragma region Size and Capacity
	inline Datum::DatumTypes Datum::Type() const
	{
		return mType;
	}

	inline void Datum::SetType(DatumTypes type)
	{
		if (mType != DatumTypes::Unknown)	throw std::runtime_error("Type reassignment.");
		if (type == DatumTypes::Unknown)	throw std::runtime_error("Unknown assignment.");

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

#pragma region Element Accessors
	template<typename T>
	inline T& Datum::Front()
	{
		return Get<T>();
	}

	template<typename T>
	inline const T& Datum::Front() const
	{
		return Get<T>();
	}

	template<typename T>
	inline T& Datum::Back()
	{
		return Get<T>(mSize-1);
	}

	template<typename T>
	inline const T& Datum::Back() const
	{
		return Get<T>(mSize-1);
	}

	template<typename T>
	inline T& Datum::Set(const T& value, std::size_t index)
	{
		static_assert(TypeOf<T>() != DatumTypes::Unknown, "Invalid data type.");

		if (mType == DatumTypes::Unknown)	throw std::runtime_error("Type not set.");
		if (mType != TypeOf<T>())			throw std::runtime_error("Mismatched type.");
		if (index >= mSize)					throw std::out_of_range("Index out of bounds.");
		
		return Get<T>(index) = value;
	}

	template<typename T>
	inline T& Datum::Get(std::size_t index)
	{
		static_assert(TypeOf<T>() != DatumTypes::Unknown, "Invalid data type.");

		if (mType == DatumTypes::Unknown)	throw std::runtime_error("Type not set.");
		if (mType != TypeOf<T>())			throw std::runtime_error("Mismatched type.");
		if (index >= mSize)					throw std::out_of_range("Index out of bounds.");

		return reinterpret_cast<T*>(mData.voidPtr)[index];
	}

	template<typename T>
	inline const T& Datum::Get(std::size_t index) const
	{
		static_assert(TypeOf<T>() != DatumTypes::Unknown, "Invalid data type.");

		if (mType == DatumTypes::Unknown)	throw std::runtime_error("Type not set.");
		if (mType != TypeOf<T>())			throw std::runtime_error("Mismatched type.");
		if (index >= mSize)					throw std::out_of_range("Index out of bounds.");

		return reinterpret_cast<T*>(mData.voidPtr)[index];
	}

	template<typename T>
	inline T* Datum::Find(const T& value)
	{
		static_assert(TypeOf<T>() != DatumTypes::Unknown, "Invalid data type.");

		const std::size_t index = IndexOf(value);
		return index < mSize ? &Get<T>(index) : nullptr;
	}

	template<typename T>
	inline const T* Datum::Find(const T& value) const
	{
		static_assert(TypeOf<T>() != DatumTypes::Unknown, "Invalid data type.");

		const std::size_t index = IndexOf(value);
		return index < mSize ? &Get<T>(index) : nullptr;
	}

	template<typename T>
	inline std::size_t Datum::IndexOf(const T& value) const
	{
		static_assert(TypeOf<T>() != DatumTypes::Unknown, "Invalid data type.");

		if (mType == DatumTypes::Unknown)	throw std::runtime_error("Type not set.");
		if (mType != TypeOf<T>())			throw std::runtime_error("Mismatched type.");

		T* const data = reinterpret_cast<T*>(mData.voidPtr);

		std::size_t i = 0;
		for (; i < mSize; ++i)
		{
			if (data[i] == value)
			{
				break;
			}
		}

		return i;
	}

	template<>
	inline std::size_t Datum::IndexOf(const RTTIPointer& value) const
	{
		if (mType == DatumTypes::Unknown)	throw std::runtime_error("Type not set.");
		if (mType != DatumTypes::Pointer)	throw std::runtime_error("Mismatched type.");

		std::size_t i = 0;
		for (; i < mSize; ++i)
		{
			if ((!mData.rttiPtr[0] && !value) || (mData.rttiPtr[0] && mData.rttiPtr[i]->Equals(value)))
			{
				break;
			}
		}

		return i;
	}
#pragma endregion Element Accessors

#pragma region Modifiers
	template<typename T>
	inline void Datum::SetStorage(T*& data, std::size_t size)
	{
		static_assert(TypeOf<T>() != DatumTypes::Unknown, "Invalid data type.");
		
		if (mType == DatumTypes::Unknown)
		{
			mType = TypeOf<T>();
		}
		else if (mType != TypeOf<T>())
		{
			throw std::runtime_error("Mismatched types.");
		}

		Clear();

		mData.voidPtr = data;
		mSize = size;
		mCapacity = size;
		mInternalStorage = false;
	}

	template<typename T>
	inline void Datum::PushBack(const T& data)
	{
		static_assert(TypeOf<T>() != DatumTypes::Unknown, "Invalid data type.");
		
		if (!mInternalStorage) throw std::runtime_error("External storage.");

		if (mType == DatumTypes::Unknown)
		{
			mType = TypeOf<T>();
		}
		else if (mType != TypeOf<T>())
		{
			throw std::runtime_error("Mismatched type.");
		}

		if (mCapacity <= mSize)
		{
			const std::size_t newCapacity = mReserveFunctor(mCapacity, mSize);
			Reserve(std::max(newCapacity, mCapacity + 1));
		}

		new(reinterpret_cast<T*>(mData.voidPtr) + mSize++)T(data);
	}

	template<typename T>
	inline bool Datum::Remove(const T& value)
	{
		static_assert(TypeOf<T>() != DatumTypes::Unknown, "Invalid data type.");

		if (!mInternalStorage) throw std::runtime_error("External storage.");

		T* const data = reinterpret_cast<T*>(mData.voidPtr);
		const std::size_t index = IndexOf(value);
		
		if (index < mSize)
		{
			if (mType == DatumTypes::String)
			{
				mData.stringPtr[index].~basic_string();
			}

			memmove(&data[index], &data[index + 1], sizeof(T) * (mSize - index));

			--mSize;
			return true;
		}

		return false;
	}

	inline void Datum::SetReserveStrategy(ReserveFunctor reserveFunctor)
	{
		mReserveFunctor = reserveFunctor;
	}
#pragma endregion Modifiers
}