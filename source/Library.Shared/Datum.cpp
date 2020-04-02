#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header File
#include "Datum.h"

// Standard
#include <cassert>

// First Party
#include "Scope.h"
#pragma endregion Includes

namespace Library
{
#pragma region Look Up Tables
	const Datum::EqualityFunctor Datum::EqualityLUT[static_cast<std::size_t>(Types::End)] =
	{
		[](void* lhs, void* rhs, std::size_t size) { return memcmp(lhs, rhs, size * TypeSizeLUT[static_cast<std::size_t>(Types::Integer)]) == 0; },
		[](void* lhs, void* rhs, std::size_t size) { return memcmp(lhs, rhs, size * TypeSizeLUT[static_cast<std::size_t>(Types::Float)]) == 0; },
		[](void* lhs, void* rhs, std::size_t size) { return memcmp(lhs, rhs, size * TypeSizeLUT[static_cast<std::size_t>(Types::Vector)]) == 0; },
		[](void* lhs, void* rhs, std::size_t size) { return memcmp(lhs, rhs, size * TypeSizeLUT[static_cast<std::size_t>(Types::Matrix)]) == 0; },
		[](void* lhs, void* rhs, std::size_t size) 
		{ 
			for (std::size_t i = 0; i < size; ++i)
			{
				if (reinterpret_cast<std::string*>(lhs)[i] != reinterpret_cast<std::string*>(rhs)[i])
				{
					return false;
				}
			}

			return true;
		},
		[](void* lhs, void* rhs, std::size_t size)
		{
			ScopePointer* lhsScope = reinterpret_cast<ScopePointer*>(lhs);
			ScopePointer* rhsScope = reinterpret_cast<ScopePointer*>(rhs);

			for (std::size_t i = 0; i < size; ++i)
			{
				if (!lhsScope[i] && !rhsScope[i]) continue;
				if (!lhsScope[i] || !rhsScope[i] || *lhsScope[i] != *rhsScope[i])
				{
					return false;
				}
			}

			return true;
		},
		[](void* lhs, void* rhs, std::size_t size) 
		{
			RTTIPointer* lhsRTTI = reinterpret_cast<RTTIPointer*>(lhs);
			RTTIPointer* rhsRTTI = reinterpret_cast<RTTIPointer*>(rhs);

			for (std::size_t i = 0; i < size; ++i)
			{
				if (!lhsRTTI[i] && !rhsRTTI[i]) continue;
				if (!lhsRTTI[i] || !rhsRTTI[i] || !lhsRTTI[i]->Equals(rhsRTTI[i]))
				{
					return false;
				}
			}
			
			return true;
		},
		[](void* lhs, void* rhs, std::size_t size) 
		{
			DatumPointer* lhsDatum = reinterpret_cast<DatumPointer*>(lhs);
			DatumPointer* rhsDatum = reinterpret_cast<DatumPointer*>(rhs);

			for (std::size_t i = 0; i < size; ++i)
			{
				if (!lhsDatum[i] && !rhsDatum[i]) continue;
				if (!lhsDatum[i] || !rhsDatum[i] || lhsDatum[i] != rhsDatum[i])
				{
					return false;
				}
			}
			
			return true;
		}
	};

	const Datum::ElementEqualityFunctor Datum::ElementEqualityLUT[static_cast<std::size_t>(Types::End)] =
	{
		[](void* lhs, void* rhs) { return *reinterpret_cast<int*>(lhs) == *reinterpret_cast<int*>(rhs); },
		[](void* lhs, void* rhs) { return *reinterpret_cast<float*>(lhs) == *reinterpret_cast<float*>(rhs); },
		[](void* lhs, void* rhs) { return *reinterpret_cast<glm::vec4*>(lhs) == *reinterpret_cast<glm::vec4*>(rhs); },
		[](void* lhs, void* rhs) { return *reinterpret_cast<glm::mat4*>(lhs) == *reinterpret_cast<glm::mat4*>(rhs); },
		[](void* lhs, void* rhs) { return *reinterpret_cast<std::string*>(lhs) == *reinterpret_cast<std::string*>(rhs); },
		[](void* lhs, void* rhs)
		{
			ScopePointer* lhsScope = reinterpret_cast<ScopePointer*>(lhs);
			ScopePointer* rhsScope = reinterpret_cast<ScopePointer*>(rhs);
			return ((!*lhsScope && !*rhsScope) || (*lhsScope && (*lhsScope)->Equals(*rhsScope)));
		},
		[](void* lhs, void* rhs) 
		{
			RTTIPointer* lhsRTTI = reinterpret_cast<RTTIPointer*>(lhs);
			RTTIPointer* rhsRTTI = reinterpret_cast<RTTIPointer*>(rhs);
			return ((!*lhsRTTI && !*rhsRTTI) || (*lhsRTTI && (*lhsRTTI)->Equals(*rhsRTTI)));
		},
		[](void* lhs, void* rhs) 
		{
			DatumPointer* lhsDatum = reinterpret_cast<DatumPointer*>(lhs);
			DatumPointer* rhsDatum = reinterpret_cast<DatumPointer*>(rhs);
			return ((!*lhsDatum && !*rhsDatum) || (*lhsDatum && *rhsDatum && **lhsDatum == **rhsDatum));
		}
	};

	const Datum::CreateDefaultFunctor Datum::CreateDefaultLUT[static_cast<std::size_t>(Types::End)] =
	{
		[](void* data, std::size_t index) { new(reinterpret_cast<int*>(data) + index)int(0); },
		[](void* data, std::size_t index) { new(reinterpret_cast<float*>(data) + index)float(0.0f); },
		[](void* data, std::size_t index) { new(reinterpret_cast<glm::vec4*>(data) + index)glm::vec4(0.0f); },
		[](void* data, std::size_t index) { new(reinterpret_cast<glm::mat4*>(data) + index)glm::mat4(0.0f); },
		[](void* data, std::size_t index) { new(reinterpret_cast<std::string*>(data) + index)std::string(); },
		[](void* data, std::size_t index) { new(reinterpret_cast<ScopePointer*>(data) + index)ScopePointer(nullptr); },
		[](void* data, std::size_t index) { new(reinterpret_cast<RTTIPointer*>(data) + index)RTTIPointer(nullptr); },
		[](void* data, std::size_t index) { new(reinterpret_cast<DatumPointer*>(data) + index)DatumPointer(nullptr); }
	};

	const Datum::ToStringFunctor Datum::ToStringLUT[static_cast<std::size_t>(Types::End)] =
	{
		[](void* data, std::size_t index) { return std::to_string(reinterpret_cast<int*>(data)[index]); },
		[](void* data, std::size_t index) { return std::to_string(reinterpret_cast<float*>(data)[index]); },
		[](void* data, std::size_t index) { return glm::to_string(reinterpret_cast<glm::vec4*>(data)[index]); },
		[](void* data, std::size_t index) { return glm::to_string(reinterpret_cast<glm::mat4*>(data)[index]); },
		[](void* data, std::size_t index) { return reinterpret_cast<std::string*>(data)[index]; },
		[](void* data, std::size_t index) { ScopePointer ptr = reinterpret_cast<ScopePointer*>(data)[index]; return ptr ? ptr->ToString() : "nullptr";  },
		[](void* data, std::size_t index) { RTTIPointer ptr = reinterpret_cast<RTTIPointer*>(data)[index]; return ptr ? ptr->ToString() : "nullptr";  },
		[](void* data, std::size_t index) { DatumPointer ptr = reinterpret_cast<DatumPointer*>(data)[index]; return ptr ? ptr->ToString() : "nullptr";  }
	};

	const Datum::FromStringFunctor Datum::FromStringLUT[static_cast<std::size_t>(Types::End)] =
	{
		[](std::string str, void* data, std::size_t index) { reinterpret_cast<int*>(data)[index] = std::stoi(str); },
		[](std::string str, void* data, std::size_t index) { reinterpret_cast<float*>(data)[index] = std::stof(str); },
		[](std::string str, void* data, std::size_t index)
		{
			float* vector = glm::value_ptr(reinterpret_cast<glm::vec4*>(data)[index]);
			sscanf_s(str.c_str(), "vec4(%f,%f,%f,%f)", &vector[0], &vector[1], &vector[2], &vector[3]);
		},
		[](std::string str, void* data, std::size_t index)
		{
			float* matrix = glm::value_ptr(reinterpret_cast<glm::mat4*>(data)[index]);
			sscanf_s(str.c_str(), "mat4x4((%f,%f,%f,%f), (%f,%f,%f,%f), (%f,%f,%f,%f), (%f,%f,%f,%f))",
						&matrix[0], &matrix[1], &matrix[2], &matrix[3],
						&matrix[4], &matrix[5], &matrix[6], &matrix[7],
						&matrix[8], &matrix[9], &matrix[10], &matrix[11],
						&matrix[12], &matrix[13], &matrix[14], &matrix[15]);
		},
		[](std::string str, void* data, std::size_t index) { reinterpret_cast<std::string*>(data)[index] = str; },
		[](std::string, void*, std::size_t) {},
		[](std::string, void*, std::size_t) {},
		[](std::string, void*, std::size_t) {}
	};
#pragma endregion Look Up Tables

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

				if (mType == Types::String)
				{
					for (std::size_t i = 0; i < mSize; ++i)
					{
						new(mData.stringPtr + i)std::string(rhs.mData.stringPtr[i]);
					}
				}
				else if (mData.voidPtr && rhs.mData.voidPtr)
				{
					std::size_t dataSize = TypeSizeLUT[static_cast<std::size_t>(mType)];
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
			if (mSize > 0)
			{
				Clear();
				ShrinkToFit();
			}

			mType = rhs.mType;
			mSize = rhs.mSize;
			mInternalStorage = rhs.mInternalStorage;
			mReserveFunctor = rhs.mReserveFunctor;

			if (rhs.mInternalStorage && rhs.mCapacity > 0)
			{
				Reserve(rhs.mCapacity);

				if (mType == Types::String)
				{
					for (std::size_t i = 0; i < mSize; ++i)
					{
						new(mData.stringPtr + i)std::string(rhs.mData.stringPtr[i]);
					}
				}
				else
				{
					std::size_t dataSize = TypeSizeLUT[static_cast<std::size_t>(mType)];
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

#pragma region Scalar/Initializer List Constructors
	Datum::Datum(const int rhs)
	{
		ScalarInitializationHelper(rhs);
	}

	Datum::Datum(const float rhs)
	{
		ScalarInitializationHelper(rhs);
	}

	Datum::Datum(const glm::vec4& rhs)
	{
		ScalarInitializationHelper(rhs);
	}

	Datum::Datum(const glm::mat4& rhs)
	{
		ScalarInitializationHelper(rhs);
	}

	Datum::Datum(const std::string& rhs)
	{
		ScalarInitializationHelper(rhs);
	}

	Datum::Datum(const ScopePointer& rhs)
	{
		ScalarInitializationHelper(rhs);
	}

	Datum::Datum(const RTTIPointer& rhs)
	{
		ScalarInitializationHelper(rhs);
	}

	Datum::Datum(const DatumPointer& rhs)
	{
		ScalarInitializationHelper(rhs);
	}

	Datum::Datum(const std::initializer_list<int> rhs)
	{
		ListInitializationHelper(rhs);
	}

	Datum::Datum(const std::initializer_list<float> rhs)
	{
		ListInitializationHelper(rhs);
	}

	Datum::Datum(const std::initializer_list<glm::vec4> rhs)
	{
		ListInitializationHelper(rhs);
	}

	Datum::Datum(const std::initializer_list<glm::mat4> rhs)
	{
		ListInitializationHelper(rhs);
	}

	Datum::Datum(const std::initializer_list<std::string> rhs)
	{
		ListInitializationHelper(rhs);
	}

	Datum::Datum(const std::initializer_list<ScopePointer> rhs)
	{
		ListInitializationHelper(rhs);
	}

	Datum::Datum(const std::initializer_list<RTTIPointer> rhs)
	{
		ListInitializationHelper(rhs);
	}

	Datum::Datum(const std::initializer_list<DatumPointer> rhs)
	{
		ListInitializationHelper(rhs);
	}
#pragma endregion Scalar/List Constructors

#pragma region Scalar/Initializer List Assignment
	Datum& Datum::operator=(const int rhs)
	{
		return ScalarInitializationHelper(rhs);
	}

	Datum& Datum::operator=(const float rhs)
	{
		return ScalarInitializationHelper(rhs);
	}

	Datum& Datum::operator=(const glm::vec4& rhs)
	{
		return ScalarInitializationHelper(rhs);
	}

	Datum& Datum::operator=(const glm::mat4& rhs)
	{
		return ScalarInitializationHelper(rhs);
	}

	Datum& Datum::operator=(const std::string& rhs)
	{
		return ScalarInitializationHelper(rhs);
	}

	Datum& Datum::operator=(const ScopePointer& rhs)
	{
		return ScalarInitializationHelper(rhs);
	}

	Datum& Datum::operator=(const RTTIPointer& rhs)
	{
		return ScalarInitializationHelper(rhs);
	}

	Datum& Datum::operator=(const DatumPointer& rhs)
	{
		return ScalarInitializationHelper(rhs);
	}

	Datum& Datum::operator=(const std::initializer_list<int> rhs)
	{
		return ListInitializationHelper(rhs);
	}

	Datum& Datum::operator=(const std::initializer_list<float> rhs)
	{
		return ListInitializationHelper(rhs);
	}

	Datum& Datum::operator=(const std::initializer_list<glm::vec4> rhs)
	{
		return ListInitializationHelper(rhs);
	}

	Datum& Datum::operator=(const std::initializer_list<glm::mat4> rhs)
	{
		return ListInitializationHelper(rhs);
	}

	Datum& Datum::operator=(const std::initializer_list<std::string> rhs)
	{
		return ListInitializationHelper(rhs);
	}

	Datum& Datum::operator=(const std::initializer_list<ScopePointer> rhs)
	{
		return ListInitializationHelper(rhs);
	}

	Datum& Datum::operator=(const std::initializer_list<RTTIPointer> rhs)
	{
		return ListInitializationHelper(rhs);
	}

	Datum& Datum::operator=(const std::initializer_list<DatumPointer> rhs)
	{
		return ListInitializationHelper(rhs);
	}
#pragma endregion Scalar/List Assignment
#pragma endregion Constructors, Destructor, Assignment

#pragma region Boolean Operators
	bool Datum::operator==(const Datum& rhs) const noexcept
	{
		if (this == &rhs)								return true;
		if (mType != rhs.mType || mSize != rhs.mSize)	return false;
		if (mType == Types::Unknown)					return true;
		
		return EqualityLUT[static_cast<std::size_t>(mType)](mData.voidPtr, rhs.mData.voidPtr, mSize);
	}

	bool Datum::operator!=(const Datum& rhs) const noexcept
	{
		return !(operator==(rhs));
	}

#pragma region Equals Scalar
	bool Datum::operator==(const int rhs) const noexcept
	{
		assert(mData.intPtr != nullptr);
		return mData.intPtr[0] == rhs;
	}

	bool Datum::operator==(const float rhs) const noexcept
	{
		assert(mData.floatPtr != nullptr);
		return mData.floatPtr[0] == rhs;
	}

	bool Datum::operator==(const glm::vec4& rhs) const noexcept
	{
		assert(mData.vectorPtr != nullptr);
		return mData.vectorPtr[0] == rhs;
	}

	bool Datum::operator==(const glm::mat4& rhs) const noexcept
	{
		assert(mData.matrixPtr != nullptr);
		return mData.matrixPtr[0] == rhs;
	}

	bool Datum::operator==(const std::string& rhs) const noexcept
	{
		assert(mData.stringPtr != nullptr);
		return mData.stringPtr[0] == rhs;
	}

	bool Datum::operator==(const ScopePointer& rhs) const noexcept
	{
		assert(mData.scopePtr != nullptr);
		return ((!mData.scopePtr[0] && !rhs) || (mData.scopePtr[0] && *mData.scopePtr[0] == *rhs));
	}

	bool Datum::operator==(const RTTIPointer& rhs) const noexcept
	{
		assert(mData.rttiPtr != nullptr);
		return ((!mData.rttiPtr[0] && !rhs) || (mData.rttiPtr[0] && mData.rttiPtr[0]->Equals(rhs)));
	}

	bool Datum::operator==(const DatumPointer& rhs) const noexcept
	{
		assert(mData.datumPtr != nullptr);
		return ((!mData.datumPtr[0] && !rhs) || (mData.datumPtr[0] && *mData.datumPtr[0] == *rhs));
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

	bool Datum::operator!=(const ScopePointer& rhs) const noexcept
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const RTTIPointer& rhs) const noexcept
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const DatumPointer& rhs) const noexcept
	{
		return !(operator==(rhs));
	}
#pragma endregion  Not Equals Scalar
#pragma endregion Boolean Operators

#pragma region Size and Capacity
	void Datum::Reserve(std::size_t capacity)
	{
		if (mType == Types::Unknown)	throw std::runtime_error("Data type unknown.");
		if (!mInternalStorage)			throw std::runtime_error("Cannot modify external storage.");

		if (capacity > mCapacity)
		{
			void* newMemory = realloc(mData.voidPtr, capacity * TypeSizeLUT[static_cast<std::size_t>(mType)]);

			assert(newMemory != nullptr);

			mData.voidPtr = newMemory;
			mCapacity = capacity;
		}
	}

	void Datum::Resize(std::size_t size)
	{
		if (mType == Types::Unknown)	throw std::runtime_error("Data type unknown.");
		if (!mInternalStorage)			throw std::runtime_error("Cannot modify external storage.");

		if (size > mSize)
		{
			Reserve(size);

			for (std::size_t i = mSize; i < size; ++i)
			{
				CreateDefaultLUT[static_cast<std::size_t>(mType)](mData.voidPtr, i);
			}
		}
		else if (mType == Types::String && size < mSize)
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
		if (mType == Types::Unknown)	throw std::runtime_error("Data type unknown.");
		if (!mInternalStorage)			throw std::runtime_error("Cannot modify external storage.");

		if (mSize == 0)
		{
			free(mData.voidPtr);
			mData.voidPtr = nullptr;
		}
		else if (mSize < mCapacity)
		{
			void* newMemory = realloc(mData.voidPtr, mSize * TypeSizeLUT[static_cast<std::size_t>(mType)]);

			assert(newMemory != nullptr);

			mData.voidPtr = newMemory;
		}

		mCapacity = mSize;
	}
#pragma endregion Size and Capacity

#pragma region Element Accessors
	Scope& Datum::operator[](std::size_t index)
	{
		if (mType != Types::Scope) throw std::runtime_error("Scope type expected.");
		return *Get<ScopePointer>(index);
	}

	const Scope& Datum::operator[](std::size_t index) const
	{
		if (mType != Types::Scope) throw std::runtime_error("Scope type expected.");
		return *Get<ScopePointer>(index);
	}
#pragma endregion Element Accessors

#pragma region Modifiers
	void Datum::PopBack()
	{
		if (mType == Types::Unknown)	throw std::runtime_error("Data type unknown.");
		if (!mInternalStorage)			throw std::runtime_error("Cannot modify external storage.");

		if (mSize > 0)
		{
			if (mType == Types::String)
			{
				mData.stringPtr[mSize - 1].~basic_string();
			}

			--mSize;
		}
	}

	void Datum::RemoveAt(const std::size_t index)
	{
		if (mType == Types::Unknown)	throw std::runtime_error("Data type unknown.");
		if (!mInternalStorage)			throw std::runtime_error("Cannot modify external storage.");
		if (index >= mSize)				throw std::out_of_range("Index out of bounds.");

		if (mType == Types::String)
		{
			mData.stringPtr[index].~basic_string();
		}

		const std::size_t size = TypeSizeLUT[static_cast<std::size_t>(mType)];
		memmove(&mData.bytePtr[index * size], &mData.bytePtr[(index * size) + size], size * (mSize - index));

		--mSize;
	}

	void Datum::Clear()
	{
		if (!mInternalStorage)
		{
			mData.voidPtr = nullptr;
		}
		else if (mType == Types::String)
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

#pragma region String Conversion
	std::string Datum::ToString(const std::size_t index) const
	{
		if (mType == Types::Unknown) throw std::runtime_error("Data type unknown.");
		if (index >= mSize)			 throw std::out_of_range("Index out of bounds.");

		return ToStringLUT[static_cast<std::size_t>(mType)](mData.voidPtr, index);
	}

	void Datum::SetFromString(const std::string& str, const std::size_t index)
	{
		if (mType == Types::Unknown) throw std::runtime_error("Data type unknown.");
		if (index >= mSize)			 throw std::out_of_range("Index out of bounds.");

		FromStringLUT[static_cast<std::size_t>(mType)](str, mData.voidPtr, index);
	}
#pragma endregion String Conversion

#pragma region Helper Methods
	template<typename T>
	Datum& Datum::ScalarInitializationHelper(const T& rhs)
	{
		if (mType == Types::Unknown)
		{
			mType = TypeOf<T>();
		}
		else if (mType != TypeOf<T>())
		{
			throw std::runtime_error("Mismatched types.");
		}

		if (mInternalStorage && mSize < 1)
		{
			Resize(1);
		}
		else if (mSize < 1)
		{
			throw std::runtime_error("External storage has insufficient memory.");
		}

		Set(rhs);		
		return *this;
	}

	template<typename T>
	Datum& Datum::ListInitializationHelper(const std::initializer_list<T> rhs)
	{
		if (mType == Types::Unknown)
		{
			mType = TypeOf<T>();
		}
		else if (mType != TypeOf<T>())
		{
			throw std::runtime_error("Mismatched types.");
		}

		if (mInternalStorage && mSize < rhs.size())
		{
			Resize(rhs.size());
		}
		else if (mSize < rhs.size())
		{
			throw std::runtime_error("External storage has insufficient memory.");
		}

		auto it = rhs.begin();
		for (std::size_t i = 0; i < rhs.size(); ++i, ++it)
		{
			Set(*it, i);
		}

		return *this;
	}
#pragma endregion Helper Methods
}
