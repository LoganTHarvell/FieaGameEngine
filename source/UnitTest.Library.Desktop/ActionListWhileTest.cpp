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

			RegisterType<Entity>();
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

			Assert::IsTrue(incrementA.Is(Entity::TypeIdClass()));
			Assert::IsTrue(incrementA.Equals(&incrementB));

			Entity* newListWhile = new ActionListWhile();
			bool isAction = newListWhile->Is(Entity::TypeIdClass());

			Entity* createdActionListWhile = isAction ? createdActionListWhile = newListWhile->CreateAs<Entity>() : nullptr;
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

			const auto condition = actionListWhile.Find(ActionListWhile::ConditionKey);
			Assert::IsNotNull(condition);
			Assert::AreEqual(Scope::Types::Integer, condition->Type());
			Assert::AreEqual(0, condition->Get<int>());
		}

		TEST_METHOD(Clone)
		{
			ActionListWhile actionListWhile;
			Scope* clone = actionListWhile.Clone();

			const bool notNull = clone;
			const bool isActionListWhile = notNull ? clone->Is(ActionListWhile::TypeIdClass()) : false;
			const bool equal = *actionListWhile.As<Entity>() == *clone->As<Entity>();

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

			actionListWhile.AddChild(*decrementCondition);

			ActionIncrement* incrementCounter = new ActionIncrement("IncrementCount");
			*incrementCounter->Find(ActionIncrement::OperandKey) = "LoopCount"s;

			actionListWhile.AddChild(*incrementCounter);

			WorldState worldState;
			actionListWhile.Update(worldState);

			Assert::AreEqual(0, condition);
			Assert::AreEqual(10, loopCount);
		}

		TEST_METHOD(ToString)
		{
			const ActionListWhile actionListWhile("ListWhile");
			Assert::AreEqual("ListWhile (ActionListWhile)"s, actionListWhile.ToString());
		}

	private:
		static _CrtMemState sStartMemState;

		ActionListWhileFactory actionListWhileFactory;
		ActionIncrementFactory actionIncrementFactory;
	};

	_CrtMemState ActionListWhileTest::sStartMemState;
}