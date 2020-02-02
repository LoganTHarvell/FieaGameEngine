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

#pragma region Boolean Operators
	template<typename T>
	inline bool Datum::operator==(const T& rhs) const
	{
		if (mType == DatumTypes::Unknown)		throw std::runtime_error("Type not set.");
		if (TypeOf<T>() == DatumTypes::Unknown) throw std::runtime_error("Unknown data type.");
		if (TypeOf<T>() != mType)				throw std::runtime_error("Incorrect type.");

		if (mSize != 1) return false;

		T* const data = reinterpret_cast<T*>(mData.voidPtr);
		bool isEqual = (data[0] == rhs);
		return isEqual;
	}

	template<>
	inline bool Datum::operator==<Datum>(const Datum& rhs) const
	{
		if (this == &rhs)
		{
			return true;
		}

		if (mType != rhs.mType || mSize != rhs.mSize)
		{
			return false;
		}

		switch (mType)
		{
		case DatumTypes::Unknown:
			return true;
		case DatumTypes::Integer:
		case DatumTypes::Float:
		case DatumTypes::Vector:
		case DatumTypes::Matrix:
		{
			const std::size_t size = mSize * DatumSizeLUT[static_cast<std::size_t>(mType)];
			return memcmp(mData.voidPtr, rhs.mData.voidPtr, size) == 0;
		}
		case DatumTypes::String:
		{
			for (std::size_t i = 0; i < mSize; ++i)
			{
				if (mData.stringPtr[i] != rhs.mData.stringPtr[i]) return false;
			}

			return true;
		}
		case DatumTypes::Pointer:
		{
			for (std::size_t i = 0; i < mSize; ++i)
			{
				if (!mData.rttiPtr[i]->Equals(rhs.mData.rttiPtr[i])) return false;
			}

			return true;
		}

		default:
			return false;
			break;
		}
	}

	template<>
	bool Datum::operator==<Datum::RTTIPointer>(const Datum::RTTIPointer& rhs) const
	{
		if (mSize != 1) return false;
		return mData.rttiPtr[0]->Equals(rhs);
	}

	template<typename T>
	bool Datum::operator!=(const T& rhs) const
	{
		return !(operator==(rhs));
	}
#pragma endregion Boolean Operators

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
		if (mType == DatumTypes::Unknown)		throw std::runtime_error("Type not set.");
		if (TypeOf<T>() == DatumTypes::Unknown) throw std::runtime_error("Unknown data type.");
		if (TypeOf<T>() != mType)				throw std::runtime_error("Incorrect type.");
		if (index >= mSize)						throw std::out_of_range("Index out of bounds.");

		reinterpret_cast<T*>(mData.voidPtr)[index] = value;
		return reinterpret_cast<T*>(mData.voidPtr)[index];
	}

	template<typename T>
	inline T& Datum::Get(std::size_t index)
	{
		if (mType == DatumTypes::Unknown)		throw std::runtime_error("Type not set.");
		if (TypeOf<T>() == DatumTypes::Unknown) throw std::runtime_error("Unknown data type.");
		if (TypeOf<T>() != mType)				throw std::runtime_error("Incorrect type.");
		if (index >= mSize)						throw std::out_of_range("Index out of bounds.");

		return reinterpret_cast<T*>(mData.voidPtr)[index];
	}

	template<typename T>
	inline const T& Datum::Get(std::size_t index) const
	{
		if (mType == DatumTypes::Unknown)		throw std::runtime_error("Type not set.");
		if (TypeOf<T>() == DatumTypes::Unknown) throw std::runtime_error("Unknown data type.");
		if (TypeOf<T>() != mType)				throw std::runtime_error("Incorrect type.");
		if (index >= mSize)						throw std::out_of_range("Index out of bounds.");

		return reinterpret_cast<T*>(mData.voidPtr)[index];
	}

	template<typename T>
	inline std::size_t Datum::Find(const T& value) const
	{
		if (mType == DatumTypes::Unknown)		throw std::runtime_error("Type not set.");
		if (TypeOf<T>() == DatumTypes::Unknown) throw std::runtime_error("Unknown data type.");
		if (TypeOf<T>() != mType)				throw std::runtime_error("Incorrect type.");

		T*& data = reinterpret_cast<T*>(mData.voidPtr);
		
		std::size_t i = 0;
		for (; i < mSize; ++i)
		{
			if (data[i] == value || (TypeOf<T>() == DatumTypes::Pointer && data[i].Equals(value)))
			{
				break;
			}
		}

		return i;
	}
#pragma endregion Element Accessors

#pragma region Modifiers
	template<typename T>
	void Datum::PushBack(const T& data)
	{
		if (TypeOf<T>() == DatumTypes::Unknown) throw std::runtime_error("Unknown data type.");
		if (!mInternalStorage)					throw std::runtime_error("External storage.");

		if (mType == DatumTypes::Unknown)
		{
			mType = TypeOf<T>();
		}
		else if (mType != TypeOf<T>())
		{
			throw std::runtime_error("Incorrect type.");
		}

		if (mCapacity <= mSize)
		{
			std::size_t newCapacity = mReserveFunctor(mCapacity, mSize);
			Reserve(std::max(newCapacity, mCapacity + 1));
		}

		new(reinterpret_cast<T*>(mData.voidPtr) + mSize++)T(data);
	}

	template<typename T>
	inline bool Datum::Remove(const T& value)
	{
		if (mType == DatumTypes::Unknown)		throw std::runtime_error("Type not set.");
		if (TypeOf<T>() == DatumTypes::Unknown) throw std::runtime_error("Unknown data type.");
		if (TypeOf<T>() != mType)				throw std::runtime_error("Incorrect type.");

		T* data = reinterpret_cast<T*>(mData.voidPtr);

		for (std::size_t i = 0; i < mSize; ++i)
		{
			if (data[i] == value || (TypeOf<T>() == DatumTypes::Pointer && data[i].Equals(value)))
			{
				data[i].~T();
				memmove(&data[i], &data[i + 1], sizeof(T) * (mSize - i));

				--mSize;
				return true;
			}
		}

		return false;
	}

	template<typename T>
	inline bool Datum::RemoveAt(const T& value, const std::size_t index)
	{
		if (mType == DatumTypes::Unknown)		throw std::runtime_error("Type not set.");
		if (TypeOf<T>() == DatumTypes::Unknown) throw std::runtime_error("Unknown data type.");
		if (TypeOf<T>() != mType)				throw std::runtime_error("Incorrect type.");

		T*& data = reinterpret_cast<T*>(mData.voidPtr);

		if (data[index] == value || (TypeOf<T>() == DatumTypes::Pointer && data[index].Equals(value)))
		{
			data[index].~T();
			memmove(&data[index], &data[index + 1], sizeof(T) * (mSize - index));

			--mSize;
			return true;
		}

		return false;
	}
#pragma endregion Modifiers
}