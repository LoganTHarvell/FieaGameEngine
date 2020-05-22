 #include "pch.h"

#include "ToStringSpecialization.h"
#include "Action.h"
#include "ActionIncrement.h"
#include "Entity.h"

using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;


namespace EntitySystemTests::ActionTests
{
	TEST_CLASS(ActionTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			TypeManager::Create();
			
			RegisterType<Action>();
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
			ActionIncrement incrementA;
			ActionIncrement incrementB;

			Assert::IsTrue(incrementA.Is(Action::TypeIdClass()));
			Assert::IsTrue(incrementA.Action::Equals(incrementB.As<Action>()));
		}

		TEST_METHOD(Constructor)
		{
			ActionIncrement actionIncrement("ActionIncrement");
			Assert::AreEqual("ActionIncrement"s, actionIncrement.Name());
			Assert::IsNotNull(actionIncrement.Find("Name"));
			Assert::AreEqual("ActionIncrement"s, actionIncrement.Find("Name")->Get<std::string>());
		}

		TEST_METHOD(Accessors)
		{
			ActionIncrement emptyAction;
			emptyAction.SetName("EmptyAction");
			Assert::AreEqual("EmptyAction"s, emptyAction.Name());
			Assert::IsNull(emptyAction.GetEntity());

			Entity entity;
			Action* action = entity.CreateAction("ActionIncrement", "Increment");
			Assert::AreEqual("Increment"s, action->Name());
			Assert::AreEqual(entity, *action->GetEntity());

			Entity adopter;
			action->SetEntity(&adopter);
			Assert::IsNotNull(action->GetEntity());
			Assert::AreEqual(adopter, *action->GetEntity());
			Assert::IsNull(entity.FindScope(*action).first);

			const ActionIncrement copy = *action->As<ActionIncrement>();
			Assert::AreEqual(*action, *copy.As<Action>());

			action->SetEntity(nullptr);
			Assert::IsNull(action->GetEntity());
			delete action;
		}

		TEST_METHOD(Update)
		{
			Entity entity;
			int& integer1 = (entity.Append("Integer1") = 0).Get<int>();
			int& integer2 = (entity.Append("Integer2") = 0).Get<int>();

			Action* increment = entity.CreateAction("ActionIncrement"s, "Increment1"s);
			Assert::IsNotNull(increment);
			Assert::IsTrue(increment->Is(ActionIncrement::TypeIdClass()));

			ActionIncrement* castedIncrement = static_cast<ActionIncrement*>(increment);
			*castedIncrement->Find(castedIncrement->OperandKey) = "Integer1"s;

			increment = entity.CreateAction("ActionIncrement"s, "Increment2"s);
			Assert::IsNotNull(increment);
			Assert::IsTrue(increment->Is(ActionIncrement::TypeIdClass()));

			castedIncrement = static_cast<ActionIncrement*>(increment);
			*castedIncrement->Find(castedIncrement->OperandKey) = "Integer2"s;

			WorldState worldState;
			worldState.Entity = &entity;

			Assert::AreEqual(0, integer1);
			Assert::AreEqual(0, integer2);

			entity.Update(worldState);

			Assert::AreEqual(1, integer1);
			Assert::AreEqual(1, integer2);

			entity.Update(worldState);

			Assert::AreEqual(2, integer1);
			Assert::AreEqual(2, integer2);

			castedIncrement->Update(worldState);

			Assert::AreEqual(2, integer1);
			Assert::AreEqual(3, integer2);
		}

		TEST_METHOD(ToString)
		{
			ActionIncrement actionIncrement("Increment");
			Assert::AreEqual("Increment (Action)"s, actionIncrement.Action::ToString());
		}

	private:
		static _CrtMemState sStartMemState;

		ActionIncrementFactory actionIncrementFactory;
		EntityFactory entityFactory;
	};

	_CrtMemState ActionTest::sStartMemState;
}