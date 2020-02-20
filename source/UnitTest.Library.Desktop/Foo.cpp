#include "pch.h"
#include "Foo.h"

namespace UnitTests
{
	RTTI_DEFINITIONS(Foo);

	Foo::Foo(int data) :
		mIntData(std::make_unique<int>(data))
	{
	}

	Foo::Foo(const Foo& rhs) :
		mIntData(std::make_unique<int>(*rhs.mIntData))
	{
	}

	Foo::Foo(Foo&& rhs) noexcept :
		mIntData(std::move(rhs.mIntData))
	{
		rhs.mIntData = nullptr;
	}

	Foo& Foo::operator=(const Foo& rhs)
	{
		if (this != &rhs)
		{
			*mIntData = *rhs.mIntData;
		}

		return *this;
	}

	Foo& Foo::operator=(Foo&& rhs) noexcept
	{
		if (this != &rhs)
		{
			mIntData = std::move(rhs.mIntData);
			rhs.mIntData = nullptr;
		}

		return *this;
	}

	bool Foo::operator==(const Foo& rhs) const noexcept
	{
		return *mIntData == *rhs.mIntData;
	}

	bool Foo::operator!=(const Foo& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	bool Foo::Equals(const RTTI* rhs) const
	{
		if (!rhs) return false;
		
		const Foo* rhsFoo = rhs->As<Foo>();
		return rhsFoo ? *mIntData == rhsFoo->Data() : false;
	}

	std::string Foo::ToString() const
	{
		return "Foo: " + std::to_string(*mIntData);
	}

	int& Foo::Data()
	{
		return *mIntData;
	}

	int Foo::Data() const
	{
		return *mIntData;
	}
	
	void Foo::SetData(int data)
	{
		*mIntData = data;
	}
}