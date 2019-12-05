#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

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
	};
}
