 #include "pch.h"

#include "ToStringSpecialization.h"
#include "ActionDestroy.h"
#include "Entity.h"

using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;


namespace EntitySystemTests::ActionTests
{
	TEST_CLASS(ActionDestroyTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			TypeManager::Create();
			
			RegisterType<Entity>();
			RegisterType<World>();

			RegisterType<ActionDestroy>();


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

		TEST_METHOD(RTTITest)
		{
			ActionDestroy destroyA;
			ActionDestroy destroyB;

			Assert::IsTrue(destroyA.Is(Entity::TypeIdClass()));
			Assert::IsTrue(destroyA.Equals(&destroyB));

			Entity* newCreate = new ActionDestroy();
			bool isAction = newCreate->Is(Entity::TypeIdClass());

			Entity* destroyedActionDestroy = isAction ? destroyedActionDestroy = newCreate->CreateAs<Entity>() : nullptr;
			bool wasDestroyed = destroyedActionDestroy != nullptr;

			bool  isActionDestroy = wasDestroyed ? destroyedActionDestroy->Is(ActionDestroy::TypeIdClass()) : false;

			delete newCreate;
			delete destroyedActionDestroy;

			Assert::IsTrue(isAction && wasDestroyed && isActionDestroy);
		}

		TEST_METHOD(Constructor)
		{
			ActionDestroy actionDestroy("ActionDestroy");
			Assert::AreEqual("ActionDestroy"s, actionDestroy.Name());
			Assert::IsNotNull(actionDestroy.Find("Name"));
			Assert::AreEqual("ActionDestroy"s, actionDestroy.Find("Name")->Get<std::string>());

			auto actionName = actionDestroy.Find(actionDestroy.AttributeKey);
			Assert::IsNotNull(actionName);
			Assert::AreEqual(Scope::Types::String, actionName->Type());
			Assert::AreEqual(std::string(), actionName->Get<std::string>());
		}

		TEST_METHOD(Clone)
		{
			ActionDestroy actionDestroy;
			Scope* clone = actionDestroy.Clone();

			bool notNull = clone;
			bool isActionDestroy = notNull ? clone->Is(ActionDestroy::TypeIdClass()) : false;
			bool equal = *actionDestroy.As<Entity>() == *clone->As<Entity>();

			delete clone;

			Assert::IsTrue(notNull && isActionDestroy && equal);
		}

		TEST_METHOD(Update)
		{
			World world;
			Entity& entity = world.CreateChild("Entity", "Sector").CreateChild("Entity", "Entity");
			
			Entity* destroyEntity = new Entity("EntityToDestroy");
			entity.Adopt(*destroyEntity, "EntityList");

			Entity& destroy = entity.CreateChild("ActionDestroy"s, "DestroyEntity"s);
			Assert::IsTrue(destroy.Is(ActionDestroy::TypeIdClass()));

			*destroy.As<ActionDestroy>()->Find(ActionDestroy::AttributeKey) = "EntityList"s;
			*destroy.As<ActionDestroy>()->Find(ActionDestroy::TargetKey) = "EntityToDestroy"s;

			destroy = entity.CreateChild("ActionDestroy"s, "DestroyAction"s);
			Assert::IsTrue(destroy.Is(ActionDestroy::TypeIdClass()));

			*destroy.As<ActionDestroy>()->Find(ActionDestroy::AttributeKey) = Entity::ChildrenKey;
			*destroy.As<ActionDestroy>()->Find(ActionDestroy::TargetKey) = "DestroyEntity"s;

			Assert::AreEqual(2_z, entity.Children().Size());
			Assert::AreEqual(1_z, entity.Find("EntityList")->Size());

			world.Update();

			Assert::AreEqual(1_z, entity.Children().Size());
			Assert::AreEqual(0_z, entity.Find("EntityList")->Size());
		}

		TEST_METHOD(ToString)
		{
			ActionDestroy actionDestroy("Destroy");
			Assert::AreEqual("Destroy (ActionDestroy)"s, actionDestroy.ToString());
		}

	private:
		static _CrtMemState sStartMemState;

		ActionDestroyFactory actionDestroyFactory;
		EntityFactory entityFactory;
	};

	_CrtMemState ActionDestroyTest::sStartMemState;
}