#include "pch.h"

#include "Datum.h"

namespace Library
{
	#pragma region Constructors, Destructor, Assignment
	Datum::Datum(ReserveFunctor reserveFunctor) :
		mReserveFunctor(reserveFunctor)
	{
	}
	
	Datum::~Datum()
	{
		Clear();
	
		if (mInternalStorage && mData.voidPtr != nullptr)
		{
			free(mData.voidPtr);
		}
	
		mData.voidPtr = nullptr;
		mCapacity = 0;
	}
	
	Datum::Datum(const Datum& rhs) :
		mType(rhs.mType), mInternalStorage(rhs.mInternalStorage), mReserveFunctor(rhs.mReserveFunctor)
	{
		Reserve(rhs.mCapacity);
	}
	
	Datum& Datum::operator=(const Datum& rhs)
	{
		if (mType != rhs.mType)
		{
			throw std::runtime_error("Incompatible types.");
		}

		return *this;
	}
	
	Datum::Datum(Datum&& rhs) noexcept :
		mData(rhs.mData), mType(rhs.mType), mSize(rhs.mSize), mCapacity(rhs.mCapacity), mInternalStorage(rhs.mInternalStorage), mReserveFunctor(rhs.mReserveFunctor)
	{
	}
	
	Datum& Datum::operator=(Datum&& rhs)
	{
		if (mType != rhs.mType)
		{
			throw std::runtime_error("Incompatible types.");
		}

		return *this;
	}
	
	Datum& Datum::operator=(const int rhs)
	{
		if (mType != DatumTypes::Integer)
		{
			throw std::runtime_error("Incompatible types.");
		}

		Reserve(1);
		PushBack(rhs, mData.intPtr);

		return *this;
	}
	
	#pragma endregion Constructors, Destructor, Assignment


#pragma region Modifiers
	void Datum::Clear()
	{
		if (!mInternalStorage) return;
		
		if (mType == DatumTypes::String)
		{
			for (std::size_t i = 0; i < mSize; ++i)
			{
				std::destroy_at(mData.stringPtr + i);
			}
		}

		mSize = 0;
	}
#pragma endregion Modifiers
}
