#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Standard
#include <sstream>

// Header
#include "Scope.h"

// First Party
#include "Utility.h"
#pragma endregion Includes

namespace Library
{
#pragma region Constructors, Destructor, Assignment
	Scope::Scope(const std::size_t capacity) :
		mTable(std::max(Table::DefaultBucketCount, Math::FindNextPrime(capacity))), mPairPtrs(capacity)
	{
	}

	Scope::~Scope()
	{		
		Clear();
	}

	Scope::Scope(const Scope& rhs) :
		mTable(rhs.mTable.BucketCount()), mPairPtrs(rhs.mPairPtrs.Size())
	{
		for (auto& tableEntryPtr : rhs.mPairPtrs)
		{
			if (tableEntryPtr->second.Type() == Types::Scope)
			{
				DataType data;
				data.SetType(Types::Scope);
				
				for (std::size_t i = 0; i < tableEntryPtr->second.Size(); ++i)
				{
					mChildren.PushBack(tableEntryPtr->second[i].Clone());
					mChildren.Back()->mParent = this;
					data.PushBack(mChildren.Back());
				}

				mPairPtrs.PushBack(&(*mTable.Insert({ tableEntryPtr->first, data }).first));
			}
			else
			{
				mPairPtrs.PushBack(&(*mTable.Insert(*tableEntryPtr).first));
			}
		}
	}

	Scope& Scope::operator=(const Scope& rhs)
	{
		Clear();

		mTable = Table(rhs.mTable.BucketCount());
		
		mPairPtrs.ShrinkToFit();
		mPairPtrs.Reserve(rhs.mPairPtrs.Size());

		for (auto& tableEntryPtr : rhs.mPairPtrs)
		{
			if (tableEntryPtr->second.Type() == Types::Scope)
			{
				DataType data;
				data.SetType(Types::Scope);
				
				for (std::size_t i = 0; i < tableEntryPtr->second.Size(); ++i)
				{
					mChildren.PushBack(tableEntryPtr->second[i].Clone());
					mChildren.Back()->mParent = this;
					data.PushBack(mChildren.Back());
				}

				mPairPtrs.PushBack(&(*mTable.Insert({ tableEntryPtr->first, data }).first));
			}
			else
			{
				mPairPtrs.PushBack(&(*mTable.Insert(*tableEntryPtr).first));
			}
		}

		return *this;
	}

	Scope::Scope(Scope&& rhs) noexcept :
		mParent(rhs.mParent), mTable(std::move(rhs.mTable)), mPairPtrs(std::move(rhs.mPairPtrs)), mChildren(std::move(rhs.mChildren))
	{
		for (auto& child : mChildren)
		{
			child->mParent = this;
		}

		if (rhs.mParent)
		{
			*rhs.mParent->mChildren.Find(&rhs) = this;

			auto [data, index] = rhs.mParent->FindScope(rhs);
			assert(data != nullptr);
			data->Set(this, index);

			rhs.mParent = nullptr;
		}
	}

	Scope& Scope::operator=(Scope&& rhs) noexcept
	{
		if (mParent)
		{
			mParent->Orphan(*this);
		}

		Clear();

		mParent = rhs.mParent;
		mTable = std::move(rhs.mTable);
		mPairPtrs = std::move(rhs.mPairPtrs);
		mChildren = std::move(rhs.mChildren);

		for (auto& child : mChildren)
		{
			child->mParent = this;
		}

		if (rhs.mParent)
		{
			*rhs.mParent->mChildren.Find(&rhs) = this;

			auto [data, index] = rhs.mParent->FindScope(rhs);
			assert(data != nullptr);
			data->Set(this, index);

			rhs.mParent = nullptr;
		}

		return *this;
	}

	Scope::Scope(const std::initializer_list<Attribute> rhs, const std::size_t capacity) :
		mTable(std::max(Table::DefaultBucketCount, Math::FindNextPrime(capacity))), mPairPtrs(capacity)
	{
		for (auto& tableEntry : rhs)
		{
			if (Find(tableEntry.first) != nullptr)
			{
				throw std::runtime_error("Duplicate names found in the initializer list.");
			}

			if (tableEntry.second.Type() == Types::Scope)
			{
				DataType data;
				data.SetType(Types::Scope);

				for (std::size_t i = 0; i < tableEntry.second.Size(); ++i)
				{
					mChildren.PushBack(tableEntry.second[i].Clone());
					mChildren.Back()->mParent = this;
					data.PushBack(mChildren.Back());

					auto [it, isNew] = mTable.Insert({ tableEntry.first, DataType(mChildren.Back()) });
					if (isNew) mPairPtrs.PushBack(&(*it));
					else it->second.PushBack(mChildren.Back());
				}
			}
			else
			{
				mPairPtrs.PushBack(&(*mTable.Insert(tableEntry).first));
			}
		}
	}

	Scope& Scope::operator=(const std::initializer_list<Attribute> rhs)
	{
		Clear();
		
		mTable = Table(Math::FindNextPrime(rhs.size()));

		mPairPtrs.ShrinkToFit();
		mPairPtrs.Reserve(rhs.size());

		for (auto& tableEntry : rhs)
		{
			if (Find(tableEntry.first) != nullptr)
			{
				throw std::runtime_error("Duplicate names found in the initializer list.");
			}

			if (tableEntry.second.Type() == Types::Scope)
			{
				DataType data;
				data.SetType(Types::Scope);

				for (std::size_t i = 0; i < tableEntry.second.Size(); ++i)
				{
					mChildren.PushBack(tableEntry.second[i].Clone());
					mChildren.Back()->mParent = this;
					data.PushBack(mChildren.Back());

					auto [it, isNew] = mTable.Insert({ tableEntry.first, DataType(mChildren.Back()) });
					if (isNew) mPairPtrs.PushBack(&(*it));
					else it->second.PushBack(mChildren.Back());
				}
			}
			else
			{
				mPairPtrs.PushBack(&(*mTable.Insert(tableEntry).first));
			}
		}

		return *this;
	}

	gsl::owner<Scope*> Scope::Clone() const
	{
		return new Scope(*this);
	}
#pragma endregion Constructors, Destructor, Assignment

#pragma region Boolean Operators
	bool Scope::operator==(const Scope& rhs) const noexcept
	{
		if (this == &rhs)								return true;
		if (mPairPtrs.Size() != rhs.mPairPtrs.Size())	return false;

		for (const auto& pairPtr : mPairPtrs)
		{
			const DataType* rhsData = rhs.Find(pairPtr->first);
			if (!rhsData || pairPtr->second != *rhsData) return false;
		}

		return true;
	}

	bool Scope::operator!=(const Scope& rhs) const noexcept
	{
		return !(operator==(rhs));
	}
#pragma endregion Boolean Operators

#pragma region Size and Capacity
	void Scope::Reserve(const std::size_t capacity)
	{		
		if (Math::FindNextPrime(capacity) > mTable.BucketCount())
		{
			Scope newScope = Scope(capacity);

			for (const auto& pairPtr : mPairPtrs)
			{
				newScope.Append(pairPtr->first) = std::move(pairPtr->second);
			}

			*this = std::move(newScope);
		}
		else
		{
			mPairPtrs.Reserve(capacity);
		}
	}

	void Scope::ShrinkToFit()
	{
		if (Math::FindNextPrime(mPairPtrs.Size()) < mTable.BucketCount())
		{
			Scope tmp = Scope(mPairPtrs.Size());

			for (const auto& pairPtr : mPairPtrs)
			{
				tmp.Append(pairPtr->first) = std::move(pairPtr->second);
			}

			*this = std::move(tmp);
		}
		else
		{
			mPairPtrs.ShrinkToFit();
		}
	}
#pragma endregion Size and Capacity

#pragma region Accessors
	Scope::DataType& Scope::operator[](const KeyType& name)
	{
		return Append(name);
	}

	const Scope::DataType& Scope::operator[](const KeyType& name) const
	{
		const DataType* entry = Find(name);
		if (entry == nullptr) throw std::runtime_error("Name not found.");

		return *entry;
	}

	Scope::DataType* Scope::Find(const KeyType& name)
	{
		Table::Iterator it = mTable.Find(name);
		return it != mTable.end() ? &it->second : nullptr;
	}

	const Scope::DataType* Scope::Find(const KeyType& name) const
	{
		Table::ConstIterator it = mTable.Find(name);
		return it != mTable.end() ? &it->second : nullptr;
	}

	std::pair<Scope::DataType*, std::size_t> Scope::FindScope(const Scope& scope)
	{
		for (auto& pairPtr : mPairPtrs)
		{
			if (pairPtr->second.Type() == Types::Scope)
			{
				for (std::size_t i = 0; i < pairPtr->second.Size(); ++i)
				{
					if (&scope == pairPtr->second.Get<DataType::ScopePointer>(i))
					{
						return { &pairPtr->second, i };
					}
				}
			}
		}

		return { nullptr, 0 };
	}

	std::pair<const Scope::DataType*, std::size_t> Scope::FindScope(const Scope& scope) const
	{
		for (auto& pairPtr : mPairPtrs)
		{
			if (pairPtr->second.Type() == Types::Scope)
			{
				for (std::size_t i = 0; i < pairPtr->second.Size(); ++i)
				{
					if (&scope == pairPtr->second.Get<DataType::ScopePointer>(i))
					{
						return { &pairPtr->second, i };
					}
				}
			}
		}

		return { nullptr, 0 };
	}

	Scope::DataType* Scope::Search(const KeyType& name, Scope** scopePtrOut)
	{
		DataType* result = Find(name);

		if (result)
		{
			if (scopePtrOut) *scopePtrOut = this;
			return result;
		}

		if (mParent) return mParent->Search(name, scopePtrOut);
		
		if (scopePtrOut) *scopePtrOut = nullptr;
		return nullptr;
	}

	const Scope::DataType* Scope::Search(const KeyType& name, const Scope** scopePtrOut) const
	{
		const DataType* result = Find(name);

		if (result)
		{
			if (scopePtrOut) *scopePtrOut = this;
			return result;
		}

		if (mParent) return mParent->Search(name, scopePtrOut);
		
		if (scopePtrOut) *scopePtrOut = nullptr;
		return nullptr;
	}

	Scope::DataType* Scope::SearchChildren(const KeyType& name, Scope** scopePtrOut)
	{
		Vector queue = { this };
		return SearchChildrenHelper(queue, name, scopePtrOut);
	}

	const Scope::DataType* Scope::SearchChildren(const KeyType& name, const Scope** scopePtrOut) const
	{
		Scope* nonConstThis = const_cast<Scope*>(this);
		Vector queue = { nonConstThis };
		return nonConstThis->SearchChildrenHelper(queue, name, const_cast<Scope**>(scopePtrOut));
	}

	void Scope::ForEachAttribute(std::function<void(Attribute&)> functor)
	{
		for (auto& pair : mPairPtrs)
		{
			functor(*pair);
		}
	}

	void Scope::ForEachAttribute(std::function<void(const Attribute&)> functor) const
	{
		for (const auto& pair : mPairPtrs)
		{
			functor(*pair);
		}
	}
#pragma endregion Accessors

#pragma region Modifiers
	Scope::DataType& Scope::Append(const KeyType& name)
	{
		if (name.empty()) throw std::runtime_error("Name cannot be empty.");

		auto [it, isNew] = mTable.Insert({ name, DataType() });
		if (isNew) mPairPtrs.PushBack(&(*it));

		return it->second;
	}

	Scope& Scope::AppendScope(const KeyType& name, const std::size_t capacity)
	{
		if (name.empty()) throw std::runtime_error("Name cannot be empty.");

		DataType* data = Find(name);

		if (data && data->Type() != Types::Unknown && data->Type() != Types::Scope)
		{
			throw std::runtime_error("Table entry already exists with a non-Scope type.");
		}

		Scope* child = new Scope(std::max(Table::DefaultBucketCount, Math::FindNextPrime(capacity)));
		child->mParent = this;
		mChildren.PushBack(child);

		if (data)
		{
			data->PushBack(child);
		}
		else
		{
			mPairPtrs.PushBack(&(*mTable.Insert({ name, DataType(mChildren.Back()) }).first));
		}

		return *child;
	}

	gsl::owner<Scope*> Scope::Orphan(Scope& child)
	{
		auto [data, index] = FindScope(child);
		
		if (data == nullptr) throw std::runtime_error("Child Scope not found.");

		data->RemoveAt(index);
		mChildren.Remove(&child);
		child.mParent = nullptr;

		return &child;
	}

	Scope& Scope::Adopt(Scope& child, const KeyType& name)
	{
		if (this == &child)			throw std::runtime_error("Cannot adopt self.");
		if (IsAncestorOf(child))	throw std::runtime_error("Cannot adopt descendant.");
		if (name.empty())			throw std::runtime_error("Name cannot be empty.");

		DataType* data = Find(name);

		if (data && data->Type() != Types::Unknown && data->Type() != Types::Scope)
		{
			throw std::runtime_error("Table entry already exists with a non-Scope type.");
		}
	
		Scope* orphan = child.mParent ? child.mParent->Orphan(child) : &child;
		orphan->mParent = this;
		mChildren.PushBack(orphan);

		if (data)
		{
			data->PushBack(orphan);
		}
		else
		{
			mPairPtrs.PushBack(&(*mTable.Insert({ name, DataType(mChildren.Back()) }).first));
		}

		return *mChildren.Back();
	}

	void Scope::Clear()
	{
		mTable.Clear();
		mPairPtrs.Clear();

		for (auto& child : mChildren)
		{
			if (child != nullptr)
			{
				child->mParent = nullptr;
				delete child;
				child = nullptr;
			}
		}

		mChildren.Clear();
	}
#pragma endregion Modifiers

#pragma region Helper Methods
	Scope::DataType* Scope::SearchChildrenHelper(const Vector<Scope*>& queue, const KeyType& name, Scope** scopePtrOut)
	{
		Vector<Scope*> newQueue;

		for (auto& scopePtr : queue)
		{
			DataType* result = scopePtr->Find(name);

			if (result)
			{
				if (scopePtrOut) *scopePtrOut = scopePtr;
				return result;
			}

			newQueue.Reserve(scopePtr->mChildren.Size());

			for (auto& child : scopePtr->mChildren)
			{
				newQueue.PushBack(child);
			}
		}

		if (newQueue.Size() > 0) return SearchChildrenHelper(newQueue, name, scopePtrOut);

		if (scopePtrOut) *scopePtrOut = nullptr;
		return nullptr;
	}
#pragma endregion Helper Methods
	
#pragma region RTTI Overrides
	std::string Scope::ToString() const
	{
		std::ostringstream oss;
		oss << "Scope(";

		for (std::size_t pairIndex = 0; pairIndex < mPairPtrs.Size(); ++pairIndex)
		{
			const auto& pairPtr = mPairPtrs[pairIndex];

			oss << "'" << pairPtr->first << "':{ ";

			for (std::size_t dataIndex = 0; dataIndex < pairPtr->second.Size(); ++dataIndex)
			{
				oss << pairPtr->second.ToString(dataIndex);

				if (dataIndex < pairPtr->second.Size() - 1) oss << ",";
			}
			
			oss << " }";

			if (pairIndex < pairPtr->second.Size() - 1) oss << ",";
		}

		oss << ")";

		return oss.str();
	}

	bool Scope::Equals(const RTTI* rhs) const
	{
		if (this == rhs)	return true;
		if (!rhs)			return false;

		const Scope* rhsScopePtr = rhs->As<Scope>();
		return rhsScopePtr ? operator==(*rhsScopePtr) : false;
	}
#pragma endregion RTTI Overrides
}