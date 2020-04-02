 #include "pch.h"

#include "ToStringSpecialization.h"
#include "ActionDestroyAction.h"
#include "Entity.h"

using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;


namespace EntitySystemTests::ActionTests
{
	TEST_CLASS(ActionDestroyActionTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			TypeManager::Create();
			
			RegisterType<World>();
			RegisterType<Sector>();
			RegisterType<Entity>();

			RegisterType<Action>();
			RegisterType<ActionDestroyAction>();


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
			ActionDestroyAction destroyA;
			ActionDestroyAction destroyB;

			Assert::IsTrue(destroyA.Is(Action::TypeIdClass()));
			Assert::IsTrue(destroyA.Equals(&destroyB));

			Action* newCreate = new ActionDestroyAction();
			bool isAction = newCreate->Is(Action::TypeIdClass());

			Action* destroyedActionDestroyAction = isAction ? destroyedActionDestroyAction = newCreate->CreateAs<Action>() : nullptr;
			bool wasDestroyed = destroyedActionDestroyAction != nullptr;

			bool  isActionDestroyAction = wasDestroyed ? destroyedActionDestroyAction->Is(ActionDestroyAction::TypeIdClass()) : false;

			delete newCreate;
			delete destroyedActionDestroyAction;

			Assert::IsTrue(isAction && wasDestroyed && isActionDestroyAction);
		}

		TEST_METHOD(Constructor)
		{
			ActionDestroyAction actionDestroy("ActionDestroyAction");
			Assert::AreEqual("ActionDestroyAction"s, actionDestroy.Name());
			Assert::IsNotNull(actionDestroy.Find("Name"));
			Assert::AreEqual("ActionDestroyAction"s, actionDestroy.Find("Name")->Get<std::string>());

			auto actionName = actionDestroy.Find(actionDestroy.AttributeNameKey);
			Assert::IsNotNull(actionName);
			Assert::AreEqual(Scope::Types::String, actionName->Type());
			Assert::AreEqual(std::string(), actionName->Get<std::string>());
		}

		TEST_METHOD(Clone)
		{
			ActionDestroyAction actionDestroy;
			Scope* clone = actionDestroy.Clone();

			bool notNull = clone;
			bool isActionDestroyAction = notNull ? clone->Is(ActionDestroyAction::TypeIdClass()) : false;
			bool equal = *actionDestroy.As<Action>() == *clone->As<Action>();

			delete clone;

			Assert::IsTrue(notNull && isActionDestroyAction && equal);
		}

		TEST_METHOD(Update)
		{
			World world;
			Entity* entity = world.CreateSector("Sector").CreateEntity("Entity", "Entity");
			
			Entity* destroyEntity = new Entity("EntityToDestroy");
			entity->Adopt(*destroyEntity, "EntityList");

			Action* destroy = entity->CreateAction("ActionDestroyAction"s, "DestroyEntity"s);
			Assert::IsNotNull(destroy);
			Assert::IsTrue(destroy->Is(ActionDestroyAction::TypeIdClass()));

			*destroy->As<ActionDestroyAction>()->Find(ActionDestroyAction::AttributeNameKey) = "EntityList"s;
			*destroy->As<ActionDestroyAction>()->Find(ActionDestroyAction::TargetKey) = "EntityToDestroy"s;

			destroy = entity->CreateAction("ActionDestroyAction"s, "DestroyAction"s);
			Assert::IsNotNull(destroy);
			Assert::IsTrue(destroy->Is(ActionDestroyAction::TypeIdClass()));

			*destroy->As<ActionDestroyAction>()->Find(ActionDestroyAction::AttributeNameKey) = Entity::ActionsKey;
			*destroy->As<ActionDestroyAction>()->Find(ActionDestroyAction::TargetKey) = "DestroyEntity"s;

			Assert::AreEqual(2_z, entity->Actions().Size());
			Assert::AreEqual(1_z, entity->Find("EntityList")->Size());

			world.Update();

			Assert::AreEqual(1_z, entity->Actions().Size());
			Assert::AreEqual(0_z, entity->Find("EntityList")->Size());
		}

		TEST_METHOD(ToString)
		{
			ActionDestroyAction actionDestroy("Destroy");
			Assert::AreEqual("Destroy (ActionDestroyAction)"s, actionDestroy.ToString());
		}

	private:
		static _CrtMemState sStartMemState;

		ActionDestroyActionFactory actionDestroyFactory;
		SectorFactory sectorFactory;
		EntityFactory entityFactory;
	};

	_CrtMemState ActionDestroyActionTest::sStartMemState;
}