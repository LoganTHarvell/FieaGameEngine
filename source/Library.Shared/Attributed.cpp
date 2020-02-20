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
		Populate(typeId);
	}

	Attributed::Attributed(const Attributed& rhs) : Scope(rhs)
	{
		(*this)["this"] = this->As<RTTI>();
		UpdateExternalStorage();
	}

	Attributed& Attributed::operator=(const Attributed& rhs)
	{
		Scope::operator=(rhs);

		(*this)["this"] = this->As<RTTI>();
		UpdateExternalStorage();

		return *this;
	}

	Attributed::Attributed(Attributed&& rhs) noexcept : Scope(std::move(rhs))
	{
		(*this)["this"] = this->As<RTTI>();
		UpdateExternalStorage();
	}

	Attributed& Attributed::operator=(Attributed&& rhs) noexcept
	{
		Scope::operator=(std::move(rhs));

		(*this)["this"] = this->As<RTTI>();
		UpdateExternalStorage();

		return *this;
	}
#pragma endregion Constructors, Destructor, Assignment

#pragma region Accessors
	bool Attributed::IsAttribute(const NameType& name)
	{
		return Search(name) != nullptr;
	}

	bool Attributed::IsPrescribedAttribute(const NameType& name)
	{
		if (Search(name) == nullptr) return false;

		auto signatures = TypeManager::Instance()->Signatures(TypeIdInstance());

		auto equalityFunctor = [&name](Signature signature) { return signature.Name == name; };
		return std::find_if(signatures.begin(), signatures.end(), equalityFunctor) != signatures.end();
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
	void Attributed::Populate(const RTTI::IdType typeId)
	{
		const Vector<Signature>& signatures = TypeManager::Instance()->Signatures(typeId);

		for (const auto& signature : signatures)
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

	void Attributed::UpdateExternalStorage()
	{
		const Vector<Signature>& signatures = TypeManager::Instance()->Signatures(this->TypeIdInstance());

		for (const auto& signature : signatures)
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