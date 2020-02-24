#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "Attributed.h"

// Third Party
#include <gsl/span>
#pragma endregion Includes


namespace Library
{
#pragma region Constructors, Destructor, Assignment
	Attributed::Attributed(const RTTI::IdType typeId)
	{	
		(*this)["this"] = this->As<RTTI>();
		Populate(TypeManager::Instance()->Find(typeId));
		mPrescribedAttributes = mPairPtrs;
	}

	Attributed::Attributed(const Attributed& rhs) : Scope(rhs), mPrescribedAttributes(rhs.mPrescribedAttributes)
	{
		(*this)["this"] = this->As<RTTI>();
		UpdateExternalStorage(TypeManager::Instance()->Find(rhs.TypeIdInstance()));
	}

	Attributed& Attributed::operator=(const Attributed& rhs)
	{
		Scope::operator=(rhs);

		mPrescribedAttributes = rhs.mPrescribedAttributes;

		(*this)["this"] = this->As<RTTI>();
		UpdateExternalStorage(TypeManager::Instance()->Find(rhs.TypeIdInstance()));

		return *this;
	}

	Attributed::Attributed(Attributed&& rhs) noexcept : Scope(std::move(rhs)), mPrescribedAttributes(std::move(rhs.mPrescribedAttributes))
	{
		(*this)["this"] = this->As<RTTI>();
		UpdateExternalStorage(TypeManager::Instance()->Find(rhs.TypeIdInstance()));
	}

	Attributed& Attributed::operator=(Attributed&& rhs) noexcept
	{
		Scope::operator=(std::move(rhs));

		mPrescribedAttributes = std::move(rhs.mPrescribedAttributes);

		(*this)["this"] = this->As<RTTI>();
		UpdateExternalStorage(TypeManager::Instance()->Find(rhs.TypeIdInstance()));

		return *this;
	}
#pragma endregion Constructors, Destructor, Assignment

#pragma region Accessors
	bool Attributed::IsAttribute(const NameType& name)
	{
		return Find(name) != nullptr;
	}

	bool Attributed::IsPrescribedAttribute(const NameType& name)
	{
		if (Find(name) == nullptr) return false;

		for (const auto& attribute : mPrescribedAttributes)
		{
			if (attribute->first == name) return true;
		}

		return false;
	}

	bool Attributed::IsAuxiliaryAttribute(const NameType& name)
	{
		return !IsPrescribedAttribute(name);
	}

	Vector<Scope::Attribute*> Attributed::PrescribedAttributes()
	{
		return mPrescribedAttributes;
	}

	Vector<const Scope::Attribute*> Attributed::PrescribedAttributes() const
	{
		Vector<const Attribute*> constAttributesCopy;

		for (const auto& attribute : mPrescribedAttributes)
		{
			constAttributesCopy.PushBack(attribute);
		}

		return constAttributesCopy;
	}

	Vector<Scope::Attribute*> Attributed::AuxiliaryAttributes()
	{
		return mAuxiliaryAttributes;
	}

	Vector<const Scope::Attribute*> Attributed::AuxiliaryAttributes() const
	{
		Vector<const Attribute*> constAttributesCopy;

		for (const auto& attribute : mAuxiliaryAttributes)
		{
			constAttributesCopy.PushBack(attribute);
		}

		return constAttributesCopy;
	}

	Attributed::DataType& Attributed::AppendAuxiliaryAttribute(const NameType& name)
	{
		if (IsPrescribedAttribute(name))
		{
			throw std::runtime_error("Attribute is prescribed.");
		}

		std::size_t prevNumAttributes = Size();
		DataType& data = Append(name);

		if (Size() > prevNumAttributes)
		{
			mAuxiliaryAttributes.PushBack(mPairPtrs.Back());
		}

		return data;
	}
#pragma endregion Accessors

#pragma region Helper Methods
	void Attributed::Populate(const TypeManager::TypeInfo* typeInfo)
	{
		auto parentTypeInfo = TypeManager::Instance()->Find(typeInfo->parentTypeId);
		if (parentTypeInfo) Populate(parentTypeInfo);

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

	void Attributed::UpdateExternalStorage(const TypeManager::TypeInfo* typeInfo)
	{
		auto parentTypeInfo = TypeManager::Instance()->Find(typeInfo->parentTypeId);
		if (parentTypeInfo) UpdateExternalStorage(parentTypeInfo);

		for (const auto& signature : typeInfo->signatures)
		{
			if (!signature.IsInternal)
			{
				std::byte* address = reinterpret_cast<std::byte*>(this) + signature.Offset;
				Find(signature.Name)->SetStorage(signature.Type, gsl::span(address, signature.Size));
			}
		}
	}
#pragma endregion Helper Methods
}