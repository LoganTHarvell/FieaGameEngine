#include "pch.h"
#include "AttributedBar.h"

namespace UnitTests
{
	RTTI_DEFINITIONS(AttributedBar);

	AttributedBar::AttributedBar(int data) :
		Attributed(AttributedBar::TypeIdClass()), mIntData(std::make_unique<int>(data))
	{
	}

	AttributedBar::AttributedBar(const AttributedBar& rhs) :
		Attributed(rhs), mIntData(std::make_unique<int>(*rhs.mIntData))
	{
	}

	AttributedBar::AttributedBar(AttributedBar&& rhs) noexcept :
		Attributed(std::move(rhs)), mIntData(std::move(rhs.mIntData))
	{
		rhs.mIntData = nullptr;
	}

	AttributedBar& AttributedBar::operator=(const AttributedBar& rhs)
	{
		Attributed::operator=(rhs);

		if (this != &rhs)
		{
			*mIntData = *rhs.mIntData;
		}

		return *this;
	}

	AttributedBar& AttributedBar::operator=(AttributedBar&& rhs) noexcept
	{
		Attributed::operator=(std::move(rhs));

		if (this != &rhs)
		{
			mIntData = std::move(rhs.mIntData);
			rhs.mIntData = nullptr;
		}

		return *this;
	}

	int& AttributedBar::Data()
	{
		return *mIntData;
	}

	int AttributedBar::Data() const
	{
		return *mIntData;
	}
	
	void AttributedBar::SetData(int data)
	{
		*mIntData = data;
	}

	bool AttributedBar::Equals(const RTTI* rhs) const
	{
		if (!rhs) return false;

		const AttributedBar* rhsFoo = rhs->As<AttributedBar>();
		return rhsFoo ? *mIntData == rhsFoo->Data() : false;
	}

	std::string AttributedBar::ToString() const
	{
		return "AttributedBar: " + std::to_string(*mIntData);
	}

	gsl::owner<Library::Scope*> AttributedBar::Clone() const
	{
		return new AttributedBar(*this);
	}

	const Library::TypeManager::TypeInfo& AttributedBar::TypeInfo()
	{
		static Library::TypeManager::TypeInfo typeInfo
		{
			{
				{ "integer", DataType::Types::Integer, false, 1, offsetof(AttributedBar, mIntData) }
			},
		
			Attributed::TypeIdClass()
		};

		return typeInfo;
	}
}