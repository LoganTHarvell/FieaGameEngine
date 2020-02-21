#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "Attributed.h"

#include <gsl/span>
#pragma endregion Includes


namespace Library
{
#pragma region Constructors, Destructor, Assignment
	Attributed::Attributed(const RTTI::IdType typeId)
	{	
		(*this)["this"] = this->As<RTTI>();
		Populate(TypeManager::Instance()->Find(typeId));
	}

	Attributed::Attributed(const Attributed& rhs) : Scope(rhs)
	{
		(*this)["this"] = this->As<RTTI>();
		UpdateExternalStorage(TypeManager::Instance()->Find(rhs.TypeIdInstance()));
	}

	Attributed& Attributed::operator=(const Attributed& rhs)
	{
		Scope::operator=(rhs);

		(*this)["this"] = this->As<RTTI>();
		UpdateExternalStorage(TypeManager::Instance()->Find(rhs.TypeIdInstance()));

		return *this;
	}

	Attributed::Attributed(Attributed&& rhs) noexcept : Scope(std::move(rhs))
	{
		(*this)["this"] = this->As<RTTI>();
		UpdateExternalStorage(TypeManager::Instance()->Find(rhs.TypeIdInstance()));
	}

	Attributed& Attributed::operator=(Attributed&& rhs) noexcept
	{
		Scope::operator=(std::move(rhs));

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

		auto typeInfo = TypeManager::Instance()->Find(TypeIdInstance());
		auto equalityFunctor = [&name](Signature signature) { return signature.Name == name; };
		return std::find_if(typeInfo->signatures.begin(), typeInfo->signatures.end(), equalityFunctor) != typeInfo->signatures.end();
	}

	bool Attributed::IsAuxiliaryAttribute(const NameType& name)
	{
		return !IsPrescribedAttribute(name);
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