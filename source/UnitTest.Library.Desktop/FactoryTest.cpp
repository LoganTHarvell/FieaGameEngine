#include "pch.h"

#include "ToStringSpecialization.h"

#include "Factory.h"
#include "DerivedFoo.h"


using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;
using namespace Library;

namespace UtilityTests
{
	TEST_CLASS(FactoryTest)
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

		TEST_METHOD(RehashRegistry)
		{
			ConcreteFactory(DerivedFoo, Foo)

			Factory<Foo>::RegistryRehash(10);
			Assert::AreEqual(0.0f, Factory<Foo>::RegistryLoadFactor());

			Factory<Foo>* derivedFooFactory = new DerivedFooFactory();
			Assert::AreEqual(1/10.0f, Factory<Foo>::RegistryLoadFactor());

			Factory<Foo>::RegistryRehash(Factory<Foo>::Registry::DefaultBucketCount);
			Assert::AreEqual(1.0f/Factory<Foo>::Registry::DefaultBucketCount, Factory<Foo>::RegistryLoadFactor());

			delete derivedFooFactory;
		}

		TEST_METHOD(RegisterFactory)
		{
			ConcreteFactory(DerivedFoo, Foo)

			Assert::IsFalse(Factory<Foo>::IsRegistered("DerivedFoo"s));
			Factory<Foo>* derivedFooFactory = new DerivedFooFactory();
			Assert::IsTrue(Factory<Foo>::IsRegistered("DerivedFoo"s));

			Foo* foo = Factory<Foo>::Create("DerivedFoo"s);
			Assert::IsNotNull(foo);
			Assert::IsTrue(foo->Is(DerivedFoo::TypeIdClass()));
			if (foo != nullptr) delete foo;
			
			delete derivedFooFactory;
		}

		TEST_METHOD(DeregisterFactory)
		{
			ConcreteFactory(DerivedFoo, Foo)

			Factory<Foo>* derivedFooFactory = new DerivedFooFactory();

			Foo* foo = Factory<Foo>::Create("DerivedFoo");
			if (foo != nullptr) delete foo;
			
			Assert::IsTrue(Factory<Foo>::IsRegistered("DerivedFoo"s));
			delete derivedFooFactory;
			Assert::IsFalse(Factory<Foo>::IsRegistered("DerivedFoo"s));

			Assert::IsNull(Factory<Foo>::Create("DerivedFoo"));
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState FactoryTest::sStartMemState;
}