#include "pch.h"

#include "ToStringSpecialization.h"
#include "RTTI.h"

#include "Foo.h"
#include "Bar.h"
#include "Scope.h"


using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;
using namespace Library;

namespace UnitTests
{
	template<typename TDerived>
	void TestQueryInterface(RTTI* derivedAsBase)
	{
		Assert::IsNull(derivedAsBase->RTTI::QueryInterface(TDerived::TypeIdClass()));
		Assert::AreEqual(derivedAsBase, derivedAsBase->QueryInterface(TDerived::TypeIdClass()));
	}

	template<typename TDerived, typename TUnrelated>
	void TestIs(RTTI* derivedAsBase, const std::string& derivedTypename, const std::string& unrelatedTypename)
	{
		Assert::IsTrue(derivedAsBase->Is(TDerived::TypeIdClass()));
		Assert::IsFalse(derivedAsBase->Is(TUnrelated::TypeIdClass()));

		Assert::IsTrue(derivedAsBase->Is(std::string(derivedTypename)));
		Assert::IsFalse(derivedAsBase->Is(std::string(unrelatedTypename)));

		const RTTI* copy = derivedAsBase;
		Assert::IsTrue(copy->Is(TDerived::TypeIdClass()));
		Assert::IsFalse(copy->Is(TUnrelated::TypeIdClass()));
	}

	template<typename TDerived, typename TUnrelated>
	void TestAs(RTTI* derivedAsBase)
	{
		Assert::IsNotNull(derivedAsBase->As<TDerived>());
		Assert::IsNull(derivedAsBase->As<TUnrelated>());

		const RTTI* copy = derivedAsBase;
		Assert::IsNotNull(copy->As<TDerived>());
		Assert::IsNull(copy->As<TUnrelated>());
	}

	template<typename TDerived>
	void TestToString(const RTTI* derivedAsBase, const std::string& toStringValue)
	{
		Assert::AreEqual("RTTI"s, derivedAsBase->RTTI::ToString());
		Assert::AreEqual(toStringValue, derivedAsBase->ToString());
	}

	template<typename TDerived>
	void TestEquals(const RTTI* derivedAsBase, const RTTI* equalRTTI, const RTTI* notEqualRTTI)
	{
		Assert::IsFalse(derivedAsBase->RTTI::Equals(equalRTTI));

		Assert::IsTrue(derivedAsBase->Equals(equalRTTI));
		Assert::IsFalse(derivedAsBase->Equals(notEqualRTTI));
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(RTTITest)
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

		TEST_METHOD(QueryInterface)
		{
			Foo foo(10);
			TestQueryInterface<Foo>(&foo);

			Scope scope;
			TestQueryInterface<Scope>(&scope);
		}

		TEST_METHOD(Is)
		{
			Foo foo(10);
			TestIs<Foo, Bar>(&foo, "Foo", "Bar");

			Scope scope;
			TestIs<Scope, Bar>(&scope, "Scope", "Bar");
		}

		TEST_METHOD(As)
		{
			Foo foo(10);
			TestAs<Foo, Bar>(&foo);

			Bar bar(10);
			TestToString<Bar>(&bar, "Bar: 10");
			
			Scope scope;
			TestAs<Scope, Bar>(&scope);
		}

		TEST_METHOD(ToString)
		{
			Foo foo(10);
			TestToString<Foo>(&foo, "Foo: 10");

			Bar bar(10);
			TestToString<Bar>(&bar, "Bar: 10");

			Scope scope;
			TestToString<Scope>(&scope, "Scope()");
		}

		TEST_METHOD(Equals)
		{
			Foo foo(10), fooEqual(10), fooNotEqual(20);
			TestEquals<Foo>(&foo, &fooEqual, &fooNotEqual);

			Scope scope, scopeEqual, scopeNotEqual;
			scopeNotEqual.Append("test");
			TestEquals<Scope>(&scope, &scopeEqual, &scopeNotEqual);
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState RTTITest::sStartMemState;
}