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
		ConstructorAssignmentHelper({ rhs });
	}

	Datum::Datum(const float rhs)
	{
		ConstructorAssignmentHelper({ rhs });
	}

	Datum::Datum(const glm::vec4& rhs)
	{
		ConstructorAssignmentHelper({ rhs });
	}

	Datum::Datum(const glm::mat4& rhs)
	{
		ConstructorAssignmentHelper({ rhs });
	}

	Datum::Datum(const std::string& rhs)
	{
		ConstructorAssignmentHelper({ rhs });
	}

	Datum::Datum(RTTIPointer const& rhs)
	{
		ConstructorAssignmentHelper({ rhs });
	}
#pragma endregion Scalar Constructor Overloads

#pragma region Initializer List Constructor Overloads
	Datum::Datum(const std::initializer_list<int> rhs)
	{
		ConstructorAssignmentHelper(rhs);
	}

	Datum::Datum(const std::initializer_list<float> rhs)
	{
		ConstructorAssignmentHelper(rhs);
	}

	Datum::Datum(const std::initializer_list <glm::vec4> rhs)
	{
		ConstructorAssignmentHelper(rhs);
	}

	Datum::Datum(const std::initializer_list <glm::mat4> rhs)
	{
		ConstructorAssignmentHelper(rhs);
	}

	Datum::Datum(const std::initializer_list <std::string> rhs)
	{
		ConstructorAssignmentHelper(rhs);
	}

	Datum::Datum(const std::initializer_list <RTTIPointer> rhs)
	{
		ConstructorAssignmentHelper(rhs);
	}
#pragma endregion Initializer List Constructor Overloads
#pragma endregion Constructors

#pragma region Assignment
#pragma region Scalar Assignment Overloads
	Datum& Datum::operator=(const int rhs)
	{
		return ConstructorAssignmentHelper({ rhs });
	}

	Datum& Datum::operator=(const float rhs)
	{
		return ConstructorAssignmentHelper({ rhs });
	}

	Datum& Datum::operator=(const glm::vec4& rhs)
	{
		return ConstructorAssignmentHelper({ rhs });
	}

	Datum& Datum::operator=(const glm::mat4& rhs)
	{
		return ConstructorAssignmentHelper({ rhs });
	}

	Datum& Datum::operator=(const std::string& rhs)
	{
		return ConstructorAssignmentHelper({ rhs });
	}

	Datum& Datum::operator=(RTTIPointer const& rhs)
	{
		return ConstructorAssignmentHelper({ rhs });
	}
#pragma endregion Scalar Assignment Overloads

#pragma region Initializer List Assignment Overloads
	Datum& Datum::operator=(std::initializer_list<int> rhs)
	{
		return ConstructorAssignmentHelper(rhs);
	}

	Datum& Datum::operator=(std::initializer_list<float> rhs)
	{
		return ConstructorAssignmentHelper(rhs);
	}

	Datum& Datum::operator=(std::initializer_list<glm::vec4> rhs)
	{
		return ConstructorAssignmentHelper(rhs);
	}

	Datum& Datum::operator=(std::initializer_list<glm::mat4> rhs)
	{
		return ConstructorAssignmentHelper(rhs);
	}

	Datum& Datum::operator=(std::initializer_list<std::string> rhs)
	{
		return ConstructorAssignmentHelper(rhs);
	}

	Datum& Datum::operator=(std::initializer_list<RTTIPointer> rhs)
	{
		return ConstructorAssignmentHelper(rhs);
	}
#pragma endregion Initializer List Assignment Overloads
#pragma endregion Assignment
#pragma endregion Constructors, Destructor, Assignment

#pragma region Boolean Operators
	bool Datum::operator==(const Datum& rhs) const noexcept
	{
		if (this == &rhs)								return true;
		if (mType != rhs.mType || mSize != rhs.mSize)	return false;
		
		bool isEqual = true;

		switch (mType)
		{
		case DatumTypes::Unknown:
			break;
		case DatumTypes::Integer:
		case DatumTypes::Float:
		case DatumTypes::Vector:
		case DatumTypes::Matrix:
		{
			const std::size_t size = mSize * DatumSizeLUT[static_cast<std::size_t>(mType)];
			isEqual = memcmp(mData.voidPtr, rhs.mData.voidPtr, size) == 0;
			break;
		}
		case DatumTypes::String:
		{
			for (std::size_t i = 0; i < mSize; ++i)
			{
				if (mData.stringPtr[i] != rhs.mData.stringPtr[i]) isEqual = false;
			}

			break;
		}
		case DatumTypes::Pointer:
		{
			for (std::size_t i = 0; i < mSize; ++i)
			{
				if (!mData.rttiPtr[i]->Equals(rhs.mData.rttiPtr[i]))
				{
					isEqual = false;
					break;
				}
			}

			break;
		}

		default:
			isEqual = false;
			break;
		}

		return isEqual;
	}

	bool Datum::operator!=(const Datum& rhs) const noexcept
	{
		return !(operator==(rhs));
	}

#pragma region Equals Scalar
	bool Datum::operator==(const int rhs) const noexcept
	{
		return mSize == 1 && mData.intPtr[0] == rhs;
	}

	bool Datum::operator==(const float rhs) const noexcept
	{
		return mSize == 1 && mData.floatPtr[0] == rhs;
	}

	bool Datum::operator==(const glm::vec4& rhs) const noexcept
	{
		return mSize == 1 && mData.vectorPtr[0] == rhs;
	}

	bool Datum::operator==(const glm::mat4& rhs) const noexcept
	{
		return mSize == 1 && mData.matrixPtr[0] == rhs;
	}

	bool Datum::operator==(const std::string& rhs) const noexcept
	{
		return mSize == 1 && mData.stringPtr[0] == rhs;
	}

	bool Datum::operator==(const RTTIPointer& rhs) const noexcept
	{
		return mSize == 1 && mData.rttiPtr[0]->Equals(rhs);
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
		if (!mInternalStorage) throw std::runtime_error("External storage.");

		if (size > mSize)
		{
			Reserve(size);

			for (std::size_t i = mSize; i < size; ++i)
			{
				ResizeHelper(i);
			}
		}
		else if (mType == DatumTypes::String && size < mSize)
		{
			for (std::size_t i = size; i < mSize; ++i)
			{
				mData.stringPtr[i].~basic_string();
			}
		}

		mSize = size;
	}

	void Datum::ShrinkToFit()
	{
		if (mType == DatumTypes::Unknown)	throw std::runtime_error("Unknown data type.");
		if (!mInternalStorage)				throw std::runtime_error("External storage.");

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
	void Datum::PopBack()
	{
		if (!mInternalStorage) throw std::runtime_error("External storage.");

		if (mSize > 0)
		{
			if (mType == DatumTypes::String)
			{
				mData.stringPtr[mSize - 1].~basic_string();
			}

			--mSize;
		}
	}

	void Datum::RemoveAt(const std::size_t index)
	{
		if (!mInternalStorage)	throw std::runtime_error("External storage.");
		if (index >= mSize)		throw std::out_of_range("Index out of bounds.");

		if (mType == DatumTypes::String)
		{
			mData.stringPtr[index].~basic_string();
		}

		std::size_t size = DatumSizeLUT[static_cast<std::size_t>(mType)];
		memmove(&mData.bytePtr[index * size], &mData.bytePtr[(index * size) + size], size * (mSize - index));

		--mSize;
	}

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
	Datum& Datum::ConstructorAssignmentHelper(const std::initializer_list<T> rhs)
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

	void Datum::ResizeHelper(std::size_t index)
	{
		switch (mType)
		{
		case DatumTypes::Integer:
			new(mData.intPtr + index)int();
			break;
		case DatumTypes::Float:
			new(mData.floatPtr + index)float();
			break;
		case DatumTypes::Vector:
			new(mData.vectorPtr + index)glm::vec4();
			break;
		case DatumTypes::Matrix:
			new(mData.matrixPtr + index)glm::mat4();
			break;
		case DatumTypes::String:
			new(mData.stringPtr + index)std::string();
			break;
		case DatumTypes::Pointer:
			new(mData.rttiPtr + index)RTTIPointer();
			break;

		default:
			break;
		}
	}
#pragma endregion Helper Methods
}
