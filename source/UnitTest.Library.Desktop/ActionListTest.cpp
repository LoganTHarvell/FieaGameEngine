 #include "pch.h"

#include "ToStringSpecialization.h"
#include "ActionList.h"
#include "ActionIncrement.h"
#include "Entity.h"

using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;


namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ActionListTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			TypeManager::Create();
			
			RegisterType<Action>();
			RegisterType<ActionList>();
			RegisterType<ActionIncrement>();

			RegisterType<Entity>();

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
			ActionList listA;
			ActionList listB;

			Assert::IsTrue(listA.Is(Action::TypeIdClass()));
			Assert::IsTrue(listA.Equals(&listB));

			Action* newList = new ActionList();
			bool isAction = newList->Is(Action::TypeIdClass());

			Action* createdActionList = isAction ? createdActionList = newList->CreateAs<Action>() : nullptr;
			bool wasCreated = createdActionList != nullptr;

			bool isActionList = wasCreated ? createdActionList->Is(ActionList::TypeIdClass()) : false;

			delete newList;
			delete createdActionList;

			Assert::IsTrue(isAction);
			Assert::IsTrue(wasCreated);
			Assert::IsTrue(isActionList);
		}

		TEST_METHOD(Constructor)
		{
			ActionList actionList("ActionList");
			Assert::AreEqual("ActionList"s, actionList.Name());
			Assert::IsNotNull(actionList.Find("Name"));
			Assert::AreEqual("ActionList"s, actionList.Find("Name")->Get<std::string>());

			auto actions = actionList.Find(actionList.ActionsKey);
			Assert::IsNotNull(actions);
			Assert::AreEqual(actionList.Actions(), *actions);
		}

		TEST_METHOD(Clone)
		{
			ActionList actionList;
			ActionIncrement* actionIncrement = new ActionIncrement();
			actionList.Adopt(*actionIncrement, ActionList::ActionsKey);

			Scope* clone = actionList.Clone();
			bool notNull = clone;
			bool isActionList = notNull ? clone->Is(ActionList::TypeIdClass()) : false;
			bool equal = *actionList.As<Action>() == *clone->As<Action>();

			delete clone;

			Assert::IsTrue(notNull && isActionList && equal);
		}

		TEST_METHOD(Accessors)
		{
			Entity entity;
			Action* action = entity.CreateAction("ActionList", "List");
			Assert::IsNotNull(action);
			Assert::AreEqual(0_z, action->As<ActionList>()->Actions().Size());

			ActionIncrement* actionIncrement = new ActionIncrement();
			action->Adopt(*actionIncrement, ActionList::ActionsKey);

			const ActionList copy = *action->As<ActionList>();

			Assert::AreEqual(1_z, copy.Actions().Size());
			Assert::AreEqual(action->As<ActionList>()->Actions(), copy.Actions());

			action->SetEntity(nullptr);
			Assert::IsNull(action->GetEntity());
			delete action;
		}

		TEST_METHOD(Update)
		{
			Entity entity;
			int& integer1 = (entity.Append("Integer1") = 0).Get<int>();
			int& integer2 = (entity.Append("Integer2") = 0).Get<int>();
			int& integer3 = (entity.Append("Integer3") = 0).Get<int>();

			/* Action Increment */

			Action* increment = entity.CreateAction("ActionIncrement"s, "Increment1"s);
			Assert::IsNotNull(increment);
			Assert::IsTrue(increment->Is(ActionIncrement::TypeIdClass()));

			ActionIncrement* castedIncrement = static_cast<ActionIncrement*>(increment);
			*castedIncrement->Find(castedIncrement->OperandKey) = "Integer1"s;

			/* Action List */

			Action* actionList = entity.CreateAction("ActionList"s, "List");

			ActionIncrement* increment2 = new ActionIncrement("Increment2");
			*increment2->Find(increment2->OperandKey) = "Integer2";
			actionList->Adopt(*increment2->As<Scope>(), ActionList::ActionsKey);

			ActionIncrement* increment3 = new ActionIncrement("Increment3");
			*increment3->Find(increment3->OperandKey) = "Integer3";
			actionList->Adopt(*increment3->As<Scope>(), ActionList::ActionsKey);

			/* Updates */

			WorldState worldState;
			worldState.Entity = &entity;

			Assert::AreEqual(0, integer1);
			Assert::AreEqual(0, integer2);
			Assert::AreEqual(0, integer3);

			entity.Update(worldState);

			Assert::AreEqual(1, integer1);
			Assert::AreEqual(1, integer2);
			Assert::AreEqual(1, integer3);

			entity.Update(worldState);

			Assert::AreEqual(2, integer1);
			Assert::AreEqual(2, integer2);
			Assert::AreEqual(2, integer3);

			castedIncrement->Update(worldState);

			Assert::AreEqual(3, integer1);
			Assert::AreEqual(2, integer2);
			Assert::AreEqual(2, integer3);
		}

		TEST_METHOD(ToString)
		{
			ActionList actionList("List");
			Assert::AreEqual("List (ActionList)"s, actionList.ToString());
		}

	private:
		static _CrtMemState sStartMemState;

		ActionListFactory actionListFactory;
		ActionIncrementFactory actionIncrementFactory;
		EntityFactory entityFactory;
	};

	_CrtMemState ActionListTest::sStartMemState;
}