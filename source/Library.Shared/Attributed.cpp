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
		DataType* data = Search(name);
		if (data == nullptr) return false;

		bool isPrescribed = false;
		for (const auto& signature : TypeManager::Instance()->Signatures(TypeIdInstance()))
		{
			if (signature.Name == name)
			{
				isPrescribed = true;
				break;
			}
		}

		return isPrescribed;
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
			data.mType = signature.Type;

			if (!signature.IsInternal)
			{
				std::byte* address = reinterpret_cast<std::byte*>(this) + signature.Offset;
				data.SetStorage(gsl::span(address, signature.Size));
			}
			else
			{
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
				Find(signature.Name)->SetStorage(gsl::span(address, signature.Size));
			}
		}
	}
#pragma endregion Helper Methods
}