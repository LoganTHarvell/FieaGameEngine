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

		TEST_METHOD(Exception)
		{
			Exception::AggregateException aggregate("Test.");
			Assert::AreEqual("Test.", aggregate.what());
			Assert::AreEqual(0_z, aggregate.Exceptions.Size());
			
			Vector exceptions(Vector<Exception::AggregateException::Entry>::EqualityFunctor{});
			exceptions.Resize(5);
			
			aggregate = Exception::AggregateException("Test.", exceptions);
			Assert::AreEqual("Test.", aggregate.what());
			Assert::AreEqual(exceptions.Size(), aggregate.Exceptions.Size());

			const auto exceptionsCopy = exceptions;
			aggregate = Exception::AggregateException("Test.", std::move(exceptions));
			Assert::AreEqual("Test.", aggregate.what());
			Assert::AreEqual(exceptionsCopy.Size(), aggregate.Exceptions.Size());
		}
		
	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState UtilityTest::sStartMemState;
}