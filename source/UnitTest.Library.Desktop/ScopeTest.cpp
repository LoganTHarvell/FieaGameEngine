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

		TEST_METHOD(Constructor)
		{
			Scope emptyScope;
			Assert::AreEqual(0_z, emptyScope.Size());
			
			Datum datumInt = { 10, 20, 30 };
			Datum datumFloat = { 10, 20, 30 };
			Datum datumVector = { glm::vec4(10), glm::vec4(20), glm::vec4(30) };
			Datum datumMatrix = { glm::mat4(10), glm::mat4(20), glm::mat4(30) };
			Datum datumString = { "10", "20", "30" };

			Foo a(10), b(20), c(30);
			Datum datumRTTI = { &a, &b, &c };

			Scope sa = { { "integers", datumInt } };
			Assert::AreEqual(1_z, sa.Size());

			Scope sb = { { "integers", datumInt }, { "floats", datumFloat }, { "vectors", datumVector },
							{ "matrices", datumMatrix }, { "strings", datumString }, { "pointers", datumRTTI } };
			Assert::AreEqual(6_z, sb.Size());

			Scope sc = sb;

			Scope& child0_0 = sc.AppendScope("child0") = sb;
			sc.AppendScope("child0") = sb;
			sc.AppendScope("child1") = sb;
			Scope& child0_0_0 = child0_0.AppendScope("child0_0") = sb;
			child0_0.AppendScope("child0_0") = sb;
			child0_0_0.AppendScope("child0_0_0") = sb;

			Datum datumScope = { &sa, &sb, &sc };

			Scope scope = { { "integers", datumInt }, { "floats", datumFloat }, { "vectors", datumVector },
							{ "matrices", datumMatrix }, { "strings", datumString }, { "pointers", datumRTTI },
							{ "child0", datumScope }, { "child1", datumScope } };

			Assert::AreEqual(8_z, scope.Size());

			scope = {	{ "child0", datumScope }, { "child1", datumScope },
						{ "integers", datumInt }, { "floats", datumFloat }, { "vectors", datumVector },
						{ "matrices", datumMatrix }, { "strings", datumString }, { "pointers", datumRTTI } };

			Assert::AreEqual(8_z, scope.Size());

			Assert::ExpectException<std::runtime_error>([] { Scope tmp = { {"test", Datum() }, {"test", Datum() } }; });
			Assert::ExpectException<std::runtime_error>([&scope] { scope = { {"test", Datum() }, {"test", Datum() } }; });
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

			Scope scope = { { "integers", datumInt }, { "floats", datumFloat }, { "vectors", datumVector },
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

			Scope scope = { { "integers", datumInt }, { "floats", datumFloat }, { "vectors", datumVector },
							{ "matrices", datumMatrix }, { "strings", datumString }, { "pointers", datumRTTI } };

			Scope copy = scope;

			Scope* child0_0 = &(scope.AppendScope("child0") = copy);
			scope.AppendScope("child0") = copy;
			scope.AppendScope("child1") = copy;
			Scope& child0_0_0 = child0_0->AppendScope("child0_0") = copy;
			child0_0->AppendScope("child0_0") = copy;
			child0_0_0.AppendScope("child0_0_0") = copy;

			copy = scope;

			Scope moveConstructed = std::move(scope);
			Assert::AreEqual(copy, moveConstructed);

			Scope moveAssigned;
			scope = copy;
			moveAssigned = std::move(scope);
			Assert::AreEqual(copy, moveAssigned);

			Scope copyChild0_0 = *child0_0;
			Scope* movedChild = new Scope(std::move(*child0_0));
			delete child0_0;
			Assert::AreEqual(copyChild0_0, *movedChild);

 			scope = copy;
 			child0_0 = &(*scope.Find("child0"))[0];
 			scope.AppendScope("moveIntoChild0") = std::move(*child0_0);
 			delete child0_0;
 			Assert::AreEqual(copyChild0_0, (*scope.Find("child0"))[0]);
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

			scope1 = {	{ "integers", datumInt }, { "floats", datumFloat }, { "vectors", datumVector },
						{ "matrices", datumMatrix }, { "strings", datumString }, { "pointers", datumRTTI }	};

			Scope& child0_0 = scope1.AppendScope("child0");
			scope1.AppendScope("child0");
			scope1.AppendScope("child1");
			Scope& child0_0_0 = child0_0.AppendScope("child0_0");
			child0_0.AppendScope("child0_0");
			child0_0_0.AppendScope("child0_0_0");

			scope2 = {	{ "pointers", datumRTTI }, { "integers", datumInt }, { "floats", datumFloat }, 
						{ "vectors", datumVector }, { "matrices", datumMatrix }, { "strings", datumString }	};

			Scope& childS2_0_0 = scope2.AppendScope("child0");
			scope2.AppendScope("child0");
			scope2.AppendScope("child1");
			Scope& childS2_0_0_0 = childS2_0_0.AppendScope("child0_0");
			childS2_0_0.AppendScope("child0_0");
			childS2_0_0_0.AppendScope("child0_0_0");

			Assert::AreEqual(scope1, scope2);
		}

		TEST_METHOD(Size)
		{
			Scope scope;

			Assert::IsTrue(scope.IsEmpty());
			Assert::AreEqual(0_z, scope.Size());

			Datum datumInt = { 10, 20, 30 };
			Datum datumFloat = { 10, 20, 30 };
			Datum datumVector = { glm::vec4(10), glm::vec4(20), glm::vec4(30) };
			Datum datumMatrix = { glm::mat4(10), glm::mat4(20), glm::mat4(30) };
			Datum datumString = { "10", "20", "30" };

			Foo a(10), b(20), c(30);
			Datum datumRTTI = { &a, &b, &c };


			scope = {	{ "integers", datumInt }, { "floats", datumFloat }, { "vectors", datumVector },
						{ "matrices", datumMatrix }, { "strings", datumString }, { "pointers", datumRTTI }	};

			Assert::IsFalse(scope.IsEmpty());
			Assert::AreEqual(6_z, scope.Size());

			Scope copy = scope;

			Scope& child0_0 = scope.AppendScope("child0") = copy;
			scope.AppendScope("child0") = copy;
			scope.AppendScope("child1") = copy;
			Scope& child0_0_0 = child0_0.AppendScope("child0_0") = copy;
			child0_0.AppendScope("child0_0") = copy;
			child0_0_0.AppendScope("child0_0_0") = copy;

			Assert::AreEqual(8_z, scope.Size());
		}

		TEST_METHOD(Reserve)
		{
			Datum datumInt = { 10, 20, 30 };
			Datum datumFloat = { 10, 20, 30 };
			Scope scope = Scope({ {"integers", datumInt }, {"floats", datumFloat } });

			scope.Reserve(32);
			Assert::AreEqual(32_z, scope.Capacity());
			
			scope.Reserve(10);
			Assert::AreEqual(32_z, scope.Capacity());
		}

		TEST_METHOD(ShrinkToFit)
		{
			Scope scope(30);
			scope.ShrinkToFit();
			Assert::AreEqual(0_z, scope.Capacity());

			Datum datumInt = { 10, 20, 30 };

			for (std::size_t i = 0; i < 50; ++i)
			{
				scope.Append("test" + std::to_string(i)) = datumInt;
			}
			
			scope.ShrinkToFit();
			Assert::AreEqual(50_z, scope.Capacity());

			scope.Reserve(100);

			scope.ShrinkToFit();
			Assert::AreEqual(50_z, scope.Capacity());
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

		TEST_METHOD(IsAncestorDescendantOf)
		{
			Datum datumInt = { 10, 20, 30 };
			Datum datumFloat = { 10, 20, 30 };
			Datum datumVector = { glm::vec4(10), glm::vec4(20), glm::vec4(30) };
			Datum datumMatrix = { glm::mat4(10), glm::mat4(20), glm::mat4(30) };
			Datum datumString = { "10", "20", "30" };

			Foo a(10), b(20), c(30);
			Datum datumRTTI = { &a, &b, &c };

			Scope scope = { { "integers", datumInt }, { "floats", datumFloat }, { "vectors", datumVector },
							{ "matrices", datumMatrix }, { "strings", datumString }, { "pointers", datumRTTI } };

			Scope copy = scope;

			Scope& child0_0 = scope.AppendScope("child0") = copy;
			scope.AppendScope("child0") = copy;
			scope.AppendScope("child1") = copy;
			Scope& child0_0_0 = child0_0.AppendScope("child0_0") = copy;
			child0_0.AppendScope("child0_0") = copy;
			child0_0_0.AppendScope("child0_0_0") = copy;

			Assert::IsTrue(scope.IsAncestorOf(child0_0_0));
			Assert::IsTrue(child0_0_0.IsDescendentOf(scope));

			Scope copyChild0_0 = child0_0;
			Assert::IsFalse(scope.IsAncestorOf((*copyChild0_0.Find("child0_0"))[0]));
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

			Scope scope = { { "integers", datumInt }, { "floats", datumFloat }, { "vectors", datumVector }, 
							{ "matrices", datumMatrix }, { "strings", datumString }, { "pointers", datumRTTI } };

			Assert::AreEqual(datumInt, scope["integers"]);
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

			Assert::AreEqual(datumInt, constScope["integers"]);
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

			Scope scope = { { "integers", datumInt }, { "floats", datumFloat }, { "vectors", datumVector }, 
							{ "matrices", datumMatrix }, { "strings", datumString }, { "pointers", datumRTTI } };

			Assert::AreEqual(datumInt, *scope.Find("integers"));
			Assert::AreEqual(datumFloat, *scope.Find("floats"));
			Assert::AreEqual(datumVector, *scope.Find("vectors"));
			Assert::AreEqual(datumMatrix, *scope.Find("matrices"));
			Assert::AreEqual(datumString, *scope.Find("strings"));
			Assert::AreEqual(datumRTTI, *scope.Find("pointers"));
			Assert::IsNull(scope.Find(""));

			const Scope constScope = scope;

			Assert::AreEqual(datumInt, *constScope.Find("integers"));
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

			Scope scope = { { "integers", datumInt }, { "floats", datumFloat }, { "vectors", datumVector }, 
							{ "matrices", datumMatrix }, { "strings", datumString }, { "pointers", datumRTTI } };

			Assert::AreEqual("integers"s, *scope.FindName(0));
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
			Assert::AreEqual({ nullptr, 0 }, scope.FindScope(Scope()));

			const Scope constScope = scope;
			const Scope& constChild0_0 = (*constScope.Find("child0"))[0];
			const Scope& constChild0_1 = (*constScope.Find("child0"))[1];
			const Scope& constChild1_0 = (*constScope.Find("child1"))[0];
			const Scope& constChild0_0_0 = (*constChild0_0.Find("child0_0"))[0];

			Assert::AreEqual({ constScope.Find("child0"), 0 }, constScope.FindScope(constChild0_0));
			Assert::AreEqual({ constScope.Find("child0"), 1 }, constScope.FindScope(constChild0_1));
			Assert::AreEqual({ constScope.Find("child1"), 0 }, constScope.FindScope(constChild1_0));
			Assert::AreEqual({ constChild0_0.Find("child0_0"), 0 }, constChild0_0.FindScope(constChild0_0_0));
			Assert::AreEqual({ nullptr, 0 }, constScope.FindScope(Scope()));
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

			const Scope constScope = scope;

			const Scope& constChild0_0 = (*constScope.Find("child0"))[0];
			const Scope& constChild0_1 = (*constScope.Find("child0"))[1];
			const Scope& constChild1_0 = (*constScope.Find("child1"))[0];
			const Scope& constChild0_0_0 = (*constChild0_0.Find("child0_0"))[0];
			const Scope& constChild0_0_1 = (*constChild0_0.Find("child0_0"))[1];
			const Scope& constChild0_0_0_0 = (*constChild0_0_0.Find("child0_0_0"))[0];

			const Scope* constTmp = nullptr;
			Assert::AreEqual(constChild0_0, (*constChild0_0_0_0.Search("child0", &constTmp))[0]);
			Assert::AreEqual(constScope, *constTmp);
			Assert::AreEqual(constChild0_1, (*constChild0_0_0_0.Search("child0", &constTmp))[1]);
			Assert::AreEqual(constScope, *constTmp);
			Assert::AreEqual(constChild1_0, (*constChild0_0_0_0.Search("child1", &constTmp))[0]);
			Assert::AreEqual(constScope, *constTmp);
			Assert::AreEqual(constChild0_0_0, (*constChild0_0_0_0.Search("child0_0", &constTmp))[0]);
			Assert::AreEqual(constChild0_0, *constTmp);
			Assert::AreEqual(constChild0_0_1, (*constChild0_0_0_0.Search("child0_0", &constTmp))[1]);
			Assert::AreEqual(constChild0_0, *constTmp);
			Assert::AreEqual(constChild0_0, (*constChild0_0_0_0.Search("child0", &constTmp))[0]);
			Assert::AreEqual(constScope, *constTmp);

			Assert::IsNull(constChild0_0_0.Search("testNotFound", &constTmp));
			Assert::IsNull(constTmp);

			Assert::AreEqual(constChild0_0, (*constScope.SearchChildren("child0", &constTmp))[0]);
			Assert::AreEqual(constScope, *constTmp);
			Assert::AreEqual(constChild0_1, (*constScope.SearchChildren("child0", &constTmp))[1]);
			Assert::AreEqual(constScope, *constTmp);
			Assert::AreEqual(constChild1_0, (*constScope.SearchChildren("child1", &constTmp))[0]);
			Assert::AreEqual(constScope, *constTmp);
			Assert::AreEqual(constChild0_0_0, (*constScope.SearchChildren("child0_0", &constTmp))[0]);
			Assert::AreEqual(constChild0_0, *constTmp);
			Assert::AreEqual(constChild0_0_1, (*constScope.SearchChildren("child0_0", &constTmp))[1]);
			Assert::AreEqual(constChild0_0, *constTmp);
			Assert::AreEqual(constChild0_0_0_0, (*constScope.SearchChildren("child0_0_0", &constTmp))[0]);
			Assert::AreEqual(constChild0_0_0, *constTmp);

			Assert::IsNull(constScope.SearchChildren("testNotFound", &constTmp));
			Assert::IsNull(constTmp);
		}

		TEST_METHOD(ForEachAttribute)
		{
			Datum datumInt = { 10, 20, 30 };
			Datum datumFloat = { 10, 20, 30 };
			Datum datumVector = { glm::vec4(10), glm::vec4(20), glm::vec4(30) };
			Datum datumMatrix = { glm::mat4(10), glm::mat4(20), glm::mat4(30) };
			Datum datumString = { "10", "20", "30" };

			Foo a(10), b(20), c(30);
			Datum datumRTTI = { &a, &b, &c };

			Scope scope = { { "integers", datumInt }, { "floats", datumFloat }, { "vectors", datumVector },
							{ "matrices", datumMatrix }, { "strings", datumString }, { "pointers", datumRTTI } };

			Scope& child0_0 = scope.AppendScope("child0");
			scope.AppendScope("child0");
			scope.AppendScope("child1");
			Scope& child0_0_0 = child0_0.AppendScope("child0_0");
			child0_0.AppendScope("child0_0");
			child0_0_0.AppendScope("child0_0_0");

			const Scope constScope = scope;

			bool isFound = false;
			scope.ForEachAttribute([&](Scope::Attribute& attribute)
				{
					if (attribute.first == "integers")
					{
						isFound = true;
						Assert::AreEqual(datumInt, attribute.second);
						attribute.second.PushBack(40);
						Assert::IsTrue(datumInt != attribute.second);
					}
				});

			Assert::IsTrue(isFound);

			isFound = false;
			constScope.ForEachAttribute([&](const Scope::Attribute& attribute)
				{
					if (attribute.first == "child0")
					{
						isFound = true;
						Assert::AreEqual(child0_0, attribute.second[0]);
					}
				});

			Assert::IsTrue(isFound);
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

			Scope scope = { { "integers", datumInt }, { "floats", datumFloat }, { "vectors", datumVector },
							{ "matrices", datumMatrix }, { "strings", datumString }, { "pointers", datumRTTI } };

			Scope copy = scope;

			Scope& child0_0 = scope.AppendScope("child0") = copy;
			Scope::DataType* data = scope.Find("child0");
			Assert::IsNotNull(data);

			scope.AppendScope("child0") = copy;
			Assert::AreEqual(2_z, data->Size());

			scope.AppendScope("child1") = copy;
			data = scope.Find("child1");
			Assert::IsNotNull(data);

			Scope& child0_0_0 = child0_0.AppendScope("child0_0") = copy;
			data = child0_0.Find("child0_0");
			Assert::IsNotNull(data);

			child0_0.AppendScope("child0_0") = copy;
			data = child0_0.Find("child0_0");
			Assert::AreEqual(2_z, data->Size());

			child0_0_0.AppendScope("child0_0_0") = copy;
			data = child0_0_0.Find("child0_0_0");
			Assert::IsNotNull(data);

			Assert::ExpectException<std::runtime_error>([&scope] { scope.AppendScope("integers"); });
		}

		TEST_METHOD(Orphan)
		{
			Scope scope;

			Scope& child0_0 = scope.AppendScope("child0");
			Scope& child0_1 = scope.AppendScope("child0");
			Scope& child1_0 = scope.AppendScope("child1");
			child0_0.AppendScope("child0_0");

			Scope* tmp = scope.Orphan(child1_0);
			delete tmp;

			tmp = scope.Orphan(child0_1);
			delete tmp;

			tmp = scope.Orphan(child0_0);
			delete tmp;

			Assert::AreEqual(0_z, scope.Find("child0")->Size());
			Assert::AreEqual(0_z, scope.Find("child1")->Size());

			Assert::ExpectException<std::runtime_error>([&scope] { Scope notFound; scope.Orphan(notFound); });
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

			Datum datumInt = { 10, 20, 30 };
			Scope adopter = Scope({ {"integers", datumInt } });

			Assert::ExpectException<std::runtime_error>([&adopter, & child1_0] { adopter.Adopt(child1_0, "integers"); });

			Assert::AreEqual(copyChild1_0, adopter.Adopt(child1_0, "child0"));
			Assert::AreEqual(copyChild0_1, adopter.Adopt(child0_1, "child0"));
			Assert::AreEqual(copyChild0_0_0, adopter.Adopt(child0_0_0, "child0"));
			Assert::AreEqual(copyChild0_0, adopter.Adopt(child0_0, "child0"));

			Assert::AreEqual(4_z, adopter.Find("child0")->Size());
			Assert::AreEqual(0_z, scope.Find("child0")->Size());
			Assert::AreEqual(0_z, scope.Find("child1")->Size());
			Assert::AreEqual(0_z, scope.Find("child0")[0].Size());
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

			Scope scope = { { "integers", datumInt }, { "floats", datumFloat }, { "vectors", datumVector },
							{ "matrices", datumMatrix }, { "strings", datumString }, { "pointers", datumRTTI } };

			Scope copy = scope;

			Scope& child0_0 = scope.AppendScope("child0") = copy;
			scope.AppendScope("child0") = copy;
			scope.AppendScope("child1") = copy;
			Scope& child0_0_0 = child0_0.AppendScope("child0_0") = copy;
			child0_0.AppendScope("child0_0") = copy;
			child0_0_0.AppendScope("child0_0_0") = copy;

			scope.Clear();

			Assert::AreEqual(0_z, scope.Size());
			Assert::IsNull(scope.Find("integers"));
			Assert::IsNull(scope.Find("floats"));
			Assert::IsNull(scope.Find("vectors"));
			Assert::IsNull(scope.Find("matrices"));
			Assert::IsNull(scope.Find("strings"));
			Assert::IsNull(scope.Find("pointers"));
			Assert::IsNull(scope.Find("child0"));
			Assert::IsNull(scope.Find("child1"));
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState ScopeTest::sStartMemState;
}