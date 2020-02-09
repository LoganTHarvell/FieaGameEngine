#include "pch.h"

#include "ToStringSpecialization.h"
#include "Foo.h"
#include "Bar.h"
#include "Scope.h"


using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ScopeTest)
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

		TEST_METHOD(Equality)
		{
			Scope scope1;
			Scope scope2;
			Assert::AreEqual(scope1, scope2);
			
			Datum datumInt = { 10, 20, 30 };

			scope1 = { { "ints", datumInt } };
			scope2 = { { "ints", datumInt } };
			Assert::AreEqual(scope1, scope2);
		}

		TEST_METHOD(ElementAccessors)
		{
			Datum datumInt = { 10, 20, 30 };
			Datum datumFloat = { 10, 20, 30 };
			Datum datumVector = { glm::vec4(10), glm::vec4(20), glm::vec4(30) };
			Datum datumMatrix = { glm::mat4(10), glm::mat4(20), glm::mat4(30) };
			Datum datumString = { "10", "20", "30" };

			Foo a(10), b(20), c(30);
			Datum datumRTTI = { &a, &b, &c };

			Scope scope = { { "ints", datumInt }, { "floats", datumFloat }, { "vectors", datumVector }, 
							{ "matrices", datumMatrix }, { "strings", datumString }, { "pointers", datumRTTI } };

			Assert::AreEqual(scope["ints"], datumInt);
			Assert::AreEqual(scope["floats"], datumFloat);
			Assert::AreEqual(scope["vectors"], datumVector);
			Assert::AreEqual(scope["matrices"], datumMatrix);
			Assert::AreEqual(scope["strings"], datumString);
			Assert::AreEqual(scope["pointers"], datumRTTI);

			Assert::AreEqual(scope[0], datumInt);
			Assert::AreEqual(scope[1], datumFloat);
			Assert::AreEqual(scope[2], datumVector);
			Assert::AreEqual(scope[3], datumMatrix);
			Assert::AreEqual(scope[4], datumString);
			Assert::AreEqual(scope[5], datumRTTI);

			Assert::AreEqual(*scope.Find("ints"), datumInt);
			Assert::AreEqual(*scope.Find("floats"), datumFloat);
			Assert::AreEqual(*scope.Find("vectors"), datumVector);
			Assert::AreEqual(*scope.Find("matrices"), datumMatrix);
			Assert::AreEqual(*scope.Find("strings"), datumString);
			Assert::AreEqual(*scope.Find("pointers"), datumRTTI);
			
			Scope constScope = scope;

			Assert::AreEqual(*constScope.Find("ints"), datumInt);
			Assert::AreEqual(*constScope.Find("floats"), datumFloat);
			Assert::AreEqual(*constScope.Find("vectors"), datumVector);
			Assert::AreEqual(*constScope.Find("matrices"), datumMatrix);
			Assert::AreEqual(*constScope.Find("strings"), datumString);
			Assert::AreEqual(*constScope.Find("pointers"), datumRTTI);
		}

		TEST_METHOD(Append)
		{
			Scope scope;

			scope.Append("test0");
			scope.Append("test1");
			scope.Append("test2");

			Assert::AreEqual("test0"s, scope.FindName(0));
			Assert::AreEqual("test1"s, scope.FindName(1));
			Assert::AreEqual("test2"s, scope.FindName(2));
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState ScopeTest::sStartMemState;
}