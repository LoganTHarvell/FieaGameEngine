#include "pch.h"
#include "Bar.h"

namespace UnitTests
{
	RTTI_DEFINITIONS(Bar);

	Bar::Bar(int data) :
		mIntData(std::make_unique<int>(data))
	{
	}

	Bar::Bar(const Bar& rhs) :
		mIntData(std::make_unique<int>(*rhs.mIntData))
	{
	}

	Bar::Bar(Bar&& rhs) noexcept :
		mIntData(std::move(rhs.mIntData))
	{
		rhs.mIntData = nullptr;
	}

	Bar& Bar::operator=(const Bar& rhs)
	{
		if (this != &rhs)
		{
			*mIntData = *rhs.mIntData;
		}

		return *this;
	}

	Bar& Bar::operator=(Bar&& rhs) noexcept
	{
		if (this != &rhs)
		{
			mIntData = std::move(rhs.mIntData);
			rhs.mIntData = nullptr;
		}

		return *this;
	}

	bool Bar::Equals(const RTTI* rhs) const
	{
		if (!rhs) return false;

		const Bar* rhsFoo = rhs->As<Bar>();
		return rhsFoo ? *mIntData == rhsFoo->Data() : false;
	}

	std::string Bar::ToString() const
	{
		return "Bar: " + std::to_string(*mIntData);
	}

	int& Bar::Data()
	{
		return *mIntData;
	}

	int Bar::Data() const
	{
		return *mIntData;
	}
	
	void Bar::SetData(int data)
	{
		*mIntData = data;
	}
}