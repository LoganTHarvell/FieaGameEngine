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
	Attributed::Attributed(const RTTI::IdType typeId)
	{	
		(*this)["this"] = static_cast<RTTI*>(this);
		Populate(TypeManager::Instance()->Find(typeId));
		mNumPrescribed = mPairPtrs.Size();
	}

	Attributed::Attributed(const Attributed& rhs) : Scope(rhs)
	{
		(*this)["this"] = static_cast<RTTI*>(this);
		UpdateExternalStorage(TypeManager::Instance()->Find(rhs.TypeIdInstance()));
	}

	Attributed& Attributed::operator=(const Attributed& rhs)
	{
		Scope::operator=(rhs);

		(*this)["this"] = static_cast<RTTI*>(this);
		UpdateExternalStorage(TypeManager::Instance()->Find(rhs.TypeIdInstance()));

		return *this;
	}

	Attributed::Attributed(Attributed&& rhs) noexcept : Scope(std::move(rhs))
	{
		(*this)["this"] = static_cast<RTTI*>(this);
		UpdateExternalStorage(TypeManager::Instance()->Find(rhs.TypeIdInstance()));
	}

	Attributed& Attributed::operator=(Attributed&& rhs) noexcept
	{
		Scope::operator=(std::move(rhs));

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
			const auto& pairPtr = mPairPtrs[i];

			const DataType* rhsData = rhs.Find(pairPtr->first);
			if (!rhsData || pairPtr->second != *rhsData) return false;
		}

		return true;
	}

	bool Attributed::operator!=(const Attributed& rhs) const noexcept
	{
		return !(operator==(rhs));
	}
#pragma endregion Boolean Operators

#pragma region Accessors
	bool Attributed::IsAttribute(const NameType& name)
	{
		return Find(name) != nullptr;
	}

	bool Attributed::IsPrescribedAttribute(const NameType& name)
	{
		for (std::size_t i = 0; i < mNumPrescribed; ++i)
		{
			if (mPairPtrs[i]->first == name) return true;
		}

		return false;
	}

	bool Attributed::IsAuxiliaryAttribute(const NameType& name)
	{
		return !IsPrescribedAttribute(name);
	}

	void Attributed::ForEachPrescribed(std::function<void(Attribute&)> functor)
	{
		for (std::size_t i = 0; i < mNumPrescribed; ++i)
		{
			functor(*mPairPtrs[i]);
		}
	}

	void Attributed::ForEachPrescribed(std::function<void(const Attribute&)> functor) const
	{
		for (std::size_t i = 0; i < mNumPrescribed; ++i)
		{
			functor(*mPairPtrs[i]);
		}
	}

	void Attributed::ForEachAuxiliary(std::function<void(Attribute&)> functor)
	{
		for (std::size_t i = mNumPrescribed; i < mPairPtrs.Size(); ++i)
		{
			functor(*mPairPtrs[i]);
		}
	}

	void Attributed::ForEachAuxiliary(std::function<void(const Attribute&)> functor) const
	{
		for (std::size_t i = mNumPrescribed; i < mPairPtrs.Size(); ++i)
		{
			functor(*mPairPtrs[i]);
		}
	}
	
	Attributed::DataType& Attributed::AppendAuxiliaryAttribute(const NameType& name)
	{
		if (IsPrescribedAttribute(name))
		{
			throw std::runtime_error("Attribute is prescribed.");
		}

		return Append(name);
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
		auto parentTypeInfo = TypeManager::Instance()->Find(typeInfo->parentTypeId);
		if (parentTypeInfo) Populate(parentTypeInfo);

		mPairPtrs.Reserve(typeInfo->signatures.Size() + mPairPtrs.Size());

		for (const auto& signature : typeInfo->signatures)
		{
			DataType& data = Append(signature.Name);

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

		auto parentTypeInfo = TypeManager::Instance()->Find(typeInfo->parentTypeId);
		if (parentTypeInfo) index = UpdateExternalStorage(parentTypeInfo);

		for (const auto& signature : typeInfo->signatures)
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