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
			TypeManager::Create();
			RegisterType<AttributedFoo>();
			RegisterType<DerivedAttributedFoo>();

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

			TypeManager::Destroy();
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
			Assert::IsTrue(a.IsAttribute("float"));
			Assert::IsTrue(a.IsAttribute("vector"));
			Assert::IsTrue(a.IsAttribute("matrix"));
			Assert::IsTrue(a.IsAttribute("string"));
			Assert::IsTrue(a.IsAttribute("scope"));
			Assert::IsTrue(a.IsAttribute("rttiPtr"));

			Assert::IsTrue(a.IsPrescribedAttribute("integer"));
			Assert::IsTrue(a.IsPrescribedAttribute("float"));
			Assert::IsTrue(a.IsPrescribedAttribute("vector"));
			Assert::IsTrue(a.IsPrescribedAttribute("matrix"));
			Assert::IsTrue(a.IsPrescribedAttribute("string"));
			Assert::IsTrue(a.IsPrescribedAttribute("scope"));
			Assert::IsTrue(a.IsPrescribedAttribute("rttiPtr"));

			Assert::IsFalse(a.IsAuxiliaryAttribute("integer"));
			Assert::IsFalse(a.IsAuxiliaryAttribute("float"));
			Assert::IsFalse(a.IsAuxiliaryAttribute("vector"));
			Assert::IsFalse(a.IsAuxiliaryAttribute("matrix"));
			Assert::IsFalse(a.IsAuxiliaryAttribute("string"));
			Assert::IsFalse(a.IsAuxiliaryAttribute("scope"));
			Assert::IsFalse(a.IsAuxiliaryAttribute("rttiPtr"));

			a.AppendAuxiliaryAttribute("auxInteger") = 20;
			Assert::IsFalse(a.IsPrescribedAttribute("auxInteger"));
			Assert::IsTrue(a.IsAuxiliaryAttribute("auxInteger"));

			a.AppendAuxiliaryAttribute("auxScope");
			a.AppendScope("auxScope");

			Assert::IsFalse(a.IsPrescribedAttribute("auxScope"));
			Assert::IsTrue(a.IsAuxiliaryAttribute("auxScope"));
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