#include "pch.h"

#include "ToStringSpecialization.h"
#include "Foo.h"
#include "Bar.h"
#include "HashMap.h"



using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(HashMapTest)
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

		TEST_METHOD(IteratorInitialization)
		{
			{
				HashMap<double, Foo>::Iterator it;
			}
		
			{
				HashMap<double, Foo> hashMap;
				HashMap<double, Foo>::Iterator it = hashMap.begin();

			}
		}

		TEST_METHOD(IteratorDereference)
		{

		}

		TEST_METHOD(IteratorEquality)
		{
			{
				HashMap<double, Foo> hashMap = HashMap<double, Foo>(10);
				
				Assert::AreEqual(hashMap.begin(), hashMap.end());
			}
		}

		TEST_METHOD(HashMapInitialization)
		{
			HashMap<double, Foo> hashMap;

		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState HashMapTest::sStartMemState;
}