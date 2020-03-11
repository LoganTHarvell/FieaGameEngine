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
			Assert::AreEqual(0, a.Find("Integer")->Get<int>());
			Assert::AreEqual(0.0f, a.Find("Float")->Get<float>());
			Assert::AreEqual(glm::vec4(0.0f), a.Find("Vector")->Get<glm::vec4>());
			Assert::AreEqual(glm::mat4(0.0f), a.Find("Matrix")->Get<glm::mat4>());
			Assert::AreEqual("0"s, a.Find("String")->Get<std::string>());

			AttributedFoo b(10);
			Assert::AreEqual(10, b.Find("Integer")->Get<int>());
			Assert::AreEqual(10.0f, b.Find("Float")->Get<float>());
			Assert::AreEqual(glm::vec4(10.0f), b.Find("Vector")->Get<glm::vec4>());
			Assert::AreEqual(glm::mat4(10.0f), b.Find("Matrix")->Get<glm::mat4>());
			Assert::AreEqual("10"s, b.Find("String")->Get<std::string>());

			AttributedFoo c = AttributedFoo(10);
			Assert::AreEqual(b, c);
		}

		TEST_METHOD(CopySemantics)
		{
			AttributedFoo a(10);

			a.AppendAuxiliaryAttribute("auxInteger") = 20;
			a.AppendAuxiliaryAttribute("auxString") = { "20", "30", "40" };
			a.AppendAuxiliaryAttribute("auxScope");
			Scope& auxScope = a.AppendScope("auxScope");

			AttributedFoo b(a);
			Assert::AreEqual(20, b.Find("auxInteger")->Get<int>());
			Assert::AreEqual("20"s, b.Find("auxString")->Get<std::string>(0));
			Assert::AreEqual("30"s, b.Find("auxString")->Get<std::string>(1));
			Assert::AreEqual("40"s, b.Find("auxString")->Get<std::string>(2));
			Assert::AreEqual(auxScope, *b.Find("auxScope")->Get<Scope*>());

			AttributedFoo c;
			c = a;
			Assert::AreEqual(20, c.Find("auxInteger")->Get<int>());
			Assert::AreEqual("20"s, c.Find("auxString")->Get<std::string>(0));
			Assert::AreEqual("30"s, c.Find("auxString")->Get<std::string>(1));
			Assert::AreEqual("40"s, c.Find("auxString")->Get<std::string>(2));
			Assert::AreEqual(auxScope, *c.Find("auxScope")->Get<Scope*>());

			AttributedFoo d(a);
			Assert::AreEqual(20, d.Find("auxInteger")->Get<int>());
			Assert::AreEqual("20"s, d.Find("auxString")->Get<std::string>(0));
			Assert::AreEqual("30"s, d.Find("auxString")->Get<std::string>(1));
			Assert::AreEqual("40"s, d.Find("auxString")->Get<std::string>(2));
			Assert::AreEqual(auxScope, *d.Find("auxScope")->Get<Scope*>());
		}

		TEST_METHOD(EqualityOperators)
		{
			AttributedFoo a(10);

			const auto& auxInteger = a.AppendAuxiliaryAttribute("auxInteger") = 20;
			const auto& auxString = a.AppendAuxiliaryAttribute("auxString") = { "20", "30", "40" };
			const auto& auxScope = a.AppendAuxiliaryAttribute("auxScope");
			a.AppendScope("auxScope");
			
			AttributedFoo b(a);

			Assert::AreEqual(0, b.Find("Integer")->Get<int>());
			Assert::AreEqual(0.0f, b.Find("Float")->Get<float>());
			Assert::AreEqual(glm::vec4(0.0f), b.Find("Vector")->Get<glm::vec4>());
			Assert::AreEqual(glm::mat4(0.0f), b.Find("Matrix")->Get<glm::mat4>());
			Assert::AreEqual("0"s, b.Find("String")->Get<std::string>());
			Assert::AreEqual(0, b.Find("Rtti")->Get<RTTI*>()->As<Foo>()->Data());

			Assert::AreEqual(auxInteger, *b.Find("auxInteger"));
			Assert::AreEqual(auxString, *b.Find("auxString"));
			Assert::AreEqual(auxScope, *b.Find("auxScope"));
		}

		TEST_METHOD(MoveSemantics)
		{
			AttributedFoo a(10);

			a.AppendAuxiliaryAttribute("auxInteger") = 20;
			a.AppendAuxiliaryAttribute("auxString") = { "20", "30", "40" };
			a.AppendAuxiliaryAttribute("auxScope");
			Scope& auxScope = a.AppendScope("auxScope");

			AttributedFoo b(std::move(a));

			Assert::AreEqual(10, b.Find("Integer")->Get<int>());
			Assert::AreEqual(10.0f, b.Find("Float")->Get<float>());
			Assert::AreEqual(glm::vec4(10.0f), b.Find("Vector")->Get<glm::vec4>());
			Assert::AreEqual(glm::mat4(10.0f), b.Find("Matrix")->Get<glm::mat4>());
			Assert::AreEqual("10"s, b.Find("String")->Get<std::string>());
			//Assert::AreEqual(10, b.Find("Rtti")->Get<RTTI*>()->As<Foo>()->Data());

			Assert::AreEqual(20, b.Find("auxInteger")->Get<int>());
			Assert::AreEqual("20"s, b.Find("auxString")->Get<std::string>(0));
			Assert::AreEqual("30"s, b.Find("auxString")->Get<std::string>(1));
			Assert::AreEqual("40"s, b.Find("auxString")->Get<std::string>(2));
			Assert::AreEqual(auxScope, *b.Find("auxScope")->Get<Scope*>());

			Assert::IsTrue(a.IsEmpty());
			Assert::AreEqual(0_z, a.Capacity());

			AttributedFoo c;
			c = std::move(b);

			Assert::AreEqual(10, c.Find("Integer")->Get<int>());
			Assert::AreEqual(10.0f, c.Find("Float")->Get<float>());
			Assert::AreEqual(glm::vec4(10.0f), c.Find("Vector")->Get<glm::vec4>());
			Assert::AreEqual(glm::mat4(10.0f), c.Find("Matrix")->Get<glm::mat4>());
			Assert::AreEqual("10"s, c.Find("String")->Get<std::string>());
			//Assert::AreEqual(10, c.Find("Rtti")->Get<RTTI*>()->As<Foo>()->Data());

			Assert::AreEqual(20, c.Find("auxInteger")->Get<int>());
			Assert::AreEqual("20"s, c.Find("auxString")->Get<std::string>(0));
			Assert::AreEqual("30"s, c.Find("auxString")->Get<std::string>(1));
			Assert::AreEqual("40"s, c.Find("auxString")->Get<std::string>(2));
			Assert::AreEqual(auxScope, *c.Find("auxScope")->Get<Scope*>());

			Assert::IsTrue(b.IsEmpty());
			Assert::AreEqual(0_z, b.Capacity());
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
			AttributedFoo a(10);

			a.AppendAuxiliaryAttribute("auxInteger") = 20;
			a.AppendAuxiliaryAttribute("auxString") = { "20", "30", "40" };
			a.AppendAuxiliaryAttribute("auxScope");
			a.AppendScope("auxScope");

			std::string str = "Foo Attributed('Integer':{10},'Float':{10.000000},'Vector':{vec4(10.000000, 10.000000, 10.000000, 10.000000)},'Matrix':{mat4x4((10.000000, 0.000000, 0.000000, 0.000000), (0.000000, 10.000000, 0.000000, 0.000000), (0.000000, 0.000000, 10.000000, 0.000000), (0.000000, 0.000000, 0.000000, 10.000000))},'String':{10},'Scope':{},'Rtti':{Foo(10)},'IntegerArray':{10,10},'FloatArray':{10.000000,0.000000},'VectorArray':{vec4(10.000000, 10.000000, 10.000000, 10.000000),vec4(10.000000, 10.000000, 10.000000, 10.000000)},'MatrixArray':{mat4x4((10.000000, 0.000000, 0.000000, 0.000000), (0.000000, 10.000000, 0.000000, 0.000000), (0.000000, 0.000000, 10.000000, 0.000000), (0.000000, 0.000000, 0.000000, 10.000000)),mat4x4((10.000000, 0.000000, 0.000000, 0.000000), (0.000000, 10.000000, 0.000000, 0.000000), (0.000000, 0.000000, 10.000000, 0.000000), (0.000000, 0.000000, 0.000000, 10.000000))},'StringArray':{10,10},'ScopeArray':{},'RttiArray':{Foo(10),Foo(10)},'auxInteger':{20},'auxString':{20,30,40},'auxScope':{Scope()})"s;

			Assert::AreEqual(str, a.ToString());
		}

		TEST_METHOD(QueryAttributes)
		{
			AttributedFoo a(10);

			Assert::IsFalse(a.IsAttribute("null"));
			Assert::IsTrue(a.IsAttribute("Integer"));
			Assert::IsTrue(a.IsAttribute("Float"));
			Assert::IsTrue(a.IsAttribute("Vector"));
			Assert::IsTrue(a.IsAttribute("Matrix"));
			Assert::IsTrue(a.IsAttribute("String"));
			Assert::IsTrue(a.IsAttribute("Scope"));
			Assert::IsTrue(a.IsAttribute("Rtti"));

			Assert::IsTrue(a.IsPrescribedAttribute("Integer"));
			Assert::IsTrue(a.IsPrescribedAttribute("Float"));
			Assert::IsTrue(a.IsPrescribedAttribute("Vector"));
			Assert::IsTrue(a.IsPrescribedAttribute("Matrix"));
			Assert::IsTrue(a.IsPrescribedAttribute("String"));
			Assert::IsTrue(a.IsPrescribedAttribute("Scope"));
			Assert::IsTrue(a.IsPrescribedAttribute("Rtti"));

			Assert::IsFalse(a.IsAuxiliaryAttribute("Integer"));
			Assert::IsFalse(a.IsAuxiliaryAttribute("Float"));
			Assert::IsFalse(a.IsAuxiliaryAttribute("Vector"));
			Assert::IsFalse(a.IsAuxiliaryAttribute("Matrix"));
			Assert::IsFalse(a.IsAuxiliaryAttribute("String"));
			Assert::IsFalse(a.IsAuxiliaryAttribute("Scope"));
			Assert::IsFalse(a.IsAuxiliaryAttribute("Rtti"));

			a.AppendAuxiliaryAttribute("auxInteger") = 20;
			Assert::IsFalse(a.IsPrescribedAttribute("auxInteger"));
			Assert::IsTrue(a.IsAuxiliaryAttribute("auxInteger"));

			a.AppendAuxiliaryAttribute("auxScope");
			a.AppendScope("auxScope");

			Assert::IsFalse(a.IsPrescribedAttribute("auxScope"));
			Assert::IsTrue(a.IsAuxiliaryAttribute("auxScope"));

			AttributedFoo b(a);

			Assert::IsFalse(b.IsAttribute("null"));
			Assert::IsTrue(b.IsAttribute("Integer"));
			Assert::IsTrue(b.IsAttribute("Float"));
			Assert::IsTrue(b.IsAttribute("Vector"));
			Assert::IsTrue(b.IsAttribute("Matrix"));
			Assert::IsTrue(b.IsAttribute("String"));
			Assert::IsTrue(b.IsAttribute("Scope"));
			Assert::IsTrue(b.IsAttribute("Rtti"));

			Assert::IsTrue(b.IsPrescribedAttribute("Integer"));
			Assert::IsTrue(b.IsPrescribedAttribute("Float"));
			Assert::IsTrue(b.IsPrescribedAttribute("Vector"));
			Assert::IsTrue(b.IsPrescribedAttribute("Matrix"));
			Assert::IsTrue(b.IsPrescribedAttribute("String"));
			Assert::IsTrue(b.IsPrescribedAttribute("Scope"));
			Assert::IsTrue(b.IsPrescribedAttribute("Rtti"));

			Assert::IsFalse(b.IsAuxiliaryAttribute("Integer"));
			Assert::IsFalse(b.IsAuxiliaryAttribute("Float"));
			Assert::IsFalse(b.IsAuxiliaryAttribute("Vector"));
			Assert::IsFalse(b.IsAuxiliaryAttribute("Matrix"));
			Assert::IsFalse(b.IsAuxiliaryAttribute("String"));
			Assert::IsFalse(b.IsAuxiliaryAttribute("Scope"));
			Assert::IsFalse(b.IsAuxiliaryAttribute("Rtti"));

			Assert::IsFalse(b.IsPrescribedAttribute("auxInteger"));
			Assert::IsTrue(b.IsAuxiliaryAttribute("auxInteger"));

			Assert::IsFalse(b.IsPrescribedAttribute("auxScope"));
			Assert::IsTrue(b.IsAuxiliaryAttribute("auxScope"));


			DerivedAttributedFoo derivedA(10);

			Assert::IsFalse(derivedA.IsAttribute("null"));
			Assert::IsTrue(derivedA.IsAttribute("Integer"));
			Assert::IsTrue(derivedA.IsAttribute("Float"));
			Assert::IsTrue(derivedA.IsAttribute("Vector"));
			Assert::IsTrue(derivedA.IsAttribute("Matrix"));
			Assert::IsTrue(derivedA.IsAttribute("String"));
			Assert::IsTrue(derivedA.IsAttribute("Scope"));
			Assert::IsTrue(derivedA.IsAttribute("Rtti"));

			Assert::IsTrue(derivedA.IsPrescribedAttribute("Integer"));
			Assert::IsTrue(derivedA.IsPrescribedAttribute("Float"));
			Assert::IsTrue(derivedA.IsPrescribedAttribute("Vector"));
			Assert::IsTrue(derivedA.IsPrescribedAttribute("Matrix"));
			Assert::IsTrue(derivedA.IsPrescribedAttribute("String"));
			Assert::IsTrue(derivedA.IsPrescribedAttribute("Scope"));
			Assert::IsTrue(derivedA.IsPrescribedAttribute("Rtti"));

			Assert::IsFalse(derivedA.IsAuxiliaryAttribute("Integer"));
			Assert::IsFalse(derivedA.IsAuxiliaryAttribute("Float"));
			Assert::IsFalse(derivedA.IsAuxiliaryAttribute("Vector"));
			Assert::IsFalse(derivedA.IsAuxiliaryAttribute("Matrix"));
			Assert::IsFalse(derivedA.IsAuxiliaryAttribute("String"));
			Assert::IsFalse(derivedA.IsAuxiliaryAttribute("Scope"));
			Assert::IsFalse(derivedA.IsAuxiliaryAttribute("Rtti"));

			derivedA.AppendAuxiliaryAttribute("auxInteger") = 20;
			Assert::IsFalse(derivedA.IsPrescribedAttribute("auxInteger"));
			Assert::IsTrue(derivedA.IsAuxiliaryAttribute("auxInteger"));

			derivedA.AppendAuxiliaryAttribute("auxScope");
			derivedA.AppendScope("auxScope");

			Assert::IsFalse(derivedA.IsPrescribedAttribute("auxScope"));
			Assert::IsTrue(derivedA.IsAuxiliaryAttribute("auxScope"));

			DerivedAttributedFoo derivedB(derivedA);

			Assert::IsFalse(derivedB.IsAttribute("null"));
			Assert::IsTrue(derivedB.IsAttribute("Integer"));
			Assert::IsTrue(derivedB.IsAttribute("Float"));
			Assert::IsTrue(derivedB.IsAttribute("Vector"));
			Assert::IsTrue(derivedB.IsAttribute("Matrix"));
			Assert::IsTrue(derivedB.IsAttribute("String"));
			Assert::IsTrue(derivedB.IsAttribute("Scope"));
			Assert::IsTrue(derivedB.IsAttribute("Rtti"));

			Assert::IsTrue(derivedB.IsPrescribedAttribute("Integer"));
			Assert::IsTrue(derivedB.IsPrescribedAttribute("Float"));
			Assert::IsTrue(derivedB.IsPrescribedAttribute("Vector"));
			Assert::IsTrue(derivedB.IsPrescribedAttribute("Matrix"));
			Assert::IsTrue(derivedB.IsPrescribedAttribute("String"));
			Assert::IsTrue(derivedB.IsPrescribedAttribute("Scope"));
			Assert::IsTrue(derivedB.IsPrescribedAttribute("Rtti"));

			Assert::IsFalse(derivedB.IsAuxiliaryAttribute("Integer"));
			Assert::IsFalse(derivedB.IsAuxiliaryAttribute("Float"));
			Assert::IsFalse(derivedB.IsAuxiliaryAttribute("Vector"));
			Assert::IsFalse(derivedB.IsAuxiliaryAttribute("Matrix"));
			Assert::IsFalse(derivedB.IsAuxiliaryAttribute("String"));
			Assert::IsFalse(derivedB.IsAuxiliaryAttribute("Scope"));
			Assert::IsFalse(derivedB.IsAuxiliaryAttribute("Rtti"));

			Assert::IsFalse(derivedB.IsPrescribedAttribute("auxInteger"));
			Assert::IsTrue(derivedB.IsAuxiliaryAttribute("auxInteger"));

			Assert::IsFalse(derivedB.IsPrescribedAttribute("auxScope"));
			Assert::IsTrue(derivedB.IsAuxiliaryAttribute("auxScope"));
		}

		TEST_METHOD(Accessors)
		{
			AttributedFoo a(10);

			std::size_t count = 0;
			auto functor = [&a, &count](const Attributed::Attribute& attribute)
			{
				count++;
				Assert::IsNotNull(a.Find(attribute.first));
			};

			a.ForEachPrescribed(functor);

			Assert::AreEqual(TypeManager::Instance()->Find(a.TypeIdInstance())->signatures.Size()+1, count);
			
			a.AppendAuxiliaryAttribute("auxInteger") = 20;
			a.AppendAuxiliaryAttribute("auxString") = { "20", "30", "40" };
			a.AppendAuxiliaryAttribute("auxScope");
			a.AppendScope("auxScope");

			a.ForEachAuxiliary(functor);

			Assert::AreEqual(a.Size(), count);


			const AttributedFoo constA(a);

			count = 0;
			auto constFunctor = [&constA, &count](const Attributed::Attribute& attribute)
			{
				count++;
				Assert::IsNotNull(constA.Find(attribute.first));
			};

			constA.ForEachPrescribed(constFunctor);

			Assert::AreEqual(TypeManager::Instance()->Find(constA.TypeIdInstance())->signatures.Size() + 1, count);

			constA.ForEachAuxiliary(constFunctor);

			Assert::AreEqual(constA.Size(), count);
		}

		TEST_METHOD(AppendAttribute)
		{
			AttributedFoo a(10);

			Assert::ExpectException<std::runtime_error>([&a] { a.AppendAuxiliaryAttribute("Integer") = 20; });
			
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
			Attributed* scope = new AttributedFoo(10);
			Scope* derived = new DerivedAttributedFoo();

			Scope* tmp = scope->Clone();
			Assert::AreEqual(0, tmp->Find("Integer")->Get<int>());
			Assert::AreEqual(0.0f, tmp->Find("Float")->Get<float>());
			Assert::AreEqual(glm::vec4(0.0f), tmp->Find("Vector")->Get<glm::vec4>());
			Assert::AreEqual(glm::mat4(0.0f), tmp->Find("Matrix")->Get<glm::mat4>());
			Assert::AreEqual("0"s, tmp->Find("String")->Get<std::string>());
			Assert::AreEqual(0, tmp->Find("Rtti")->Get<RTTI*>()->As<Foo>()->Data());			
			delete tmp;

			tmp = derived->Clone();
			Assert::AreEqual(0, tmp->Find("Integer")->Get<int>());
			Assert::AreEqual(0.0f, tmp->Find("Float")->Get<float>());
			Assert::AreEqual(glm::vec4(0.0f), tmp->Find("Vector")->Get<glm::vec4>());
			Assert::AreEqual(glm::mat4(0.0f), tmp->Find("Matrix")->Get<glm::mat4>());
			Assert::AreEqual("0"s, tmp->Find("String")->Get<std::string>());
			Assert::AreEqual(0, tmp->Find("Rtti")->Get<RTTI*>()->As<Foo>()->Data());	
			delete tmp;
			
			delete scope;
			delete derived;
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState AttributedFooTest::sStartMemState;
}