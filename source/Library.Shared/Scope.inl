#pragma once

#include "Scope.h"

namespace Library
{
#pragma region Size and Capacity
	inline std::size_t Scope::Size() const
	{
		return mTable.Size();
	}

	inline bool Scope::IsEmpty() const
	{
		return mTable.IsEmpty();
	}

	inline std::size_t Scope::Capacity() const
	{
		return mPairPtrs.Capacity();
	}
#pragma endregion Size and Capacity

#pragma region Accessors
	inline Scope* Scope::GetParent()
	{
		return mParent;
	}

	inline const Scope* Scope::GetParent() const
	{
		return mParent;
	}

	inline Scope::DataType& Scope::operator[](const std::size_t index)
	{
		return mPairPtrs[index]->second;
	}

	inline const Scope::DataType& Scope::operator[](const std::size_t index) const
	{
		return mPairPtrs[index]->second;
	}

	inline const std::string* Scope::FindName(const std::size_t index) const
	{
		return index < mPairPtrs.Size() ? &mPairPtrs[index]->first : nullptr;
	}
#pragma endregion Accessors
}