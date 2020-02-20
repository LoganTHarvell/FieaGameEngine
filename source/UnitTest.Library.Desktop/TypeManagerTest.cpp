#include "pch.h"

#include "ToStringSpecialization.h"
#include "TypeManager.h"
#include "AttributedFoo.h"
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
			
			Assert::ExpectException<std::runtime_error>([]
				{
					TypeManager::Instance()->Register(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures(), RTTI::TypeIdClass());
				});

			REGISTER_TYPE(AttributedFoo, Attributed);
			Assert::IsTrue(TypeManager::Instance()->IsRegistered(AttributedFoo::TypeIdClass()));

			Assert::ExpectException<std::runtime_error>([] 
				{
					TypeManager::Instance()->Register(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures(), Attributed::TypeIdClass());
				});
 	
 			TypeManager::Instance()->Deregister(AttributedFoo::TypeIdClass());
 			Assert::IsFalse(TypeManager::Instance()->IsRegistered(AttributedFoo::TypeIdClass()));
 		
			TypeManager::Instance()->Register(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures(), Attributed::TypeIdClass());
			TypeManager::Instance()->Register(AttributedBar::TypeIdClass(), AttributedBar::Signatures(), Attributed::TypeIdClass());
 			TypeManager::Instance()->Clear();
			
			Assert::IsFalse(TypeManager::Instance()->IsRegistered(AttributedFoo::TypeIdClass()));
			Assert::IsFalse(TypeManager::Instance()->IsRegistered(AttributedBar::TypeIdClass()));
		}


	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState TypeManagerTest::sStartMemState;
}