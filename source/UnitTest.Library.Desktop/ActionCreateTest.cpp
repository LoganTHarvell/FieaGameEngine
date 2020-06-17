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
			
			const auto* prototype = actionCreate.Find(ActionCreate::EntityPrototypeKey);
			Assert::IsNotNull(prototype);
			Assert::AreEqual(Scope::Types::Scope, prototype->Type());
		}

		TEST_METHOD(Clone)
		{
			ActionCreate actionCreate;
			Scope* clone = actionCreate.Clone();

			const bool notNull = clone;
			const bool isActionCreate = notNull ? clone->Is(ActionCreate::TypeIdClass()) : false;
			const bool equal = *actionCreate.As<Entity>() == *clone->As<Entity>();

			delete clone;

			Assert::IsTrue(notNull && isActionCreate && equal);
		}

		TEST_METHOD(Update)
		{
			World world;
			Entity& entity = world.CreateChild("Entity", "Root");
			
			/* Creates Entity */

			Entity* create = &entity.CreateChild("ActionCreate"s, "CreateEntity"s);
			Assert::IsTrue(create->Is(ActionCreate::TypeIdClass()));

			Entity addedEntity("Added");
			*create->As<ActionCreate>()->Find(ActionCreate::EntityPrototypeKey) = addedEntity.As<Scope>();

			world.Update();

			Assert::AreEqual(1_z, entity.FindChild("Added")->Size());
			Assert::AreEqual(addedEntity, *entity.FindChild("Added"));

			entity.DestroyChild(*create);
			entity.DestroyChild(*entity.FindChild("Added"));

			/* Creates Action */

			create = &entity.CreateChild("ActionCreate"s, "CreateAction"s);
			Assert::IsTrue(create->Is(ActionCreate::TypeIdClass()));

			ActionCreate addedAction("Added");
			*create->As<ActionCreate>()->Find(ActionCreate::EntityPrototypeKey) = addedAction.As<Scope>();

			Assert::AreEqual(1_z, entity.ChildCount());

			world.Update();

			Assert::AreEqual(2_z, entity.ChildCount());
			Assert::IsTrue(entity.FindChild("Added")->Is(ActionCreate::TypeIdClass()));
			Assert::AreEqual(addedAction, *entity.FindChild("Added")->As<ActionCreate>());
		}

		TEST_METHOD(ToString)
		{
			const ActionCreate actionCreate("Create");
			Assert::AreEqual("Create (ActionCreate)"s, actionCreate.ToString());
		}

	private:
		static _CrtMemState sStartMemState;

		ActionCreateFactory actionCreateFactory;
		EntityFactory entityFactory;
	};

	_CrtMemState ActionCreateTest::sStartMemState;
}