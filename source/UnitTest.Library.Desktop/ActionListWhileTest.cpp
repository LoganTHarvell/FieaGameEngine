 #include "pch.h"

#include "ToStringSpecialization.h"
#include "ActionListWhile.h"
#include "ActionIncrement.h"

using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;


namespace EntitySystemTests::ActionTests
{
	TEST_CLASS(ActionListWhileTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			TypeManager::Create();
			
			RegisterType<Action>();
			RegisterType<ActionList>();
			RegisterType<ActionListWhile>();
			RegisterType<ActionIncrement>();

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
			ActionListWhile incrementA;
			ActionListWhile incrementB;

			Assert::IsTrue(incrementA.Is(Action::TypeIdClass()));
			Assert::IsTrue(incrementA.Equals(&incrementB));

			Action* newListWhile = new ActionListWhile();
			bool isAction = newListWhile->Is(Action::TypeIdClass());

			Action* createdActionListWhile = isAction ? createdActionListWhile = newListWhile->CreateAs<Action>() : nullptr;
			bool wasCreated = createdActionListWhile != nullptr;

			bool  isActionListWhile = wasCreated ? createdActionListWhile->Is(ActionListWhile::TypeIdClass()) : false;

			delete newListWhile;
			delete createdActionListWhile;

			Assert::IsTrue(isAction && wasCreated && isActionListWhile);
		}

		TEST_METHOD(Constructor)
		{
			ActionListWhile actionListWhile("ActionListWhile");
			Assert::AreEqual("ActionListWhile"s, actionListWhile.Name());
			Assert::IsNotNull(actionListWhile.Find("Name"));
			Assert::AreEqual("ActionListWhile"s, actionListWhile.Find("Name")->Get<std::string>());

			auto condition = actionListWhile.Find(actionListWhile.ConditionKey);
			Assert::IsNotNull(condition);
			Assert::AreEqual(Scope::Types::Integer, condition->Type());
			Assert::AreEqual(0, condition->Get<int>());
		}

		TEST_METHOD(Clone)
		{
			ActionListWhile actionListWhile;
			Scope* clone = actionListWhile.Clone();

			bool notNull = clone;
			bool isActionListWhile = notNull ? clone->Is(ActionListWhile::TypeIdClass()) : false;
			bool equal = *actionListWhile.As<Action>() == *clone->As<Action>();

			delete clone;

			Assert::IsTrue(notNull && isActionListWhile && equal);
		}

		TEST_METHOD(Update)
		{
			ActionListWhile actionListWhile;
			int& condition = (*actionListWhile.Find("Condition") = 10).Get<int>();
			int& loopCount = (actionListWhile.Append("LoopCount") = 0).Get<int>();

			ActionIncrement* decrementCondition = new ActionIncrement("DecrementCondition");
			*decrementCondition->Find(ActionIncrement::OperandKey) = ActionListWhile::ConditionKey;
			*decrementCondition->Find(ActionIncrement::IncrementStepKey) = -1;

			actionListWhile.Adopt(*decrementCondition, ActionListWhile::ActionsKey);

			ActionIncrement* incrementCounter = new ActionIncrement("IncrementCount");
			*incrementCounter->Find(ActionIncrement::OperandKey) = "LoopCount"s;

			actionListWhile.Adopt(*incrementCounter, ActionListWhile::ActionsKey);

			WorldState worldState;
			actionListWhile.Update(worldState);

			Assert::AreEqual(0, condition);
			Assert::AreEqual(10, loopCount);
		}

		TEST_METHOD(ToString)
		{
			ActionListWhile actionListWhile("ListWhile");
			Assert::AreEqual("ListWhile (ActionListWhile)"s, actionListWhile.ToString());
		}

	private:
		static _CrtMemState sStartMemState;

		ActionListWhileFactory actionListWhileFactory;
		ActionIncrementFactory actionIncrementFactory;
	};

	_CrtMemState ActionListWhileTest::sStartMemState;
}