#include "pch.h"
#include "Bar.h"

namespace UnitTests
{
	Bar::Bar(int data) :
		mData(std::make_unique<int>(data))
	{
	}

	Bar::Bar(const Bar& rhs) :
		mData(std::make_unique<int>(*rhs.mData))
	{
	}

	Bar::Bar(Bar&& rhs) noexcept :
		mData(std::move(rhs.mData))
	{
		rhs.mData = nullptr;
	}

	Bar& Bar::operator=(const Bar& rhs)
	{
		if (this != &rhs)
		{
			*mData = *rhs.mData;
		}

		return *this;
	}

	Bar& Bar::operator=(Bar&& rhs) noexcept
	{
		if (this != &rhs)
		{
			mData = std::move(rhs.mData);
			rhs.mData = nullptr;
		}

		return *this;
	}

	bool Bar::Equals(const RTTI* rhs) const
	{
		if (!rhs) return false;

		const Bar* rhsFoo = rhs->As<Bar>();
		return rhsFoo ? *mData == rhsFoo->Data() : false;
	}

	std::string Bar::ToString() const
	{
		std::ostringstream oss;
		oss << "Bar(" << *mData << ")";
		return oss.str();
	}

	int& Bar::Data()
	{
		return *mData;
	}

	int Bar::Data() const
	{
		return *mData;
	}
	
	void Bar::SetData(int data)
	{
		*mData = data;
	}
}