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
		mPairPtrs(capacity), mTable(std::max(Table::DefaultBucketCount, Math::FindNextPrime(capacity)))
	{
	}

	Scope::~Scope()
	{		
		Clear();
	}

	Scope::Scope(const Scope& rhs) :
		mPairPtrs(rhs.mPairPtrs.Size()), mTable(rhs.mTable.BucketCount())
	{
		for (const auto& tableEntryPtr : rhs.mPairPtrs)
		{
			if (tableEntryPtr->second.Type() == Types::Scope)
			{
				Data data;
				data.SetType(Types::Scope);
				
				for (std::size_t i = 0; i < tableEntryPtr->second.Size(); ++i)
				{
					mChildren.EmplaceBack(tableEntryPtr->second[i].Clone());
					mChildren.Back()->mParent = this;
					data.EmplaceBack<Scope*>(mChildren.Back());
				}

				mPairPtrs.EmplaceBack(&(*mTable.TryEmplace(tableEntryPtr->first, data).first));
			}
			else
			{
				mPairPtrs.EmplaceBack(&(*mTable.Emplace(*tableEntryPtr).first));
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
				Data data;
				data.SetType(Types::Scope);
				
				for (std::size_t i = 0; i < tableEntryPtr->second.Size(); ++i)
				{
					mChildren.EmplaceBack(tableEntryPtr->second[i].Clone());
					mChildren.Back()->mParent = this;
					data.EmplaceBack<Scope*>(mChildren.Back());
				}

				mPairPtrs.EmplaceBack(&(*mTable.TryEmplace(tableEntryPtr->first, data).first));
			}
			else
			{
				mPairPtrs.EmplaceBack(&(*mTable.Emplace(*tableEntryPtr).first));
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

	Scope::Scope(std::initializer_list<Attribute> rhs, const std::size_t capacity) :
		mPairPtrs(capacity), mTable(std::max(Table::DefaultBucketCount, Math::FindNextPrime(capacity)))
	{
		for (const auto& tableEntry : rhs)
		{
			if (Scope::Find(tableEntry.first) != nullptr)
			{
				throw std::runtime_error("Duplicate names found in the initializer list.");
			}

			if (tableEntry.second.Type() == Types::Scope)
			{
				Data data;
				data.SetType(Types::Scope);

				for (std::size_t i = 0; i < tableEntry.second.Size(); ++i)
				{
					mChildren.EmplaceBack(tableEntry.second[i].Clone());
					mChildren.Back()->mParent = this;
					data.EmplaceBack<Scope*>(mChildren.Back());

					auto [it, isNew] = mTable.TryEmplace(tableEntry.first, Data(mChildren.Back()));
					if (isNew) mPairPtrs.EmplaceBack(&(*it));
					else it->second.EmplaceBack<Scope*>(mChildren.Back());
				}
			}
			else
			{
				mPairPtrs.EmplaceBack(&(*mTable.Emplace(tableEntry).first));
			}
		}
	}

	Scope& Scope::operator=(std::initializer_list<Attribute> rhs)
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
				Data data;
				data.SetType(Types::Scope);

				for (std::size_t i = 0; i < tableEntry.second.Size(); ++i)
				{
					mChildren.EmplaceBack(tableEntry.second[i].Clone());
					mChildren.Back()->mParent = this;
					data.EmplaceBack<Scope*>(mChildren.Back());

					auto [it, isNew] = mTable.TryEmplace(tableEntry.first, Data(mChildren.Back()));
					if (isNew) mPairPtrs.EmplaceBack(&(*it));
					else it->second.EmplaceBack<Scope*>(mChildren.Back());
				}
			}
			else
			{
				mPairPtrs.EmplaceBack(&(*mTable.Emplace(tableEntry).first));
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
			const Data* rhsData = rhs.Find(pairPtr->first);
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
	Scope::Data& Scope::operator[](const Key& key)
	{
		return Append(key);
	}

	const Scope::Data& Scope::operator[](const Key& key) const
	{
		const Data* entry = Find(key);
		if (entry == nullptr) throw std::runtime_error("Name not found.");

		return *entry;
	}

	Scope::Data* Scope::Find(const Key& key)
	{
		Table::Iterator it = mTable.Find(key);
		return it != mTable.end() ? &it->second : nullptr;
	}

	const Scope::Data* Scope::Find(const Key& key) const
	{
		return const_cast<Scope*>(this)->Find(key);
	}

	std::pair<Scope::Data*, std::size_t> Scope::FindScope(const Scope& scope)
	{
		for (auto& pairPtr : mPairPtrs)
		{
			if (pairPtr->second.Type() == Types::Scope)
			{
				for (std::size_t i = 0; i < pairPtr->second.Size(); ++i)
				{
					if (&scope == pairPtr->second.Get<Data::ScopePointer>(i))
					{
						return { &pairPtr->second, i };
					}
				}
			}
		}

		return { nullptr, 0 };
	}

	std::pair<const Scope::Data*, std::size_t> Scope::FindScope(const Scope& scope) const
	{
		for (auto& pairPtr : mPairPtrs)
		{
			if (pairPtr->second.Type() == Types::Scope)
			{
				for (std::size_t i = 0; i < pairPtr->second.Size(); ++i)
				{
					if (&scope == pairPtr->second.Get<Data::ScopePointer>(i))
					{
						return { &pairPtr->second, i };
					}
				}
			}
		}

		return { nullptr, 0 };
	}

	Scope::Data* Scope::Search(const Key& key, Scope** scopePtrOut)
	{
		Data* result = nullptr;
		Scope* parent = this;

		while (parent != nullptr)
		{
			result = parent->Find(key);
			if (result) break;
			
			parent = parent->mParent;
		}

		if (scopePtrOut) *scopePtrOut = parent;
		return result;
	}

	const Scope::Data* Scope::Search(const Key& key, const Scope** scopePtrOut) const
	{
		return const_cast<Scope*>(this)->Search(key, const_cast<Scope**>(scopePtrOut));
	}

	Scope::Data* Scope::SearchChildren(const Key& key, Scope** scopePtrOut)
	{
		return SearchChildrenHelper({ this }, key, scopePtrOut);
	}

	const Scope::Data* Scope::SearchChildren(const Key& key, const Scope** scopePtrOut) const
	{
		const Vector queue = { const_cast<Scope*>(this) };
		return SearchChildrenHelper(queue, key, const_cast<Scope**>(scopePtrOut));
	}

	void Scope::ForEachAttribute(const std::function<void(Attribute&)>& functor)
	{
		for (auto& pair : mPairPtrs)
		{
			functor(*pair);
		}
	}

	void Scope::ForEachAttribute(const std::function<void(const Attribute&)>& functor) const
	{
		for (const auto& pair : mPairPtrs)
		{
			functor(*pair);
		}
	}
#pragma endregion Accessors

#pragma region Modifiers
	Scope::Data& Scope::Append(const Key& key)
	{
		if (key.empty()) throw std::runtime_error("Name cannot be empty.");

		auto [it, isNew] = mTable.TryEmplace(key, Data());
		if (isNew) mPairPtrs.EmplaceBack(&(*it));

		return it->second;
	}

	Scope& Scope::AppendScope(const Key& key, const std::size_t capacity)
	{
		if (key.empty()) throw std::runtime_error("Name cannot be empty.");

		Data* data = Find(key);

		if (data && data->Type() != Types::Unknown && data->Type() != Types::Scope)
		{
			throw std::runtime_error("Table entry already exists with a non-Scope type.");
		}

		Scope* child = new Scope(std::max(Table::DefaultBucketCount, Math::FindNextPrime(capacity)));
		child->mParent = this;
		mChildren.EmplaceBack(child);

		if (data)
		{
			data->EmplaceBack<Scope*>(child);
		}
		else
		{
			mPairPtrs.EmplaceBack(&(*mTable.TryEmplace(key, Data(mChildren.Back())).first));
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

	Scope& Scope::Adopt(Scope& child, const Key& key)
	{
		if (this == &child)			throw std::runtime_error("Cannot adopt self.");
		if (IsAncestorOf(child))	throw std::runtime_error("Cannot adopt descendant.");
		if (key.empty())			throw std::runtime_error("Name cannot be empty.");

		Data* data = Find(key);

		if (data && data->Type() != Types::Unknown && data->Type() != Types::Scope)
		{
			throw std::runtime_error("Table entry already exists with a non-Scope type.");
		}
	
		Scope* orphan = child.mParent ? child.mParent->Orphan(child) : &child;
		orphan->mParent = this;
		mChildren.EmplaceBack(orphan);

		if (data)
		{
			data->EmplaceBack<Scope*>(orphan);
		}
		else
		{
			mPairPtrs.EmplaceBack(&(*mTable.TryEmplace(key, Data(mChildren.Back())).first));
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
	Scope::Data* Scope::SearchChildrenHelper(const Vector<Scope*>& queue, const Key& key, Scope** scopePtrOut)
	{
		Vector<Scope*> newQueue;

		for (auto& scopePtr : queue)
		{
			Data* result = scopePtr->Find(key);

			if (result)
			{
				if (scopePtrOut) *scopePtrOut = scopePtr;
				return result;
			}

			newQueue.Reserve(scopePtr->mChildren.Size());

			for (auto& child : scopePtr->mChildren)
			{
				newQueue.EmplaceBack(child);
			}
		}

		if (newQueue.Size() > 0) return SearchChildrenHelper(newQueue, key, scopePtrOut);

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