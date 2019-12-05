#pragma once

namespace Library
{
	class Foo
	{
	public:
		int& Data();
		int Data() const;

		Foo(int data = 0);

	private:
		int mData;
	};
}
