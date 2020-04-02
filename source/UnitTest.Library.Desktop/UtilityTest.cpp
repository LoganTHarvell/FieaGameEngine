#include "pch.h"

#include "ToStringSpecialization.h"
#include "Utility.h"


using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;
using namespace Library;

namespace UtilityTests
{
	TEST_CLASS(UtilityTest)
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

		TEST_METHOD(ToUpperLower)
		{
			Assert::AreEqual("TEST"s, String::ToUpper("test"));
			Assert::AreEqual("test"s, String::ToLower("TEST"));
		}

		TEST_METHOD(PrimeNumbers)
		{
			Assert::IsTrue(Math::IsPrime(113));
			Assert::AreEqual(127_z, Math::FindNextPrime(113));
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState UtilityTest::sStartMemState;
}