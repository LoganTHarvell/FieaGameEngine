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
// 	template<typename T>
// 	bool Datum::operator==(const T& rhs) const
// 	{
// 		if (TypeOf<T>() == DatumTypes::Unknown) throw std::runtime_error("Incorrect type.");
// 
// 		if (mSize != 1) return false;
// 		
// 		T*& data = reinterpret_cast<T*>(mData.voidPtr);
// 		bool isEqual = (data[0] == rhs || (TypeOf<T>() == DatumTypes::Pointer && data[0].Equals(rhs)));
// 		return isEqual;
// 	}
// 
// 	template<>
// 	bool Datum::operator==<Datum>(const Datum& rhs) const
// 	{
// 		if (this == &rhs)
// 		{
// 			return true;
// 		}
// 
// 		if (mType != rhs.mType || mSize != rhs.mSize)
// 		{
// 			return false;
// 		}
// 
// 		switch (mType)
// 		{
// 		case DatumTypes::Unknown:
// 			return true;
// 		case DatumTypes::Integer:
// 		case DatumTypes::Float:
// 		case DatumTypes::Vector:
// 		case DatumTypes::Matrix:
// 		{
// 			const std::size_t size = mSize * DatumSizeLUT[static_cast<std::size_t>(mType)];
// 			return memcmp(mData.voidPtr, rhs.mData.voidPtr, size) == 0;
// 		}
// 		case DatumTypes::String:
// 		{
// 			for (std::size_t i = 0; i < mSize; ++i)
// 			{
// 				if (mData.stringPtr[i] != rhs.mData.stringPtr[i]) return false;
// 			}
// 
// 			return true;
// 		}
// 		case DatumTypes::Pointer:
// 		{	
// 			for (std::size_t i = 0; i < mSize; ++i)
// 			{
// 				if (mData.rttiPtr[i]->Equals(rhs.mData.rttiPtr[i])) return false;
// 			}
// 
// 			return true;
// 		}
// 
// 		default:
// 			return false;
// 			break;
// 		}
// 	}
// 
// 	template<typename T>
// 	bool Datum::operator!=(const T& rhs) const
// 	{
// 		return !(operator==(rhs));
// 	}

// #pragma region Equals Scalar
// 	bool Datum::operator==(const int rhs) const noexcept
// 	{
// 		return EqualsScalarHelper(rhs);
// 	}
// 
// 	bool Datum::operator==(const float rhs) const noexcept
// 	{
// 		return EqualsScalarHelper(rhs);
// 	}
// 
// 	bool Datum::operator==(const glm::vec4& rhs) const noexcept
// 	{
// 		return EqualsScalarHelper(rhs);
// 	}
// 
// 	bool Datum::operator==(const glm::mat4& rhs) const noexcept
// 	{
// 		return EqualsScalarHelper(rhs);
// 	}
// 
// 	bool Datum::operator==(const std::string& rhs) const noexcept
// 	{
// 		return EqualsScalarHelper(rhs);
// 	}
// 
// 	bool Datum::operator==(const RTTIPointer& rhs) const noexcept
// 	{
// 		return EqualsScalarHelper(rhs);
// 	}
// #pragma endregion Equals Scalar
// 
// #pragma region Not Equals Scalar
// 	bool Datum::operator!=(const int rhs) const noexcept
// 	{
// 		return !(operator==(rhs));
// 	}
// 
// 	bool Datum::operator!=(const float rhs) const noexcept
// 	{
// 		return !(operator==(rhs));
// 	}
// 
// 	bool Datum::operator!=(const glm::vec4& rhs) const noexcept
// 	{
// 		return !(operator==(rhs));
// 	}
// 
// 	bool Datum::operator!=(const glm::mat4& rhs) const noexcept
// 	{
// 		return !(operator==(rhs));
// 	}
// 
// 	bool Datum::operator!=(const std::string& rhs) const noexcept
// 	{
// 		return !(operator==(rhs));
// 	}
// 
// 	bool Datum::operator!=(const RTTIPointer& rhs) const noexcept
// 	{
// 		return !(operator==(rhs));
// 	}
// #pragma endregion  Not Equals Scalar
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
			ResizeHelper<int>(size);
			break;
		case DatumTypes::Float:
			ResizeHelper<float>(size);
			break;
		case DatumTypes::Vector:
			ResizeHelper<glm::vec4>(size);
			break;
		case DatumTypes::Matrix:
			ResizeHelper<glm::mat4>(size);
			break;
		case DatumTypes::String:
			ResizeHelper<std::string>(size);
			break;
		case DatumTypes::Pointer:
			ResizeHelper<RTTIPointer>(size);
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
	void Datum::PopBack()
	{
		switch (mType)
		{
		case DatumTypes::Integer:
			PopBackHelper<int>();
			break;
		case DatumTypes::Float:
			PopBackHelper<float>();
			break;
		case DatumTypes::Vector:
			PopBackHelper<glm::vec4>();
			break;
		case DatumTypes::Matrix:
			PopBackHelper<glm::mat4>();
			break;
		case DatumTypes::String:
			PopBackHelper<std::string>();
			break;
		case DatumTypes::Pointer:
			PopBackHelper<RTTIPointer>();
			break;

		default:
			break;
		}
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
	inline bool Datum::EqualsScalarHelper(const T& rhs) const
	{
		if (mSize != 1) return false;
		return reinterpret_cast<T*>(mData.voidPtr)[0] == rhs;
	}

	template<>
	inline bool Datum::EqualsScalarHelper(const Datum::RTTIPointer& rhs) const
	{
		if (mSize != 1) return false;
		return (*mData.rttiPtr)->Equals(rhs);
	}

	template<typename T>
	void Datum::ResizeHelper(std::size_t size)
	{
		if (size > mSize)
		{
			Reserve(size);

			for (std::size_t i = mSize; i < size; ++i)
			{
				new(reinterpret_cast<T*>(mData.voidPtr) + i)T();
			}
		}
		else if (size < mSize)
		{
			for (std::size_t i = size; i < mSize; ++i)
			{
				reinterpret_cast<T*>(mData.voidPtr)[i].~T();
			}
		}

		mSize = size;
	}

	template<typename T>
	inline void Datum::PopBackHelper()
	{
		if (!mInternalStorage) throw std::runtime_error("External storage.");

		if (mSize > 0)
		{
			reinterpret_cast<T*>(mData.voidPtr)[--mSize].~T();
		}
	}
#pragma endregion Helper Methods
}
