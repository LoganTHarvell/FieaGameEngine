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

			AttributedFoo b(a);

			Assert::IsFalse(b.IsAttribute("null"));
			Assert::IsTrue(b.IsAttribute("integer"));
			Assert::IsTrue(b.IsAttribute("float"));
			Assert::IsTrue(b.IsAttribute("vector"));
			Assert::IsTrue(b.IsAttribute("matrix"));
			Assert::IsTrue(b.IsAttribute("string"));
			Assert::IsTrue(b.IsAttribute("scope"));
			Assert::IsTrue(b.IsAttribute("rttiPtr"));

			Assert::IsTrue(b.IsPrescribedAttribute("integer"));
			Assert::IsTrue(b.IsPrescribedAttribute("float"));
			Assert::IsTrue(b.IsPrescribedAttribute("vector"));
			Assert::IsTrue(b.IsPrescribedAttribute("matrix"));
			Assert::IsTrue(b.IsPrescribedAttribute("string"));
			Assert::IsTrue(b.IsPrescribedAttribute("scope"));
			Assert::IsTrue(b.IsPrescribedAttribute("rttiPtr"));

			Assert::IsFalse(b.IsAuxiliaryAttribute("integer"));
			Assert::IsFalse(b.IsAuxiliaryAttribute("float"));
			Assert::IsFalse(b.IsAuxiliaryAttribute("vector"));
			Assert::IsFalse(b.IsAuxiliaryAttribute("matrix"));
			Assert::IsFalse(b.IsAuxiliaryAttribute("string"));
			Assert::IsFalse(b.IsAuxiliaryAttribute("scope"));
			Assert::IsFalse(b.IsAuxiliaryAttribute("rttiPtr"));

			Assert::IsFalse(b.IsPrescribedAttribute("auxInteger"));
			Assert::IsTrue(b.IsAuxiliaryAttribute("auxInteger"));

			Assert::IsFalse(b.IsPrescribedAttribute("auxScope"));
			Assert::IsTrue(b.IsAuxiliaryAttribute("auxScope"));


			DerivedAttributedFoo derivedA(10);

			Assert::IsFalse(derivedA.IsAttribute("null"));
			Assert::IsTrue(derivedA.IsAttribute("integer"));
			Assert::IsTrue(derivedA.IsAttribute("float"));
			Assert::IsTrue(derivedA.IsAttribute("vector"));
			Assert::IsTrue(derivedA.IsAttribute("matrix"));
			Assert::IsTrue(derivedA.IsAttribute("string"));
			Assert::IsTrue(derivedA.IsAttribute("scope"));
			Assert::IsTrue(derivedA.IsAttribute("rttiPtr"));

			Assert::IsTrue(derivedA.IsPrescribedAttribute("integer"));
			Assert::IsTrue(derivedA.IsPrescribedAttribute("float"));
			Assert::IsTrue(derivedA.IsPrescribedAttribute("vector"));
			Assert::IsTrue(derivedA.IsPrescribedAttribute("matrix"));
			Assert::IsTrue(derivedA.IsPrescribedAttribute("string"));
			Assert::IsTrue(derivedA.IsPrescribedAttribute("scope"));
			Assert::IsTrue(derivedA.IsPrescribedAttribute("rttiPtr"));

			Assert::IsFalse(derivedA.IsAuxiliaryAttribute("integer"));
			Assert::IsFalse(derivedA.IsAuxiliaryAttribute("float"));
			Assert::IsFalse(derivedA.IsAuxiliaryAttribute("vector"));
			Assert::IsFalse(derivedA.IsAuxiliaryAttribute("matrix"));
			Assert::IsFalse(derivedA.IsAuxiliaryAttribute("string"));
			Assert::IsFalse(derivedA.IsAuxiliaryAttribute("scope"));
			Assert::IsFalse(derivedA.IsAuxiliaryAttribute("rttiPtr"));

			derivedA.AppendAuxiliaryAttribute("auxInteger") = 20;
			Assert::IsFalse(derivedA.IsPrescribedAttribute("auxInteger"));
			Assert::IsTrue(derivedA.IsAuxiliaryAttribute("auxInteger"));

			derivedA.AppendAuxiliaryAttribute("auxScope");
			derivedA.AppendScope("auxScope");

			Assert::IsFalse(derivedA.IsPrescribedAttribute("auxScope"));
			Assert::IsTrue(derivedA.IsAuxiliaryAttribute("auxScope"));

			DerivedAttributedFoo derivedB(derivedA);

			Assert::IsFalse(derivedB.IsAttribute("null"));
			Assert::IsTrue(derivedB.IsAttribute("integer"));
			Assert::IsTrue(derivedB.IsAttribute("float"));
			Assert::IsTrue(derivedB.IsAttribute("vector"));
			Assert::IsTrue(derivedB.IsAttribute("matrix"));
			Assert::IsTrue(derivedB.IsAttribute("string"));
			Assert::IsTrue(derivedB.IsAttribute("scope"));
			Assert::IsTrue(derivedB.IsAttribute("rttiPtr"));

			Assert::IsTrue(derivedB.IsPrescribedAttribute("integer"));
			Assert::IsTrue(derivedB.IsPrescribedAttribute("float"));
			Assert::IsTrue(derivedB.IsPrescribedAttribute("vector"));
			Assert::IsTrue(derivedB.IsPrescribedAttribute("matrix"));
			Assert::IsTrue(derivedB.IsPrescribedAttribute("string"));
			Assert::IsTrue(derivedB.IsPrescribedAttribute("scope"));
			Assert::IsTrue(derivedB.IsPrescribedAttribute("rttiPtr"));

			Assert::IsFalse(derivedB.IsAuxiliaryAttribute("integer"));
			Assert::IsFalse(derivedB.IsAuxiliaryAttribute("float"));
			Assert::IsFalse(derivedB.IsAuxiliaryAttribute("vector"));
			Assert::IsFalse(derivedB.IsAuxiliaryAttribute("matrix"));
			Assert::IsFalse(derivedB.IsAuxiliaryAttribute("string"));
			Assert::IsFalse(derivedB.IsAuxiliaryAttribute("scope"));
			Assert::IsFalse(derivedB.IsAuxiliaryAttribute("rttiPtr"));

			Assert::IsFalse(derivedB.IsPrescribedAttribute("auxInteger"));
			Assert::IsTrue(derivedB.IsAuxiliaryAttribute("auxInteger"));

			Assert::IsFalse(derivedB.IsPrescribedAttribute("auxScope"));
			Assert::IsTrue(derivedB.IsAuxiliaryAttribute("auxScope"));
		}

		TEST_METHOD(Accessors)
		{
			AttributedFoo a(10);

			Vector<Attributed::Attribute*> prescribed = a.PrescribedAttributes();
			Vector<Attributed::Attribute*> auxiliary = a.AuxiliaryAttributes();

			Assert::AreEqual(TypeManager::Instance()->Find(a.TypeIdInstance())->signatures.Size()+1, prescribed.Size());
			
			for (const auto& attribute : prescribed)
			{
				Assert::IsNotNull(a.Find(attribute->first));
			}

			Assert::AreEqual(0_z, auxiliary.Size());

			a.AppendAuxiliaryAttribute("auxInteger") = 20;
			a.AppendAuxiliaryAttribute("auxString") = { "20", "30", "40" };
			a.AppendAuxiliaryAttribute("auxScope");
			a.AppendScope("auxScope");

			auxiliary = a.AuxiliaryAttributes();
			Assert::AreEqual(3_z, auxiliary.Size());

			for (const auto& attribute : auxiliary)
			{
				Assert::IsNotNull(a.Find(attribute->first));
			}

			Assert::AreEqual(a.Size(), prescribed.Size() + auxiliary.Size());
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