 #include "pch.h"

#include "ToStringSpecialization.h"
#include "Action.h"
#include "ActionList.h"
#include "ActionIncrement.h"
#include "Entity.h"

using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;


namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ActionTest)
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

		TEST_METHOD(Constructor)
		{
			/* ActionList */
			ActionList actionList("ActionList");
			Assert::AreEqual("ActionList"s, actionList.Name());
			Assert::IsNotNull(actionList.Find("Name"));
			Assert::AreEqual("ActionList"s, actionList.Find("Name")->Get<std::string>());

			auto actions = actionList.Find(actionList.ActionsKey);
			Assert::IsNotNull(actions);
			Assert::AreEqual(actionList.Actions(), *actions);

			/* ActionIncrement */

			ActionIncrement actionIncrement("ActionIncrement");
			Assert::AreEqual("ActionIncrement"s, actionIncrement.Name());
			Assert::IsNotNull(actionIncrement.Find("Name"));
			Assert::AreEqual("ActionIncrement"s, actionIncrement.Find("Name")->Get<std::string>());

			auto operand = actionIncrement.Find(actionIncrement.OperandKey);
			Assert::IsNotNull(operand);
			Assert::AreEqual(Scope::Types::String, operand->Type());
			Assert::AreEqual(std::string(), operand->Get<std::string>());

			auto incrementStep = actionIncrement.Find(actionIncrement.IncrementStepKey);
			Assert::IsNotNull(incrementStep);
			Assert::AreEqual(Scope::Types::Integer, incrementStep->Type());
			Assert::AreEqual(1, incrementStep->Get<int>());
		}

		TEST_METHOD(RTTITest)
		{
			/* ActionList */

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


			/* ActionIncrement */

			ActionIncrement incrementA;
			ActionIncrement incrementB;

			Assert::IsTrue(incrementA.Is(Action::TypeIdClass()));
			Assert::IsTrue(incrementA.Equals(&incrementB));

			Action* newIncrement = new ActionIncrement();
			bool isActionIncrement = newIncrement->Is(Action::TypeIdClass());

			Action* createdActionIncrement = isAction ? createdActionIncrement = newIncrement->CreateAs<Action>() : nullptr;
			wasCreated = createdActionIncrement != nullptr;

			isActionList = wasCreated ? createdActionIncrement->Is(ActionIncrement::TypeIdClass()) : false;
			
			delete newIncrement;
			delete createdActionIncrement;

			Assert::IsTrue(isAction);
			Assert::IsTrue(wasCreated);
			Assert::IsTrue(isActionIncrement);
		}

		TEST_METHOD(ToString)
		{
			/* ActionList */
			ActionList actionList("List");
			Assert::AreEqual("List (ActionList)"s, actionList.ToString());

			/* ActionIncrement */
			ActionIncrement actionIncrement("Increment");
			Assert::AreEqual("Increment (ActionIncrement)"s, actionIncrement.ToString());

			/* Base Implementation */
			Assert::AreEqual("Increment (Action)"s, actionIncrement.Action::ToString());
		}

		TEST_METHOD(Accessors)
		{
			ActionList emptyAction;
			emptyAction.SetName("EmptyAction");
			Assert::AreEqual("EmptyAction"s, emptyAction.Name());
			Assert::IsNull(emptyAction.GetEntity());

			Entity entity;
			Action* action = entity.CreateAction("ActionList", "List");
			Assert::AreEqual("List"s, action->Name());
			Assert::AreEqual(entity, *action->GetEntity());

			ActionIncrement increment;
			action->As<ActionList>()->Actions().PushBack(increment.As<Scope>());

			Entity adopter;
			action->SetEntity(&adopter);
			Assert::IsNotNull(action->GetEntity());
			Assert::AreEqual(adopter, *action->GetEntity());
			Assert::IsNull(entity.FindScope(*action).first);

			const ActionList copy = *action->As<ActionList>();

			Assert::AreEqual(1_z, copy.Actions().Size());
			Assert::AreEqual(increment.As<Action>(), copy.Actions().Get<Scope*>()->As<Action>());

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

			Action* actionList = entity.CreateAction("ActionList"s, "List");

			Action* increment = entity.CreateAction("ActionIncrement"s, "Increment1"s);
			Assert::IsNotNull(increment);
			Assert::IsTrue(increment->Is(ActionIncrement::TypeIdClass()));

			ActionIncrement* castedIncrement = static_cast<ActionIncrement*>(increment);
			*castedIncrement->Find(castedIncrement->OperandKey) = "Integer1"s;

			ActionIncrement increment2("Increment2");
			*increment2.Find(increment2.OperandKey) = "Integer2";
			actionList->As<ActionList>()->Actions().PushBack(increment2.As<Scope>());

			ActionIncrement increment3("Increment3");
			*increment3.Find(increment3.OperandKey) = "Integer3";
			actionList->As<ActionList>()->Actions().PushBack(increment3.As<Scope>());


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

		TEST_METHOD(Clone)
		{
			ActionList actionList;
			ActionIncrement actionIncrement;
			actionList.Actions().PushBack(actionIncrement.As<Scope>());

			Scope* clone = actionList.Clone();
			bool notNull = clone;
			bool isActionList = notNull ? clone->Is(ActionList::TypeIdClass()) : false;
			bool equal = *actionList.As<Action>() == *clone->As<Action>();

			delete clone;

			Assert::IsTrue(notNull && isActionList && equal);

			clone = actionIncrement.Clone();
			notNull = clone;
			bool isActionIncrement = notNull ? clone->Is(ActionIncrement::TypeIdClass()) : false;
			equal = *actionIncrement.As<Action>() == *clone->As<Action>();

			delete clone;

			Assert::IsTrue(notNull && isActionIncrement && equal);
		}

	private:
		static _CrtMemState sStartMemState;

		ActionListFactory actionListFactory;
		ActionIncrementFactory actionIncrementFactory;
		EntityFactory entityFactory;
	};

	_CrtMemState ActionTest::sStartMemState;
}