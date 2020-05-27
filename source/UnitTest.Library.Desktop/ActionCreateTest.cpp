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
			
			RegisterType<Entity>();
			RegisterType<World>();

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

			Assert::IsTrue(createA.Is(Entity::TypeIdClass()));
			Assert::IsTrue(createA.Equals(&createB));

			Entity* newCreate = new ActionCreate();
			bool isAction = newCreate->Is(Entity::TypeIdClass());

			Entity* createdActionCreate = isAction ? createdActionCreate = newCreate->CreateAs<Entity>() : nullptr;
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
			bool equal = *actionCreate.As<Entity>() == *clone->As<Entity>();

			delete clone;

			Assert::IsTrue(notNull && isActionCreate && equal);
		}

		TEST_METHOD(Update)
		{
			World world;
			Entity& entity = world.CreateChild("Entity", "Sector").CreateChild("Entity", "Entity");
			
			/* Creates Entity */

			Entity& create = entity.CreateChild("ActionCreate"s, "CreateEntity"s);
			Assert::IsTrue(create.Is(ActionCreate::TypeIdClass()));

			Entity addedEntity("Added");
			*create.As<ActionCreate>()->Find(ActionCreate::AttributeKey) = "NewEntity";
			*create.As<ActionCreate>()->Find(ActionCreate::NewScopeKey) = addedEntity.As<Scope>();

			world.Update();

			Assert::AreEqual(1_z, entity.Find("NewEntity")->Size());
			Assert::IsTrue(entity.Find("NewEntity")->Get<Scope*>()->Is(Entity::TypeIdClass()));
			Assert::AreEqual(*addedEntity.As<Entity>(), *entity.Find("NewEntity")->Get<Scope*>()->As<Entity>());

			delete entity.Orphan(create);

			/* Creates Entity */

			create = entity.CreateChild("ActionCreate"s, "CreateAction"s);
			Assert::IsTrue(create.Is(ActionCreate::TypeIdClass()));

			ActionCreate addedAction("Added");
			*create.As<ActionCreate>()->Find(ActionCreate::AttributeKey) = Entity::ChildrenKey;
			*create.As<ActionCreate>()->Find(ActionCreate::NewScopeKey) = addedAction.As<Scope>();

			Assert::AreEqual(1_z, entity.Children().Size());

			world.Update();

			Assert::AreEqual(2_z, entity.Children().Size());
			Assert::IsTrue(entity.Children()[1].Is(ActionCreate::TypeIdClass()));
			Assert::AreEqual(*addedAction.As<Entity>(), *entity.Children()[1].As<Entity>());
		}

		TEST_METHOD(ToString)
		{
			ActionCreate actionCreate("Create");
			Assert::AreEqual("Create (ActionCreate)"s, actionCreate.ToString());
		}

	private:
		static _CrtMemState sStartMemState;

		ActionCreateFactory actionCreateFactory;
		EntityFactory entityFactory;
	};

	_CrtMemState ActionCreateTest::sStartMemState;
}