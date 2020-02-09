#include "pch.h"

#include "Scope.h"

#include "Utility.h"

namespace Library
{
#pragma region Constructors, Assignment
	Scope::Scope(const std::size_t capacity) :
		mTable(TableType(Math::FindNextPrime(capacity))), mPairPtrs(capacity)
	{
	}

	Scope::Scope(const std::initializer_list<TableEntryType> rhs, const std::size_t capacity) :
		mTable(TableType(Math::FindNextPrime(capacity))), mPairPtrs(capacity)
	{
		for (auto& value : rhs)
		{
			mPairPtrs.PushBack(&(*(mTable.Insert(value).first)));
		}
	}

	Scope& Scope::operator=(const std::initializer_list<TableEntryType> rhs)
	{
		mTable = rhs;
		
		Vector<TableEntryType*> tmp(mTable.Size());
		for (auto& value : mTable)
		{
			tmp.PushBack(&value);
		}

		mPairPtrs = tmp;

		return *this;
	}
#pragma endregion Constructors, Assignment

#pragma region Boolean Operators
	bool Scope::operator==(const Scope& rhs) const noexcept
	{
		if (this == &rhs)								return true;
		if (mPairPtrs.Size() != rhs.mPairPtrs.Size())	return false;

		for (std::size_t i = 0; i < mPairPtrs.Size(); ++i)
		{
			if (*mPairPtrs[i] != *rhs.mPairPtrs[i]) return false;
		}

		return true;
	}

	bool Scope::operator!=(const Scope& rhs) const noexcept
	{
		return !(operator==(rhs));
	}
#pragma endregion Boolean Operators

#pragma region Element Accessors
	Scope* Scope::GetParent()
	{
		return mParent;
	}

	const Scope* Scope::GetParent() const
	{
		return mParent;
	}

	Scope::DataType& Scope::operator[](const NameType name)
	{
		return Append(name);
	}

	Scope::DataType& Scope::operator[](const std::size_t index)
	{
		return mPairPtrs[index]->second;
	}

	Scope::DataType* Scope::Find(const NameType name)
	{
		TableType::Iterator it = mTable.Find(name);
		return it != mTable.end() ? &(it->second) : nullptr;
	}

	const Scope::DataType* Scope::Find(const NameType name) const
	{
		TableType::ConstIterator it = mTable.Find(name);
		return it != mTable.end() ? &(it->second) : nullptr;
	}

	const std::string& Scope::FindName(const std::size_t index) const
	{
		return mPairPtrs[index]->first;
	}
#pragma endregion Element Accessors

#pragma region Modifiers
	Scope::DataType& Scope::Append(const NameType name)
	{
		auto [it, isNew] = mTable.Insert({ name, DataType() });
		if (isNew) mPairPtrs.PushBack(&(*it));

		return it->second;
	}
#pragma endregion Modifiers
}