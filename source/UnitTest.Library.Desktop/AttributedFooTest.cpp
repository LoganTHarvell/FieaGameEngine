#include "pch.h"

#include "ToStringSpecialization.h"
#include "AttributedFoo.h"
#include "DerivedAttributedFoo.h"


using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;


namespace UnitTestLibraryDesktop
{
	TEST_CLASS(AttributedFooTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif

			TypeManager::Create();
			REGISTER_TYPE(AttributedFoo, Attributed)
			REGISTER_TYPE(DerivedAttributedFoo, AttributedFoo)
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			TypeManager::Destroy();

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
			AttributedFoo a;
			Assert::AreEqual(0, a.Data());

			AttributedFoo b(10);
			Assert::AreEqual(10, b.Data());

			AttributedFoo c = AttributedFoo(20);
		}

		TEST_METHOD(CopySemantics)
		{
			AttributedFoo a(10);
			Assert::AreEqual(10, a.Data());

			AttributedFoo b(a);
			Assert::AreEqual(a, b);

			AttributedFoo c;
			c = a;
			Assert::AreEqual(c, a);

			AttributedFoo d(a);
			Assert::AreEqual(d, a);
		}

		TEST_METHOD(EqualityOperators)
		{
			AttributedFoo a(10);
			AttributedFoo b(a);
			Assert::AreEqual(a, b);

			AttributedFoo c;
			Assert::IsTrue(c != a); // Assert::AreNotEqual does not invoke operator!=
		}

		TEST_METHOD(SetData)
		{
			AttributedFoo a;
			Assert::AreEqual(0, a.Data());

			const int data = 10;
			a.SetData(data);
			Assert::AreEqual(data, a.Data());
		}

		TEST_METHOD(MoveSemantics)
		{
			const int data = 10;
			AttributedFoo a(data);
			Assert::AreEqual(data, a.Data());

			AttributedFoo b(std::move(a));
			Assert::AreEqual(data, b.Data());

			AttributedFoo c;
			Assert::AreEqual(0, c.Data());
			c = std::move(b);
			Assert::AreEqual(data, c.Data());
		}

		TEST_METHOD(RTTITest)
		{
			AttributedFoo a = AttributedFoo(10);
			AttributedFoo b = AttributedFoo(10);

			Assert::IsTrue(a.Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(a.Equals(&b));
		}

		TEST_METHOD(ToString)
		{
			Assert::AreEqual("AttributedFoo: 0", AttributedFoo().ToString().c_str());
		}

		TEST_METHOD(QueryAttributes)
		{
			AttributedFoo a(10);

			Assert::IsFalse(a.IsAttribute("null"));
			Assert::IsTrue(a.IsAttribute("integer"));
			Assert::IsTrue(a.IsPrescribedAttribute("integer"));
			Assert::IsFalse(a.IsAuxiliaryAttribute("integer"));

			a.AppendAuxiliaryAttribute("auxInt") = 20;
			Assert::IsFalse(a.IsPrescribedAttribute("auxInt"));
			Assert::IsTrue(a.IsAuxiliaryAttribute("auxInt"));
		}

		TEST_METHOD(AppendAttribute)
		{
			AttributedFoo a(10);

			Assert::ExpectException<std::runtime_error>([&a] { a.AppendAuxiliaryAttribute("integer") = 20; });
			
			a.AppendAuxiliaryAttribute("auxInt") = 20;
			Assert::IsNotNull(a.Find("auxInt"));
			Assert::IsTrue(a.IsAuxiliaryAttribute("auxInt"));
		}

		TEST_METHOD(Derived)
		{
			DerivedAttributedFoo a(20);
		}

		TEST_METHOD(Clone)
		{
			Scope* scope = new AttributedFoo();
			Scope* derived = new DerivedAttributedFoo();

			Scope* tmp = scope->Clone();
			Assert::AreEqual(*AttributedFoo().As<Scope>(), *tmp);
			delete tmp;

			tmp = derived->Clone();
			Assert::AreEqual(*AttributedFoo().As<Scope>(), *tmp);
			delete tmp;
			
			delete scope;
			delete derived;
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState AttributedFooTest::sStartMemState;
}