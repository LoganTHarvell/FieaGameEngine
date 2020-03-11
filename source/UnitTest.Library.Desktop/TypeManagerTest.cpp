#include "pch.h"

#include "ToStringSpecialization.h"
#include "TypeManager.h"
#include "AttributedFoo.h"
#include "DerivedAttributedFoo.h"
#include "AttributedBar.h"


using namespace std::string_literals;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;


namespace UnitTestLibraryDesktop
{
	TEST_CLASS(TypeManagerTest)
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

		TEST_METHOD(InstanceManagement)
		{
			Assert::IsNull(TypeManager::Instance());
			TypeManager::Create();
			Assert::IsNotNull(TypeManager::Instance());
			TypeManager::Destroy();
			Assert::IsNull(TypeManager::Instance());
		}

		TEST_METHOD(Registry)
		{
			TypeManager::Create();
			
			Assert::ExpectException<std::runtime_error>([] { TypeManager::Instance()->Register<DerivedAttributedFoo>(); });
			Assert::ExpectException<std::runtime_error>([] { RegisterType<DerivedAttributedFoo>(); });

			TypeManager::Instance()->Register<AttributedFoo>();
			Assert::IsTrue(TypeManager::Instance()->IsRegistered(AttributedFoo::TypeIdClass()));
 			TypeManager::Instance()->Deregister(AttributedFoo::TypeIdClass());
 			Assert::IsFalse(TypeManager::Instance()->IsRegistered(AttributedFoo::TypeIdClass()));

			RegisterType<AttributedFoo>();
			Assert::IsTrue(TypeManager::Instance()->IsRegistered(AttributedFoo::TypeIdClass()));
 			TypeManager::Instance()->Deregister(AttributedFoo::TypeIdClass());
 			Assert::IsFalse(TypeManager::Instance()->IsRegistered(AttributedFoo::TypeIdClass()));

			RegisterType<AttributedFoo>();
			Assert::ExpectException<std::runtime_error>([] { RegisterType<AttributedFoo>(); });
			RegisterType<AttributedBar>();
			RegisterType<DerivedAttributedFoo>();
			
			TypeManager::Instance()->Clear();
			
			Assert::IsFalse(TypeManager::Instance()->IsRegistered(AttributedFoo::TypeIdClass()));
			Assert::IsFalse(TypeManager::Instance()->IsRegistered(DerivedAttributedFoo::TypeIdClass()));
			Assert::IsFalse(TypeManager::Instance()->IsRegistered(AttributedBar::TypeIdClass()));
		}

		TEST_METHOD(RehashRegistry)
		{
			TypeManager::Create();
			TypeManager* instance = TypeManager::Instance();

			instance->RegistryRehash(10);
			Assert::AreEqual(0.0f, instance->RegistryLoadFactor());

			instance->Register<AttributedFoo>();
			Assert::AreEqual(1 / 10.0f, instance->RegistryLoadFactor());

			instance->RegistryRehash(TypeManager::Registry::DefaultBucketCount);
			Assert::AreEqual(1.0f / TypeManager::Registry::DefaultBucketCount, instance->RegistryLoadFactor());
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState TypeManagerTest::sStartMemState;
}