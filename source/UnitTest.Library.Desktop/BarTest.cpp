#include "pch.h"

#include "ToStringSpecialization.h"
#include "Bar.h"


using namespace std;
using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;


namespace UnitTestLibraryDesktop
{
	TEST_CLASS(BarTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(Constructor)
		{
			Bar a;
			Assert::AreEqual(0, a.Data());

			Bar b(10);
			Assert::AreEqual(10, b.Data());

			Bar c = Bar(20);
		}

		TEST_METHOD(CopySemantics)
		{
			Bar a(10);
			Assert::AreEqual(10, a.Data());

			Bar b(a);
			Assert::AreEqual(a.Data(), b.Data());

			Bar c;
			c = a;
			Assert::AreEqual(c.Data(), a.Data());

			Bar d(a);
			Assert::AreEqual(d.Data(), a.Data());
		}

		TEST_METHOD(SetData)
		{
			Bar a;
			Assert::AreEqual(0, a.Data());

			const int data = 10;
			a.SetData(data);
			Assert::AreEqual(data, a.Data());
		}

		TEST_METHOD(MoveSemantics)
		{
			const int data = 10;
			Bar a(data);
			Assert::AreEqual(data, a.Data());

			Bar b(std::move(a));
			Assert::AreEqual(data, b.Data());

			Bar c;
			Assert::AreEqual(0, c.Data());
			c = std::move(b);
			Assert::AreEqual(data, c.Data());
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState BarTest::sStartMemState;
}