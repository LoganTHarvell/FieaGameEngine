#include "pch.h"
#include "Foo.h"

namespace UnitTests
{
	RTTI_DEFINITIONS(Foo);

	Foo::Foo(int data) :
		mData(std::make_unique<int>(data))
	{
	}

	Foo::Foo(const Foo& rhs) :
		mData(std::make_unique<int>(*rhs.mData))
	{
	}

	Foo::Foo(Foo&& rhs) noexcept :
		mData(std::move(rhs.mData))
	{
		rhs.mData = nullptr;
	}

	Foo& Foo::operator=(const Foo& rhs)
	{
		if (this != &rhs)
		{
			*mData = *rhs.mData;
		}

		return *this;
	}

	Foo& Foo::operator=(Foo&& rhs) noexcept
	{
		if (this != &rhs)
		{
			mData = std::move(rhs.mData);
			rhs.mData = nullptr;
		}

		return *this;
	}

	bool Foo::operator==(const Foo& rhs) const noexcept
	{
		return *mData == *rhs.mData;
	}

	bool Foo::operator!=(const Foo& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	bool Foo::Equals(const RTTI* rhs) const
	{
		if (!rhs) return false;
		
		const Foo* rhsFoo = rhs->As<Foo>();
		return rhsFoo ? *mData == rhsFoo->Data() : false;
	}

	int& Foo::Data()
	{
		return *mData;
	}

	int Foo::Data() const
	{
		return *mData;
	}
	
	void Foo::SetData(int data)
	{
		*mData = data;
	}
}