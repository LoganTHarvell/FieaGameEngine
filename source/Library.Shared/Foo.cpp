#include "pch.h"
#include "Foo.h"

namespace Library
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
}