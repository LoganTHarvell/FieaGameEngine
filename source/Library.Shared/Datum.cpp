#include "pch.h"

#include "Datum.h"

namespace Library
{
#pragma region Constructors, Destructor, Assignment
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
		mType(rhs.mType), mSize(rhs.mSize), mInternalStorage(rhs.mInternalStorage), mReserveFunctor(rhs.mReserveFunctor)
	{
		if (this != &rhs)
		{
			if (mInternalStorage && rhs.mCapacity > 0)
			{
				Reserve(rhs.mCapacity);

				if (mType == DatumTypes::String)
				{
					for (std::size_t i = 0; i < mSize; ++i)
					{
						new(mData.stringPtr + i)std::string(rhs.mData.stringPtr[i]);
					}
				}
				else if (mData.voidPtr && rhs.mData.voidPtr)
				{
					std::size_t dataSize = DatumSizeLUT[static_cast<std::size_t>(mType)];
					std::memcpy(mData.voidPtr, rhs.mData.voidPtr, rhs.mSize * dataSize);
				}
			}
			else
			{
				mData = rhs.mData;
				mCapacity = rhs.mCapacity;
			}
		}
	}
	
	Datum& Datum::operator=(const Datum& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			ShrinkToFit();
			
			mType = rhs.mType;
			mSize = rhs.mSize;
			mInternalStorage = rhs.mInternalStorage;
			mReserveFunctor = rhs.mReserveFunctor;

			if (mInternalStorage && rhs.mCapacity > 0)
			{
				Reserve(rhs.mCapacity);

				if (mType == DatumTypes::String)
				{
					for (std::size_t i = 0; i < mSize; ++i)
					{
						new(mData.stringPtr + i)std::string(rhs.mData.stringPtr[i]);
					}
				}
				else
				{
					std::size_t dataSize = DatumSizeLUT[static_cast<std::size_t>(mType)];
					std::memcpy(mData.voidPtr, rhs.mData.voidPtr, rhs.mSize * dataSize);
				}
			}
			else
			{
				mData = rhs.mData;
				mCapacity = rhs.mCapacity;
			}
		}

		return *this;
	}
	
	Datum::Datum(Datum&& rhs) noexcept :
		mData(rhs.mData), mType(rhs.mType), mSize(rhs.mSize), mCapacity(rhs.mCapacity), 
		mInternalStorage(rhs.mInternalStorage), mReserveFunctor(rhs.mReserveFunctor)
	{
		rhs.mData.voidPtr = nullptr;
		rhs.mSize = 0;
		rhs.mCapacity = 0;
		rhs.mInternalStorage = true;
	}
	
	Datum& Datum::operator=(Datum&& rhs) noexcept
	{
		if (this != &rhs)
		{
			if (mInternalStorage && mCapacity > 0)
			{
				Clear();
				free(mData.voidPtr);
			}

			mData.voidPtr = rhs.mData.voidPtr;
			mType = rhs.mType;
			mSize = rhs.mSize;
			mCapacity = rhs.mCapacity;
			mInternalStorage = rhs.mInternalStorage;
			mReserveFunctor = rhs.mReserveFunctor;

			rhs.mData.voidPtr = nullptr;
			rhs.mSize = 0;
			rhs.mCapacity = 0;
			rhs.mInternalStorage = true;
		}

		return *this;
	}

#pragma region Constructors
#pragma region Scalar Constructor Overloads
	Datum::Datum(const int rhs)
	{
		ConstructorHelper({ rhs });
	}

	Datum::Datum(const float rhs)
	{
		ConstructorHelper({ rhs });
	}

	Datum::Datum(const glm::vec4& rhs)
	{
		ConstructorHelper({ rhs });
	}

	Datum::Datum(const glm::mat4& rhs)
	{
		ConstructorHelper({ rhs });
	}

	Datum::Datum(const std::string& rhs)
	{
		ConstructorHelper({ rhs });
	}

	Datum::Datum(RTTIPointer const& rhs)
	{
		ConstructorHelper({ rhs });
	}
#pragma endregion Scalar Constructor Overloads

#pragma region Initializer List Constructor Overloads
	Datum::Datum(const std::initializer_list<int> rhs)
	{
		ConstructorHelper(rhs);
	}

	Datum::Datum(const std::initializer_list<float> rhs)
	{
		ConstructorHelper(rhs);
	}

	Datum::Datum(const std::initializer_list <glm::vec4> rhs)
	{
		ConstructorHelper(rhs);
	}

	Datum::Datum(const std::initializer_list <glm::mat4> rhs)
	{
		ConstructorHelper(rhs);
	}

	Datum::Datum(const std::initializer_list <std::string> rhs)
	{
		ConstructorHelper(rhs);
	}

	Datum::Datum(const std::initializer_list <RTTIPointer> rhs)
	{
		ConstructorHelper(rhs);
	}
#pragma endregion Initializer List Constructor Overloads
#pragma endregion Constructors

#pragma region Assignment
#pragma region Scalar Assignment Overloads
	Datum& Datum::operator=(const int rhs)
	{
		return AssignmentHelper({ rhs });
	}

	Datum& Datum::operator=(const float rhs)
	{
		return AssignmentHelper({ rhs });
	}

	Datum& Datum::operator=(const glm::vec4& rhs)
	{
		return AssignmentHelper({ rhs });
	}

	Datum& Datum::operator=(const glm::mat4& rhs)
	{
		return AssignmentHelper({ rhs });
	}

	Datum& Datum::operator=(const std::string& rhs)
	{
		return AssignmentHelper({ rhs });
	}

	Datum& Datum::operator=(RTTIPointer const& rhs)
	{
		return AssignmentHelper({ rhs });
	}
#pragma endregion Scalar Assignment Overloads

#pragma region Initializer List Assignment Overloads
	Datum& Datum::operator=(std::initializer_list<int> rhs)
	{
		return AssignmentHelper(rhs);
	}

	Datum& Datum::operator=(std::initializer_list<float> rhs)
	{
		return AssignmentHelper(rhs);
	}

	Datum& Datum::operator=(std::initializer_list<glm::vec4> rhs)
	{
		return AssignmentHelper(rhs);
	}

	Datum& Datum::operator=(std::initializer_list<glm::mat4> rhs)
	{
		return AssignmentHelper(rhs);
	}

	Datum& Datum::operator=(std::initializer_list<std::string> rhs)
	{
		return AssignmentHelper(rhs);
	}

	Datum& Datum::operator=(std::initializer_list<RTTIPointer> rhs)
	{
		return AssignmentHelper(rhs);
	}
#pragma endregion Initializer List Assignment Overloads
#pragma endregion Assignment
#pragma endregion Constructors, Destructor, Assignment

#pragma region Boolean Operators
	bool Datum::operator==(const Datum& rhs) const noexcept
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
				if (mData.rttiPtr[i]->Equals(rhs.mData.rttiPtr[i])) return false;
			}

			return true;
		}

		default:
			return false;
			break;
		}
	}

	bool Datum::operator!=(const Datum& rhs) const noexcept
	{
		return !(operator==(rhs));
	}

#pragma region Equals Scalar
	bool Datum::operator==(const int rhs) const noexcept
	{
		return EqualsScalarHelper(rhs, mData.intPtr);
	}

	bool Datum::operator==(const float rhs) const noexcept
	{
		return EqualsScalarHelper(rhs, mData.floatPtr);
	}

	bool Datum::operator==(const glm::vec4& rhs) const noexcept
	{
		return EqualsScalarHelper(rhs, mData.vectorPtr);
	}

	bool Datum::operator==(const glm::mat4& rhs) const noexcept
	{
		return EqualsScalarHelper(rhs, mData.matrixPtr);
	}

	bool Datum::operator==(const std::string& rhs) const noexcept
	{
		return EqualsScalarHelper(rhs, mData.stringPtr);
	}

	bool Datum::operator==(const RTTIPointer& rhs) const noexcept
	{
		return EqualsScalarHelper(rhs, mData.rttiPtr);
	}
#pragma endregion Equals Scalar

#pragma region Not Equals Scalar
	bool Datum::operator!=(const int rhs) const noexcept
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const float rhs) const noexcept
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const glm::vec4& rhs) const noexcept
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const glm::mat4& rhs) const noexcept
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const std::string& rhs) const noexcept
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const RTTIPointer& rhs) const noexcept
	{
		return !(operator==(rhs));
	}
#pragma endregion  Not Equals Scalar
#pragma endregion Boolean Operators

#pragma region Size and Capacity
	void Datum::Reserve(std::size_t capacity)
	{
		if (!mInternalStorage) throw std::runtime_error("External storage.");
		if (mType == DatumTypes::Unknown) throw std::runtime_error("Datum type is unknown.");

		if (capacity > mCapacity)
		{
			void* newMemory = realloc(mData.voidPtr, capacity * DatumSizeLUT[static_cast<std::size_t>(mType)]);

			assert(newMemory != nullptr);

			mData.voidPtr = newMemory;
			mCapacity = capacity;
		}
	}

	void Datum::Resize(std::size_t size)
	{
		switch (mType)
		{
		case DatumTypes::Integer:
			ResizeHelper(size, mData.intPtr);
			break;
		case DatumTypes::Float:
			ResizeHelper(size, mData.floatPtr);
			break;
		case DatumTypes::Vector:
			ResizeHelper(size, mData.vectorPtr);
			break;
		case DatumTypes::Matrix:
			ResizeHelper(size, mData.matrixPtr);
			break;
		case DatumTypes::String:
			ResizeHelper(size, mData.stringPtr);
			break;
		case DatumTypes::Pointer:
			ResizeHelper(size, mData.rttiPtr);
			break;

		default:
			break;
		}
	}

	void Datum::ShrinkToFit()
	{
		if (!mInternalStorage) throw std::runtime_error("External storage.");
		if (mType == DatumTypes::Unknown) throw std::runtime_error("Datum type is unknown.");

		if (mSize == 0)
		{
			free(mData.voidPtr);
			mData.voidPtr = nullptr;
		}
		else if (mSize < mCapacity)
		{
			void* newMemory = realloc(mData.voidPtr, mSize * DatumSizeLUT[static_cast<std::size_t>(mType)]);

			assert(newMemory != nullptr);

			mData.voidPtr = newMemory;
		}

		mCapacity = mSize;
	}
#pragma endregion Size and Capacity

#pragma region Modifiers
#pragma region Push Back Overloads
	void Datum::PushBack(const int data)
	{
		PushBackHelper(data, mData.intPtr);
	}

	void Datum::PushBack(const float data)
	{
		PushBackHelper(data, mData.floatPtr);
	}

	void Datum::PushBack(const glm::vec4& data)
	{
		PushBackHelper(data, mData.vectorPtr);
	}

	void Datum::PushBack(const glm::mat4& data)
	{
		PushBackHelper(data, mData.matrixPtr);
	}

	void Datum::PushBack(const std::string& data)
	{
		PushBackHelper(data, mData.stringPtr);
	}

	void Datum::PushBack(const RTTIPointer& data)
	{
		PushBackHelper(data, mData.rttiPtr);
	}
#pragma endregion Push Back Overloads

	void Datum::Clear()
	{		
		if (!mInternalStorage)
		{
			mData.voidPtr = nullptr;
		}
		else if (mType == DatumTypes::String)
		{
			for (std::size_t i = 0; i < mSize; ++i)
			{
				std::destroy_at(mData.stringPtr + i);
			}
		}

		mSize = 0;
		mInternalStorage = true;
	}
#pragma endregion Modifiers

#pragma region Helper Methods
	template<typename T>
	void Datum::ConstructorHelper(const std::initializer_list<T> rhs)
	{
		mType = TypeOf<T>();
		Reserve(rhs.size());

		for (const auto& value : rhs)
		{
			PushBack(value);
		}
	}

	template<typename T>
	Datum& Datum::AssignmentHelper(const std::initializer_list<T> rhs)
	{
		if (!mInternalStorage) throw std::runtime_error("External storage.");

		if (mType == DatumTypes::Unknown)
		{
			mType = TypeOf<T>();
		}
		else if (mType != TypeOf<T>())
		{
			throw std::runtime_error("Incompatible types.");
		}

		Clear();
		ShrinkToFit();
		Reserve(rhs.size());

		for (const auto& value : rhs)
		{
			PushBack(value);
		}

		return *this;
	}

	template<typename T>
	inline bool Datum::EqualsScalarHelper(const T& rhs, const T* mDataPtr) const
	{
		if (mSize != 1) return false;
		return mDataPtr[0] == rhs;
	}

	template<>
	inline bool Datum::EqualsScalarHelper(const Datum::RTTIPointer& rhs, const Datum::RTTIPointer* mDataPtr) const
	{
		if (mSize != 1) return false;
		return (*mDataPtr)->Equals(rhs);
	}

	template<typename T>
	void Datum::ResizeHelper(std::size_t size, T*& mDataPtr)
	{
		if (size > mSize)
		{
			Reserve(size);

			for (std::size_t i = mSize; i < size; ++i)
			{
				new(mDataPtr + i)T();
			}
		}
		else if (size < mSize)
		{
			for (std::size_t i = size; i < mSize; ++i)
			{
				mDataPtr[i].~T();
			}
		}

		mSize = size;
	}

	template<typename T>
	inline void Datum::PushBackHelper(const T& data, T*& mDataPtr)
	{
		if (!mInternalStorage) throw std::runtime_error("External storage.");

		if (mType == DatumTypes::Unknown)
		{
			mType = TypeOf<T>();
		}
		else if (mType != TypeOf<T>())
		{
			throw std::runtime_error("Incompatible types.");
		}

		if (mCapacity <= mSize)
		{
			std::size_t newCapacity = mReserveFunctor(mCapacity, mSize);
			Reserve(std::max(newCapacity, mCapacity + 1));
		}

		new(mDataPtr + mSize++)T(data);
	}
#pragma endregion Helper Methods
}
