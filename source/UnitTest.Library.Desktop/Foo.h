#pragma once

namespace UnitTestLibraryDesktop
{
	class Foo
	{
	public:
		int& Data();
		int Data() const;

		Foo(int data = 0);

		bool operator==(const Foo& rhs);
		bool operator!=(const Foo& rhs);

	private:
		int mData;
	};
}
