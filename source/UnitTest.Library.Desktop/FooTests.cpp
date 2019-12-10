#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTestLibraryDesktop
{
	TEST_CLASS(FooTests)
	{
	public:
		
		TEST_METHOD(Data)
		{
			Foo f;
			Assert::AreEqual(0, f.Data());

			const int data = 10;
			f.Data() = data;
			Assert::AreEqual(data, f.Data());
		}

		TEST_METHOD(DataConst)
		{
			const int data = 10;
			const Foo f(data);
			Assert::AreEqual(data, f.Data());
		}

		TEST_METHOD(Operators)
		{
			Foo f1 = Foo(10);
			Foo f2 = Foo(10);
			Foo f3 = Foo(20);

			Assert::IsTrue(f1 == f2);
			Assert::IsFalse(f1 != f2);

			Assert::IsFalse(f1 == f3);
			Assert::IsTrue(f1 != f3);
		}
	};
}
