#include "pch.h"
#include "AttributedBar.h"

namespace UnitTests
{

	AttributedBar::AttributedBar(int data) :
		Attributed(AttributedBar::TypeIdClass()), mIntData(data)
	{
	}

	AttributedBar::AttributedBar(const AttributedBar& rhs) :
		Attributed(rhs), mIntData(rhs.mIntData)
	{
	}

	AttributedBar::AttributedBar(AttributedBar&& rhs) noexcept :
		Attributed(std::move(rhs)), mIntData(rhs.mIntData)
	{
		rhs.mIntData = 0;
	}

	AttributedBar& AttributedBar::operator=(const AttributedBar& rhs)
	{
		Attributed::operator=(rhs);

		if (this != &rhs)
		{
			mIntData = rhs.mIntData;
		}

		return *this;
	}

	AttributedBar& AttributedBar::operator=(AttributedBar&& rhs) noexcept
	{
		Attributed::operator=(std::move(rhs));

		if (this != &rhs)
		{
			mIntData = rhs.mIntData;
			rhs.mIntData = 0;
		}

		return *this;
	}

	int& AttributedBar::Data()
	{
		return mIntData;
	}

	int AttributedBar::Data() const
	{
		return mIntData;
	}
	
	void AttributedBar::SetData(int data)
	{
		mIntData = data;
	}

	bool AttributedBar::Equals(const RTTI* rhs) const
	{
		if (!rhs) return false;

		const AttributedBar* rhsFoo = rhs->As<AttributedBar>();
		return rhsFoo ? mIntData == rhsFoo->Data() : false;
	}

	std::string AttributedBar::ToString() const
	{
		return "AttributedBar: " + std::to_string(mIntData);
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
				{ "integer", Types::Integer, false, 1, offsetof(AttributedBar, mIntData) }
			},
		
			Attributed::TypeIdClass()
		};

		return typeInfo;
	}
}