 #include "pch.h"

#include "ToStringSpecialization.h"
#include "ActionCreate.h"
#include "Entity.h"

using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;


namespace EntitySystemTests::ActionTests
{
	TEST_CLASS(ActionCreateTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			TypeManager::Create();
			
			RegisterType<World>();
			RegisterType<Sector>();
			RegisterType<Entity>();

			RegisterType<Action>();
			RegisterType<ActionCreate>();


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
			ActionCreate createA;
			ActionCreate createB;

			Assert::IsTrue(createA.Is(Action::TypeIdClass()));
			Assert::IsTrue(createA.Equals(&createB));

			Action* newCreate = new ActionCreate();
			bool isAction = newCreate->Is(Action::TypeIdClass());

			Action* createdActionCreate = isAction ? createdActionCreate = newCreate->CreateAs<Action>() : nullptr;
			bool wasCreated = createdActionCreate != nullptr;

			bool  isActionCreate = wasCreated ? createdActionCreate->Is(ActionCreate::TypeIdClass()) : false;

			delete newCreate;
			delete createdActionCreate;

			Assert::IsTrue(isAction && wasCreated && isActionCreate);
		}

		TEST_METHOD(Constructor)
		{
			ActionCreate actionCreate("ActionCreate");
			Assert::AreEqual("ActionCreate"s, actionCreate.Name());
			Assert::IsNotNull(actionCreate.Find("Name"));
			Assert::AreEqual("ActionCreate"s, actionCreate.Find("Name")->Get<std::string>());

			auto actionAttributeName = actionCreate.Find(actionCreate.AttributeKey);
			Assert::IsNotNull(actionAttributeName);
			Assert::AreEqual(Scope::Types::String, actionAttributeName->Type());
			Assert::AreEqual(std::string(), actionAttributeName->Get<std::string>());

			auto action = actionCreate.Find(actionCreate.NewScopeKey);
			Assert::IsNotNull(action);
			Assert::AreEqual(Scope::Types::Scope, action->Type());
		}

		TEST_METHOD(Clone)
		{
			ActionCreate actionCreate;
			Scope* clone = actionCreate.Clone();

			bool notNull = clone;
			bool isActionCreate = notNull ? clone->Is(ActionCreate::TypeIdClass()) : false;
			bool equal = *actionCreate.As<Action>() == *clone->As<Action>();

			delete clone;

			Assert::IsTrue(notNull && isActionCreate && equal);
		}

		TEST_METHOD(Update)
		{
			World world;
			Entity* entity = world.CreateSector("Sector").CreateEntity("Entity", "Entity");
			
			/* Creates Entity */

			Action* create = entity->CreateAction("ActionCreate"s, "CreateEntity"s);
			Assert::IsNotNull(create);
			Assert::IsTrue(create->Is(ActionCreate::TypeIdClass()));

			Entity addedEntity("Added");
			*create->As<ActionCreate>()->Find(ActionCreate::AttributeKey) = "NewEntity";
			*create->As<ActionCreate>()->Find(ActionCreate::NewScopeKey) = addedEntity.As<Scope>();

			world.Update();
			delete entity->Orphan(*create);
			world.Update();

			Assert::IsNotNull(entity->Find("NewEntity"));
			Assert::AreEqual(1_z, entity->Find("NewEntity")->Size());
			Assert::IsTrue(entity->Find("NewEntity")->Get<Scope*>()->Is(Entity::TypeIdClass()));
			Assert::AreEqual(*addedEntity.As<Entity>(), *entity->Find("NewEntity")->Get<Scope*>()->As<Entity>());

			/* Creates Action */

			create = entity->CreateAction("ActionCreate"s, "CreateAction"s);
			Assert::IsNotNull(create);
			Assert::IsTrue(create->Is(ActionCreate::TypeIdClass()));

			ActionCreate addedAction("Added");
			*create->As<ActionCreate>()->Find(ActionCreate::AttributeKey) = Entity::ActionsKey;
			*create->As<ActionCreate>()->Find(ActionCreate::NewScopeKey) = addedAction.As<Scope>();

			Assert::AreEqual(1_z, entity->Actions().Size());

			world.Update();
			delete entity->Orphan(*create);
			world.Update();

			Assert::AreEqual(1_z, entity->Actions().Size());
			Assert::IsTrue(entity->Actions()[0].Is(ActionCreate::TypeIdClass()));
			Assert::AreEqual(*addedAction.As<Action>(), *entity->Actions()[0].As<Action>());
		}

		TEST_METHOD(ToString)
		{
			ActionCreate actionCreate("Create");
			Assert::AreEqual("Create (ActionCreate)"s, actionCreate.ToString());
		}

	private:
		static _CrtMemState sStartMemState;

		ActionCreateFactory actionCreateFactory;
		SectorFactory sectorFactory;
		EntityFactory entityFactory;
	};

	_CrtMemState ActionCreateTest::sStartMemState;
}