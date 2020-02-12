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

		TEST_METHOD(Copy)
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

			Scope copyConstructedNoNestedScopes = scope;
			Assert::AreEqual(scope, copyConstructedNoNestedScopes);

			Scope& child0_0 = scope.AppendScope("child0") = copyConstructedNoNestedScopes;
			scope.AppendScope("child0") = copyConstructedNoNestedScopes;
			scope.AppendScope("child1") = copyConstructedNoNestedScopes;
			Scope& child0_0_0 = child0_0.AppendScope("child0_0") = copyConstructedNoNestedScopes;
			child0_0.AppendScope("child0_0") = copyConstructedNoNestedScopes;
			child0_0_0.AppendScope("child0_0_0") = copyConstructedNoNestedScopes;

			Scope copyConstructedNestedScopes = scope;
			Assert::AreEqual(scope, copyConstructedNestedScopes);

			Scope copyAssigned;
			copyAssigned = scope;
			Assert::AreEqual(scope, copyAssigned);

			Scope emptyScope;
			copyAssigned = emptyScope;
			Assert::AreEqual(emptyScope, copyAssigned);

			copyAssigned = scope;
			Assert::AreEqual(scope, copyAssigned);
		}

		TEST_METHOD(Move)
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

			Scope copy = scope;

			Scope& child0_0 = scope.AppendScope("child0") = copy;
			scope.AppendScope("child0") = copy;
			scope.AppendScope("child1") = copy;
			Scope& child0_0_0 = child0_0.AppendScope("child0_0") = copy;
			child0_0.AppendScope("child0_0") = copy;
			child0_0_0.AppendScope("child0_0_0") = copy;

			copy = scope;

			Scope moveConstructed = std::move(scope);
			Assert::AreEqual(copy, moveConstructed);

			Scope moveAssigned;
			scope = copy;
			moveAssigned = std::move(scope);
			Assert::AreEqual(copy, moveAssigned);
		}

		TEST_METHOD(Equality)
		{
			Scope scope1;
			Scope scope2;
			Assert::AreEqual(scope1, scope2);
			
			Datum datumInt = { 10, 20, 30 };
			Datum datumFloat = { 10, 20, 30 };
			Datum datumVector = { glm::vec4(10), glm::vec4(20), glm::vec4(30) };
			Datum datumMatrix = { glm::mat4(10), glm::mat4(20), glm::mat4(30) };
			Datum datumString = { "10", "20", "30" };

			Foo a(10), b(20), c(30);
			Datum datumRTTI = { &a, &b, &c };

			scope1 = {	{ "ints", datumInt }, { "floats", datumFloat }, { "vectors", datumVector },
						{ "matrices", datumMatrix }, { "strings", datumString }, { "pointers", datumRTTI }	};

			Scope& child0_0 = scope1.AppendScope("child0");
			scope1.AppendScope("child0");
			scope1.AppendScope("child1");
			Scope& child0_0_0 = child0_0.AppendScope("child0_0");
			child0_0.AppendScope("child0_0");
			child0_0_0.AppendScope("child0_0_0");

			scope2 = {	{ "pointers", datumRTTI }, { "ints", datumInt }, { "floats", datumFloat }, 
						{ "vectors", datumVector }, { "matrices", datumMatrix }, { "strings", datumString }	};

			Scope& childS2_0_0 = scope2.AppendScope("child0");
			scope2.AppendScope("child0");
			scope2.AppendScope("child1");
			Scope& childS2_0_0_0 = childS2_0_0.AppendScope("child0_0");
			childS2_0_0.AppendScope("child0_0");
			childS2_0_0_0.AppendScope("child0_0_0");

			Assert::AreEqual(scope1, scope2);
		}

		TEST_METHOD(GetParent)
		{
			Scope scope;

			Scope& child0_0 = scope.AppendScope("child0");
			Scope& child0_1 = scope.AppendScope("child0");
			Scope& child1_0 = scope.AppendScope("child1");
			Scope& child0_0_0 = child0_0.AppendScope("child0_0");

			Assert::IsNull(scope.GetParent());
			Assert::AreEqual(&scope, child0_0.GetParent());
			Assert::AreEqual(&scope, child0_1.GetParent());
			Assert::AreEqual(&scope, child1_0.GetParent());
			Assert::AreEqual(&child0_0, child0_0_0.GetParent());
		}

		TEST_METHOD(SubscriptOperators)
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

			Assert::AreEqual(datumInt, scope["ints"]);
			Assert::AreEqual(datumFloat, scope["floats"]);
			Assert::AreEqual(datumVector, scope["vectors"]);
			Assert::AreEqual(datumMatrix, scope["matrices"]);
			Assert::AreEqual(datumString, scope["strings"]);
			Assert::AreEqual(datumRTTI, scope["pointers"]);
			Assert::AreEqual(Datum(), scope["test"]);

			Assert::AreEqual(datumInt, scope[0]);
			Assert::AreEqual(datumFloat, scope[1]);
			Assert::AreEqual(datumVector, scope[2]);
			Assert::AreEqual(datumMatrix, scope[3]);
			Assert::AreEqual(datumString, scope[4]);
			Assert::AreEqual(datumRTTI, scope[5]);
			Assert::AreEqual(Datum(), scope[6]);
			Assert::ExpectException<std::out_of_range>([&scope] { scope[7]; });

			const Scope constScope = scope;

			Assert::AreEqual(datumInt, constScope["ints"]);
			Assert::AreEqual(datumFloat, constScope["floats"]);
			Assert::AreEqual(datumVector, constScope["vectors"]);
			Assert::AreEqual(datumMatrix, constScope["matrices"]);
			Assert::AreEqual(datumString, constScope["strings"]);
			Assert::AreEqual(datumRTTI, constScope["pointers"]);
			Assert::AreEqual(Datum(), constScope["test"]);
			Assert::ExpectException<std::runtime_error>([&constScope] { constScope[""]; });

			Assert::AreEqual(datumInt, constScope[0]);
			Assert::AreEqual(datumFloat, constScope[1]);
			Assert::AreEqual(datumVector, constScope[2]);
			Assert::AreEqual(datumMatrix, constScope[3]);
			Assert::AreEqual(datumString, constScope[4]);
			Assert::AreEqual(datumRTTI, constScope[5]);
			Assert::AreEqual(Datum(), constScope[6]);
			Assert::ExpectException<std::out_of_range>([&constScope] { constScope[7]; });
		}

		TEST_METHOD(Find)
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

			Assert::AreEqual(datumInt, *scope.Find("ints"));
			Assert::AreEqual(datumFloat, *scope.Find("floats"));
			Assert::AreEqual(datumVector, *scope.Find("vectors"));
			Assert::AreEqual(datumMatrix, *scope.Find("matrices"));
			Assert::AreEqual(datumString, *scope.Find("strings"));
			Assert::AreEqual(datumRTTI, *scope.Find("pointers"));
			Assert::IsNull(scope.Find(""));

			const Scope constScope = scope;

			Assert::AreEqual(datumInt, *constScope.Find("ints"));
			Assert::AreEqual(datumFloat, *constScope.Find("floats"));
			Assert::AreEqual(datumVector, *constScope.Find("vectors"));
			Assert::AreEqual(datumMatrix, *constScope.Find("matrices"));
			Assert::AreEqual(datumString, *constScope.Find("strings"));
			Assert::AreEqual(datumRTTI, *constScope.Find("pointers"));
			Assert::IsNull(constScope.Find(""));
		}

		TEST_METHOD(FindName)
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

			Assert::AreEqual("ints"s, *scope.FindName(0));
			Assert::AreEqual("floats"s, *scope.FindName(1));
			Assert::AreEqual("vectors"s, *scope.FindName(2));
			Assert::AreEqual("matrices"s, *scope.FindName(3));
			Assert::AreEqual("strings"s, *scope.FindName(4));
			Assert::AreEqual("pointers"s, *scope.FindName(5));
			Assert::IsNull(scope.FindName(6));
		}

		TEST_METHOD(FindScope)
		{
			Scope scope;

			Scope& child0_0 = scope.AppendScope("child0");
			Scope& child0_1 = scope.AppendScope("child0");
			Scope& child1_0 = scope.AppendScope("child1");
			Scope& child0_0_0 = child0_0.AppendScope("child0_0");

			Assert::AreEqual({ scope.Find("child0"), 0 }, scope.FindScope(child0_0));
			Assert::AreEqual({ scope.Find("child0"), 1 }, scope.FindScope(child0_1));
			Assert::AreEqual({ scope.Find("child1"), 0 }, scope.FindScope(child1_0));
			Assert::AreEqual({ child0_0.Find("child0_0"), 0 }, child0_0.FindScope(child0_0_0));
		}

		TEST_METHOD(Search)
		{
			Scope scope;

			Scope& child0_0 = scope.AppendScope("child0");
			Scope& child0_1 = scope.AppendScope("child0");
			Scope& child1_0 = scope.AppendScope("child1");
			Scope& child0_0_0 = child0_0.AppendScope("child0_0");
			Scope& child0_0_1 = child0_0.AppendScope("child0_0");
			Scope& child0_0_0_0 = child0_0_0.AppendScope("child0_0_0");

			Scope* tmp = nullptr;
			Assert::AreEqual(child0_0, (*child0_0_0_0.Search("child0", &tmp))[0]);
			Assert::AreEqual(scope, *tmp);
			Assert::AreEqual(child0_1, (*child0_0_0_0.Search("child0", &tmp))[1]);
			Assert::AreEqual(scope, *tmp);
			Assert::AreEqual(child1_0, (*child0_0_0_0.Search("child1", &tmp))[0]);
			Assert::AreEqual(scope, *tmp);
			Assert::AreEqual(child0_0_0, (*child0_0_0_0.Search("child0_0", &tmp))[0]);
			Assert::AreEqual(child0_0, *tmp);
			Assert::AreEqual(child0_0_1, (*child0_0_0_0.Search("child0_0", &tmp))[1]);
			Assert::AreEqual(child0_0, *tmp);
			Assert::AreEqual(child0_0, (*child0_0_0_0.Search("child0", &tmp))[0]);
			Assert::AreEqual(scope, *tmp);

			Assert::IsNull(child0_0_0.Search("testNotFound", &tmp));
			Assert::IsNull(tmp);

			Assert::AreEqual(child0_0, (*scope.SearchChildren("child0", &tmp))[0]);
			Assert::AreEqual(scope, *tmp);
			Assert::AreEqual(child0_1, (*scope.SearchChildren("child0", &tmp))[1]);
			Assert::AreEqual(scope, *tmp);
			Assert::AreEqual(child1_0, (*scope.SearchChildren("child1", &tmp))[0]);
			Assert::AreEqual(scope, *tmp);
			Assert::AreEqual(child0_0_0, (*scope.SearchChildren("child0_0", &tmp))[0]);
			Assert::AreEqual(child0_0, *tmp);
			Assert::AreEqual(child0_0_1, (*scope.SearchChildren("child0_0", &tmp))[1]);
			Assert::AreEqual(child0_0, *tmp);
			Assert::AreEqual(child0_0_0_0, (*scope.SearchChildren("child0_0_0", &tmp))[0]);
			Assert::AreEqual(child0_0_0, *tmp);

			Assert::IsNull(scope.SearchChildren("testNotFound", &tmp));
			Assert::IsNull(tmp);
		}

		TEST_METHOD(Append)
		{
			Scope scope;

			scope.Append("test0");
			scope.Append("test1");
			scope.Append("test2");

			Assert::AreEqual("test0"s, *scope.FindName(0));
			Assert::AreEqual("test1"s, *scope.FindName(1));
			Assert::AreEqual("test2"s, *scope.FindName(2));
		}

		TEST_METHOD(AppendScope)
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

			Scope copy = scope;

			Scope& child0_0 = scope.AppendScope("child0") = copy;
			scope.AppendScope("child0") = copy;
			scope.AppendScope("child1") = copy;
			Scope& child0_0_0 = child0_0.AppendScope("child0_0") = copy;
			child0_0.AppendScope("child0_0") = copy;
			child0_0_0.AppendScope("child0_0_0") = copy;
		}

		TEST_METHOD(Orphan)
		{
			Scope scope;

			Scope& child0_0 = scope.AppendScope("child0");
			Scope& child0_1 = scope.AppendScope("child0");
			Scope& child1_0 = scope.AppendScope("child1");
			child0_0.AppendScope("child0_0");

			Scope* tmp = scope.Orphan(child1_0);
			Assert::IsNotNull(tmp);
			delete tmp;

			tmp = scope.Orphan(child0_1);
			Assert::IsNotNull(tmp);
			delete tmp;

			tmp = scope.Orphan(child0_0);
			Assert::IsNotNull(tmp);
			delete tmp;

			Assert::AreEqual(0_z, scope.Find("child0")->Size());
			Assert::AreEqual(0_z, scope.Find("child1")->Size());
		}

		TEST_METHOD(Adopt)
		{
			Scope scope;

			Scope& child0_0 = scope.AppendScope("child0");
			Scope& child0_1 = scope.AppendScope("child0");
			Scope& child1_0 = scope.AppendScope("child1");
			Scope& child0_0_0 = child0_0.AppendScope("child0_0");

			Scope copyChild0_0 = child0_0;
			Scope copyChild0_1 = child0_1;
			Scope copyChild1_0 = child1_0;
			Scope copyChild0_0_0 = child0_0_0;

			Scope adopter;

			Assert::AreEqual(copyChild1_0, adopter.Adopt(child1_0, "child0"));
			//Assert::AreEqual(copyChild0_1, adopter.Adopt(child0_1, "child0"));
			//Assert::AreEqual(copyChild0_0_0, adopter.Adopt(child0_0_0, "child0"));
			//Assert::AreEqual(copyChild0_0, adopter.Adopt(child0_0, "child0"));

			//Assert::AreEqual(4_z, adopter.Find("child0")->Size());
			//Assert::AreEqual(0_z, scope.Find("child0")->Size());
			//Assert::AreEqual(0_z, scope.Find("child1")->Size());
			//Assert::AreEqual(0_z, scope.Find("child0")[0].Size());
		}

		TEST_METHOD(Clear)
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

			Scope copy = scope;

			Scope& child0_0 = scope.AppendScope("child0") = copy;
			scope.AppendScope("child0") = copy;
			scope.AppendScope("child1") = copy;
			Scope& child0_0_0 = child0_0.AppendScope("child0_0") = copy;
			child0_0.AppendScope("child0_0") = copy;
			child0_0_0.AppendScope("child0_0_0") = copy;

			scope.Clear();
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState ScopeTest::sStartMemState;
}