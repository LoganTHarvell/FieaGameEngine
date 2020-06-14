#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "Attributed.h"

// Standard
#include <sstream>

// Third Party
#include <gsl/span>
#pragma endregion Includes


namespace Library
{
#pragma region Constructors, Destructor, Assignment
	Attributed::Attributed(const IdType typeId)
	{	
		(*this)["this"] = static_cast<RTTI*>(this);
		Populate(TypeManager::Instance()->Find(typeId));
		mNumPrescribed = mPairPtrs.Size();
	}

	Attributed::Attributed(const Attributed& rhs) : Scope(rhs), 
		mNumPrescribed(rhs.mNumPrescribed)
	{
		(*this)["this"] = static_cast<RTTI*>(this);
		UpdateExternalStorage(TypeManager::Instance()->Find(rhs.TypeIdInstance()));
	}

	Attributed& Attributed::operator=(const Attributed& rhs)
	{
		if (this == &rhs) return *this;
		
		Scope::operator=(rhs);
		mNumPrescribed = rhs.mNumPrescribed;

		(*this)["this"] = static_cast<RTTI*>(this);
		UpdateExternalStorage(TypeManager::Instance()->Find(rhs.TypeIdInstance()));

		return *this;
	}

	Attributed::Attributed(Attributed&& rhs) noexcept : Scope(std::move(rhs)), 
		mNumPrescribed(rhs.mNumPrescribed)
	{
		(*this)["this"] = static_cast<RTTI*>(this);
		UpdateExternalStorage(TypeManager::Instance()->Find(rhs.TypeIdInstance()));
	}

	Attributed& Attributed::operator=(Attributed&& rhs) noexcept
	{
		if (this == &rhs) return *this;

		Scope::operator=(std::move(rhs));
		mNumPrescribed = rhs.mNumPrescribed;

		(*this)["this"] = static_cast<RTTI*>(this);
		UpdateExternalStorage(TypeManager::Instance()->Find(rhs.TypeIdInstance()));

		return *this;
	}
#pragma endregion Constructors, Destructor, Assignment

#pragma region Boolean Operators
	bool Attributed::operator==(const Attributed& rhs) const noexcept
	{
		if (this == &rhs)								return true;
		if (mPairPtrs.Size() != rhs.mPairPtrs.Size())	return false;

		for (std::size_t i = 1; i < mPairPtrs.Size(); ++i)
		{
			try
			{
				const auto& pairPtr = mPairPtrs[i];

				const Data* rhsData = rhs.Find(pairPtr->first);
				if (!rhsData || pairPtr->second != *rhsData) return false;
			}
			catch (...)
			{
				return false;
			}
		}

		return true;
	}

	bool Attributed::operator!=(const Attributed& rhs) const noexcept
	{
		return !(operator==(rhs));
	}
#pragma endregion Boolean Operators

#pragma region Accessors
	bool Attributed::IsAttribute(const Key& key)
	{
		return Find(key) != nullptr;
	}

	bool Attributed::IsPrescribedAttribute(const Key& key)
	{
		for (std::size_t i = 0; i < mNumPrescribed; ++i)
		{
			if (mPairPtrs[i]->first == key) return true;
		}

		return false;
	}

	bool Attributed::IsAuxiliaryAttribute(const Key& key)
	{
		return !IsPrescribedAttribute(key);
	}

	void Attributed::ForEachPrescribed(const std::function<void(Attribute&)>& functor)
	{
		for (std::size_t i = 0; i < mNumPrescribed; ++i)
		{
			functor(*mPairPtrs[i]);
		}
	}

	void Attributed::ForEachPrescribed(const std::function<void(const Attribute&)>& functor) const
	{
		for (std::size_t i = 0; i < mNumPrescribed; ++i)
		{
			functor(*mPairPtrs[i]);
		}
	}

	void Attributed::ForEachAuxiliary(const std::function<void(Attribute&)>& functor)
	{
		for (std::size_t i = mNumPrescribed; i < mPairPtrs.Size(); ++i)
		{
			functor(*mPairPtrs[i]);
		}
	}

	void Attributed::ForEachAuxiliary(const std::function<void(const Attribute&)>& functor) const
	{
		for (std::size_t i = mNumPrescribed; i < mPairPtrs.Size(); ++i)
		{
			functor(*mPairPtrs[i]);
		}
	}
	
	Attributed::Data& Attributed::AppendAuxiliaryAttribute(const Key& key)
	{
		if (IsPrescribedAttribute(key))
		{
			throw std::runtime_error("Attribute is prescribed.");
		}

		return Append(key);
	}
#pragma endregion Accessors

#pragma region RTTI Overrides
	std::string Attributed::ToString() const
	{
		std::ostringstream oss;
		oss << "Attributed(";

		for (std::size_t pairIndex = 1; pairIndex < mPairPtrs.Size(); ++pairIndex)
		{
			const auto& pairPtr = mPairPtrs[pairIndex];

			oss << "'" << pairPtr->first << "':{";

			for (std::size_t dataIndex = 0; dataIndex < pairPtr->second.Size(); ++dataIndex)
			{
				oss << pairPtr->second.ToString(dataIndex);

				if (dataIndex < pairPtr->second.Size() - 1) oss << ",";
			}

			oss << "}";

			if (pairIndex < mPairPtrs.Size() - 1) oss << ",";
		}

		oss << ")";

		return oss.str();
	}

	bool Attributed::Equals(const RTTI* rhs) const
	{
		if (this == rhs)	return true;
		if (!rhs)			return false;

		const Attributed* rhsAttributedPtr = rhs->As<Attributed>();
		return rhsAttributedPtr ? operator==(*rhsAttributedPtr) : false;
	}
#pragma endregion RTTI Overrides

#pragma region Helper Methods
	void Attributed::Populate(const TypeManager::TypeInfo* typeInfo)
	{
		const auto* parentTypeInfo = TypeManager::Instance()->Find(typeInfo->ParentTypeId);
		if (parentTypeInfo) Populate(parentTypeInfo);

		mPairPtrs.Reserve(typeInfo->Signatures.Size() + mPairPtrs.Size());

		for (const auto& signature : typeInfo->Signatures)
		{
			Data& data = Append(signature.Key);

			if (!signature.IsInternal)
			{
				std::byte* address = reinterpret_cast<std::byte*>(this) + signature.Offset;
				data.SetStorage(signature.Type, gsl::span(address, signature.Size));
			}
			else
			{
				data.mType = signature.Type;
				data.Reserve(signature.Size);
			}
		}
	}

	std::size_t Attributed::UpdateExternalStorage(const TypeManager::TypeInfo* typeInfo)
	{
		std::size_t index = 1;

		const auto parentTypeInfo = TypeManager::Instance()->Find(typeInfo->ParentTypeId);
		if (parentTypeInfo) index = UpdateExternalStorage(parentTypeInfo);

		for (const auto& signature : typeInfo->Signatures)
		{
			if (!signature.IsInternal)
			{
				std::byte* address = reinterpret_cast<std::byte*>(this) + signature.Offset;
				mPairPtrs[index]->second.SetStorage(signature.Type, gsl::span(address, signature.Size));
			}

			++index;
		}

		return index;
	}
#pragma endregion Helper Methods
}