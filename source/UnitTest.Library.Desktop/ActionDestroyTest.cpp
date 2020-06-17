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
			const ActionDestroy destroyA;
			ActionDestroy destroyB;

			Assert::IsTrue(destroyA.Is(Entity::TypeIdClass()));
			Assert::IsTrue(destroyA.Equals(&destroyB));

			Entity* newCreate = new ActionDestroy();
			const bool isAction = newCreate->Is(Entity::TypeIdClass());

			Entity* destroyedActionDestroy = isAction ? destroyedActionDestroy = newCreate->CreateAs<Entity>() : nullptr;
			const bool wasDestroyed = destroyedActionDestroy != nullptr;

			const bool  isActionDestroy = wasDestroyed ? destroyedActionDestroy->Is(ActionDestroy::TypeIdClass()) : false;

			delete newCreate;
			delete destroyedActionDestroy;

			Assert::IsTrue(isAction && wasDestroyed && isActionDestroy);
		}

		TEST_METHOD(Constructor)
		{
			ActionDestroy actionDestroy("ActionDestroy");
			Assert::AreEqual("ActionDestroy"s, actionDestroy.Name());

			const auto* targetKey = actionDestroy.Find(ActionDestroy::TargetKey);
			Assert::IsNotNull(targetKey);
			Assert::AreEqual(Scope::Types::String, targetKey->Type());
			Assert::AreEqual(std::string(), targetKey->Get<std::string>());
		}

		TEST_METHOD(Clone)
		{
			ActionDestroy actionDestroy;
			Scope* clone = actionDestroy.Clone();

			const bool notNull = clone;
			const bool isActionDestroy = notNull ? clone->Is(ActionDestroy::TypeIdClass()) : false;
			const bool equal = *actionDestroy.As<Entity>() == *clone->As<Entity>();

			delete clone;

			Assert::IsTrue(notNull && isActionDestroy && equal);
		}

		TEST_METHOD(Update)
		{
			World world;
			Entity& entity = world.CreateChild("Entity", "Sector").CreateChild("Entity", "Entity");
			
			Entity* destroyEntity = new Entity("EntityToDestroy");
			entity.AddChild(*destroyEntity);

			Entity& actionDestroyEntity = entity.CreateChild("ActionDestroy"s, "DestroyEntity"s);
			Assert::IsTrue(actionDestroyEntity.Is(ActionDestroy::TypeIdClass()));

			*actionDestroyEntity.As<ActionDestroy>()->Find(ActionDestroy::TargetKey) = "EntityToDestroy"s;

			Entity& actionDestroyAction = entity.CreateChild("ActionDestroy"s, "DestroyAction"s);
			Assert::IsTrue(actionDestroyAction.Is(ActionDestroy::TypeIdClass()));

			*actionDestroyAction.As<ActionDestroy>()->Find(ActionDestroy::TargetKey) = "DestroyEntity"s;

			Assert::AreEqual(3_z, entity.ChildCount());

			world.Update();

			Assert::AreEqual(1_z, entity.ChildCount());
		}

		TEST_METHOD(ToString)
		{
			const ActionDestroy actionDestroy("Destroy");
			Assert::AreEqual("Destroy (ActionDestroy)"s, actionDestroy.ToString());
		}

	private:
		static _CrtMemState sStartMemState;

		ActionDestroyFactory actionDestroyFactory;
		EntityFactory entityFactory;
	};

	_CrtMemState ActionDestroyTest::sStartMemState;
}