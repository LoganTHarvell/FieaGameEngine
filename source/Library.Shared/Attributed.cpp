#include "pch.h"

#include "Attributed.h"

#include <gsl/span>


namespace Library
{
	Attributed::Attributed(const RTTI::IdType typeId)
	{
		(*this)["this"] = this->As<RTTI>();
		Populate(typeId);
	}

	Attributed::Attributed(const Attributed& rhs) : Scope(rhs)
	{
		(*this)["this"] = this->As<RTTI>();
	}

	Attributed& Attributed::operator=(const Attributed& rhs)
	{
		Scope::operator=(rhs);

		(*this)["this"] = this->As<RTTI>();

		return *this;
	}

	Attributed::Attributed(Attributed&& rhs) noexcept : Scope(std::move(rhs))
	{
		(*this)["this"] = this->As<RTTI>();
	}

	Attributed& Attributed::operator=(Attributed&& rhs) noexcept
	{
		Scope::operator=(std::move(rhs));

		(*this)["this"] = this->As<RTTI>();

		return *this;
	}

	TypeManager::SignaturesType&& Attributed::Signatures()
	{
		return TypeManager::SignaturesType();
	}

	void Attributed::Populate(const RTTI::IdType typeId)
	{
		const Vector<Signature>& signatures = TypeManager::Instance()->Signatures(typeId);

		for (const auto& signature : signatures)
		{
			DataType& data = Append(signature.Name);

			if (!signature.IsInternal)
			{
				std::byte* address = reinterpret_cast<std::byte*>(this) + signature.Offset;
				data.SetStorage(gsl::span(address, signature.Size));
			}
		}
	}

	void Attributed::UpdateExternalStorage()
	{
	}
}