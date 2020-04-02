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
			
			RegisterType<World>();
			RegisterType<Sector>();
			RegisterType<Entity>();

			RegisterType<Action>();
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

			Assert::IsTrue(destroyA.Is(Action::TypeIdClass()));
			Assert::IsTrue(destroyA.Equals(&destroyB));

			Action* newCreate = new ActionDestroy();
			bool isAction = newCreate->Is(Action::TypeIdClass());

			Action* destroyedActionDestroy = isAction ? destroyedActionDestroy = newCreate->CreateAs<Action>() : nullptr;
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

			auto actionName = actionDestroy.Find(actionDestroy.AttributeNameKey);
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
			bool equal = *actionDestroy.As<Action>() == *clone->As<Action>();

			delete clone;

			Assert::IsTrue(notNull && isActionDestroy && equal);
		}

		TEST_METHOD(Update)
		{
			World world;
			Entity* entity = world.CreateSector("Sector").CreateEntity("Entity", "Entity");
			
			Entity* destroyEntity = new Entity("EntityToDestroy");
			entity->Adopt(*destroyEntity, "EntityList");

			Action* destroy = entity->CreateAction("ActionDestroy"s, "DestroyEntity"s);
			Assert::IsNotNull(destroy);
			Assert::IsTrue(destroy->Is(ActionDestroy::TypeIdClass()));

			*destroy->As<ActionDestroy>()->Find(ActionDestroy::AttributeNameKey) = "EntityList"s;
			*destroy->As<ActionDestroy>()->Find(ActionDestroy::TargetKey) = "EntityToDestroy"s;

			destroy = entity->CreateAction("ActionDestroy"s, "DestroyAction"s);
			Assert::IsNotNull(destroy);
			Assert::IsTrue(destroy->Is(ActionDestroy::TypeIdClass()));

			*destroy->As<ActionDestroy>()->Find(ActionDestroy::AttributeNameKey) = Entity::ActionsKey;
			*destroy->As<ActionDestroy>()->Find(ActionDestroy::TargetKey) = "DestroyEntity"s;

			Assert::AreEqual(2_z, entity->Actions().Size());
			Assert::AreEqual(1_z, entity->Find("EntityList")->Size());

			world.Update();

			Assert::AreEqual(1_z, entity->Actions().Size());
			Assert::AreEqual(0_z, entity->Find("EntityList")->Size());
		}

		TEST_METHOD(ToString)
		{
			ActionDestroy actionDestroy("Destroy");
			Assert::AreEqual("Destroy (ActionDestroy)"s, actionDestroy.ToString());
		}

	private:
		static _CrtMemState sStartMemState;

		ActionDestroyFactory actionDestroyFactory;
		SectorFactory sectorFactory;
		EntityFactory entityFactory;
	};

	_CrtMemState ActionDestroyTest::sStartMemState;
}