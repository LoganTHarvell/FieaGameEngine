#include "pch.h"
#include "Foo.h"

namespace UnitTestLibraryDesktop
{
	int& Foo::Data()
	{
		return mData;
	}

	int Foo::Data() const
	{
		return mData;
	}

	Foo::Foo(int data)
	{
		mData = data;
	}

	bool Foo::operator==(const Foo& rhs)
	{
		return mData == rhs.mData;
	}

	bool Foo::operator!=(const Foo& rhs)
	{
		return mData != rhs.mData;
	}
}