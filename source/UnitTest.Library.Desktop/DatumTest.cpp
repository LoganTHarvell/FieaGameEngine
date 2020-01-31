#include "pch.h"

#include "ToStringSpecialization.h"
#include "Foo.h"
#include "Bar.h"
#include "Datum.h"

#include <vector>

using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;
using namespace Library;

namespace UnitTests
{
	template<typename T>
	void TestInitialization()
	{

	}

	template<typename T>
	void TestCopy()
	{

	}

	template<typename T>
	void TestMove()
	{

	}

	template<typename T>
	void TestInitializerListAssignment()
	{

	}

	template<typename T>
	void TestSizeCapacity()
	{

	}

	template<typename T>
	void TestRehash()
	{

	}

	template<typename T>
	void TestFind()
	{

	}

	template<typename T>
	void TestElementAccessors()
	{

	}

	template<typename T>
	void TestPushBack()
	{

	}

	template<typename T>
	void TestPopBack()
	{

	}

	template<typename T>
	void TestRemove()
	{

	}

	template<typename T>
	void TestClear()
	{
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DatumTest)
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

 		TEST_METHOD(Initialization)
 		{
 			TestInitialization<int>();
 			TestInitialization<float>();
 			TestInitialization<Foo>();
 		}

 		TEST_METHOD(Copy)
 		{
 			TestCopy<int>();
 			TestCopy<double>();
 			TestCopy<Foo>();
 		}

		TEST_METHOD(Move)
		{
			TestMove<int>();
			TestMove<double>();
			TestMove<Foo>();
		}

		TEST_METHOD(InitializerListAssignment)
		{
			TestInitializerListAssignment<int>();
			TestInitializerListAssignment<double>();
			TestInitializerListAssignment<Foo>();
		}

		TEST_METHOD(SizeCapacity)
		{
			TestSizeCapacity<int>();
			TestSizeCapacity<double>();
			TestSizeCapacity<Foo>();
		}

		TEST_METHOD(Rehash)
		{
			TestRehash<int>();
			TestRehash<double>();
			TestRehash<Foo>();
		}

 		TEST_METHOD(Find)
 		{
 			TestFind<int>();
 			TestFind<double>();
 			TestFind<Foo>();
 		}

		TEST_METHOD(ElementAccess)
		{
			TestElementAccessors<int>();
			TestElementAccessors<double>();
			TestElementAccessors<Foo>();
		}

		TEST_METHOD(PushBack)
		{
			TestPushBack<int>();
			TestPushBack<double>();
			TestPushBack<Foo>();
		}

		TEST_METHOD(PopBack)
		{
			TestPopBack<int>();
			TestPopBack<double>();
			TestPopBack<Foo>();
		}

		TEST_METHOD(Remove)
		{
			TestRemove<int>();
			TestRemove<double>();
			TestRemove<Foo>();
		}

		TEST_METHOD(Clear)
		{
			TestClear<int>();
			TestClear<double>();
			TestClear<Foo>();
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState DatumTest::sStartMemState;
}