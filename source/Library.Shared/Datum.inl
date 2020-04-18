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
	inline constexpr Datum::Types Datum::TypeOf()
	{
		return Types::Unknown;
	}

	template<>
	inline constexpr Datum::Types Datum::TypeOf<int>()
	{
		return Types::Integer;
	}

	template<>
	inline constexpr Datum::Types Datum::TypeOf<float>()
	{
		return Types::Float;
	}

	template<>
	inline constexpr Datum::Types Datum::TypeOf<glm::vec4>()
	{
		return Types::Vector;
	}

	template<>
	inline constexpr Datum::Types Datum::TypeOf<glm::mat4>()
	{
		return Types::Matrix;
	}

	template<>
	inline constexpr Datum::Types Datum::TypeOf<std::string>()
	{
		return Types::String;
	}

	template<>
	inline constexpr Datum::Types Datum::TypeOf<Datum::ScopePointer>()
	{
		return Types::Scope;
	}

	template<>
	inline constexpr Datum::Types Datum::TypeOf<Datum::RTTIPointer>()
	{
		return Types::Pointer;
	}

	template<>
	inline constexpr Datum::Types Datum::TypeOf<Datum::DatumPointer>()
	{
		return Types::Reference;
	}
#pragma endregion TypeOf Static Method

#pragma region Type, Size, Capacity
	inline Datum::Types Datum::Type() const
	{
		return mType;
	}

	inline void Datum::SetType(Types type)
	{
		if (type == Types::Unknown || type == Types::End)	throw std::runtime_error("Cannot assign to unknown type.");
		if (mType != Types::Unknown && mType != type)		throw std::runtime_error("Type cannot be reassigned.");

		mType = type;
	}

	inline bool Datum::HasInternalStorage() const
	{
		return mInternalStorage;
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
#pragma endregion Type, Size, Capacity

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
		static_assert(TypeOf<T>() != Types::Unknown && TypeOf<T>() != Types::End, "Invalid data type.");

		if (mType == Types::Unknown)	throw std::runtime_error("Type not set.");
		if (mType != TypeOf<T>())		throw std::runtime_error("Mismatched type.");
		if (index >= mSize)				throw std::out_of_range("Index out of bounds.");
		
		return Get<T>(index) = value;
	}

	template<typename T>
	inline T& Datum::Get(std::size_t index)
	{
		static_assert(TypeOf<T>() != Types::Unknown && TypeOf<T>() != Types::End, "Invalid data type.");

		if (mType == Types::Unknown)	throw std::runtime_error("Type not set.");
		if (mType != TypeOf<T>())		throw std::runtime_error("Mismatched type.");
		if (index >= mSize)				throw std::out_of_range("Index out of bounds.");

		return reinterpret_cast<T*>(mData.VoidPtr)[index];
	}

	template<typename T>
	inline const T& Datum::Get(std::size_t index) const
	{
		static_assert(TypeOf<T>() != Types::Unknown && TypeOf<T>() != Types::End, "Invalid data type.");

		if (mType == Types::Unknown)	throw std::runtime_error("Type not set.");
		if (mType != TypeOf<T>())		throw std::runtime_error("Mismatched type.");
		if (index >= mSize)				throw std::out_of_range("Index out of bounds.");

		return reinterpret_cast<T*>(mData.VoidPtr)[index];
	}

	template<typename T>
	inline T* Datum::Find(const T& value)
	{
		static_assert(TypeOf<T>() != Types::Unknown && TypeOf<T>() != Types::End, "Invalid data type.");

		const std::size_t index = IndexOf(value);
		return index < mSize ? &Get<T>(index) : nullptr;
	}

	template<typename T>
	inline const T* Datum::Find(const T& value) const
	{
		static_assert(TypeOf<T>() != Types::Unknown && TypeOf<T>() != Types::End, "Invalid data type.");

		const std::size_t index = IndexOf(value);
		return index < mSize ? &Get<T>(index) : nullptr;
	}

	template<typename T>
	inline std::size_t Datum::IndexOf(const T& value) const
	{
		static_assert(TypeOf<T>() != Types::Unknown && TypeOf<T>() != Types::End, "Invalid data type.");

		if (mType == Types::Unknown)	throw std::runtime_error("Type not set.");
		if (mType != TypeOf<T>())		throw std::runtime_error("Mismatched type.");

		T* const data = reinterpret_cast<T*>(mData.VoidPtr);
		T valueCopy = value;

		std::size_t i = 0;
		for (; i < mSize; ++i)
		{
			if (ElementEqualityLUT[static_cast<std::size_t>(mType)](&data[i], &valueCopy))
			{
				break;
			}
		}

		return i;
	}
#pragma endregion Element Accessors

#pragma region Modifiers
	template<typename T>
	inline void Datum::SetStorage(const gsl::span<T> storage)
	{
		static_assert(TypeOf<T>() != Types::Unknown && TypeOf<T>() != Types::End, "Invalid data type.");

		if (storage.size() < 1) throw std::runtime_error("External storage size must be greater than zero.");
		
		if (mType == Types::Unknown)
		{
			mType = TypeOf<T>();
		}
		else if (mType != TypeOf<T>())
		{
			throw std::runtime_error("Mismatched types.");
		}

		Clear();
		ShrinkToFit();

		mData.VoidPtr = storage.data();
		mSize = storage.size();
		mCapacity = storage.size();
		mInternalStorage = false;
	}

	inline void Datum::SetStorage(const Types type, const gsl::span<std::byte> storage)
	{		
		assert(storage.size() > 0);

		mType = type;

		Clear();
		ShrinkToFit();

		mData.VoidPtr = storage.data();
		mSize = storage.size();
		mCapacity = storage.size();
		mInternalStorage = false;
	}

	template<typename T, typename... Args>
	inline T& Datum::EmplaceBack(Args&&... args)
	{
		static_assert(TypeOf<T>() != Types::Unknown && TypeOf<T>() != Types::End, "Invalid data type.");

		if (!mInternalStorage) throw std::runtime_error("Cannot modify external storage.");

		if (mType == Types::Unknown)
		{
			mType = TypeOf<T>();
		}
		else if (mType != TypeOf<T>())
		{
			throw std::runtime_error("Mismatched type.");
		}

		if (mCapacity <= mSize)
		{
			const std::size_t newCapacity = mReserveFunctor->operator()(mCapacity, mSize);
			Reserve(std::max(newCapacity, mCapacity + 1));
		}

		return *new(reinterpret_cast<T*>(mData.VoidPtr) + mSize++)T(std::forward<Args>(args)...);
	}

	template<typename T>
	inline void Datum::PushBack(const T& data)
	{
		EmplaceBack<T>(data);
	}

	template<typename T>
	inline typename std::enable_if_t<!std::is_lvalue_reference<T>::value> Datum::PushBack(T&& data)
	{
		EmplaceBack<T>(std::move(data));
	}

	template<typename T>
	inline bool Datum::Remove(const T& value)
	{
		static_assert(TypeOf<T>() != Types::Unknown && TypeOf<T>() != Types::End, "Invalid data type.");

		if (!mInternalStorage) throw std::runtime_error("Cannot modify external storage.");

		T* const data = reinterpret_cast<T*>(mData.VoidPtr);
		const std::size_t index = IndexOf(value);
		
		if (index < mSize)
		{
			if (mType == Types::String)
			{
				mData.StringPtr[index].~basic_string();
			}

			std::memmove(&data[index], &data[index + 1], sizeof(T) * (mSize - index));

			--mSize;
			return true;
		}

		return false;
	}

	inline void Datum::SetReserveStrategy(const ReserveFunctor& reserveFunctor)
	{
		mReserveFunctor = std::make_shared<ReserveFunctor>(reserveFunctor);
	}
#pragma endregion Modifiers
}