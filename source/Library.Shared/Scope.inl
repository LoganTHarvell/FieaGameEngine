#pragma once

// Header
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
	inline Scope* Scope::GetParent() const
	{
		return mParent;
	}

	inline bool Scope::IsAncestorOf(const Scope& scope) const
	{
		Scope* parent = scope.mParent;

		while (parent != nullptr)
		{
			if (parent == this) return true;

			parent = parent->mParent;
		}

		return false;
	}

	inline bool Scope::IsDescendentOf(const Scope& scope) const
	{
		return scope.IsAncestorOf(*this);
	}

	inline Scope::Data& Scope::operator[](const std::size_t index)
	{
		return mPairPtrs[index]->second;
	}

	inline const Scope::Data& Scope::operator[](const std::size_t index) const
	{
		return mPairPtrs[index]->second;
	}

	inline const std::string* Scope::FindName(const std::size_t index) const
	{
		return index < mPairPtrs.Size() ? &mPairPtrs[index]->first : nullptr;
	}
#pragma endregion Accessors
}