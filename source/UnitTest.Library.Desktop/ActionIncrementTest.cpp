 #include "pch.h"

#include "ToStringSpecialization.h"
#include "ActionIncrement.h"
#include "Entity.h"

using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;


namespace EntitySystemTests::ActionTests
{
	TEST_CLASS(ActionIncrementTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			TypeManager::Create();
			
			RegisterType<Entity>();
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
			ActionIncrement incrementA;
			ActionIncrement incrementB;

			Assert::IsTrue(incrementA.Is(Entity::TypeIdClass()));
			Assert::IsTrue(incrementA.Equals(&incrementB));

			Entity* newIncrement = new ActionIncrement();
			bool isAction = newIncrement->Is(Entity::TypeIdClass());

			Entity* createdActionIncrement = isAction ? createdActionIncrement = newIncrement->CreateAs<Entity>() : nullptr;
			bool wasCreated = createdActionIncrement != nullptr;

			bool  isActionIncrement = wasCreated ? createdActionIncrement->Is(ActionIncrement::TypeIdClass()) : false;

			delete newIncrement;
			delete createdActionIncrement;

			Assert::IsTrue(isAction && wasCreated && isActionIncrement);
		}

		TEST_METHOD(Constructor)
		{
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

		TEST_METHOD(Clone)
		{
			ActionIncrement actionIncrement;
			Scope* clone = actionIncrement.Clone();

			bool notNull = clone;
			bool isActionIncrement = notNull ? clone->Is(ActionIncrement::TypeIdClass()) : false;
			bool equal = *actionIncrement.As<Entity>() == *clone->As<Entity>();

			delete clone;

			Assert::IsTrue(notNull && isActionIncrement && equal);
		}

		TEST_METHOD(Update)
		{
			Entity entity;
			int& integer1 = (entity.Append("Integer1") = 0).Get<int>();
			int& integer2 = (entity.Append("Integer2") = 0).Get<int>();

			Entity& increment = entity.CreateChild("ActionIncrement"s, "Increment1"s);
			Assert::IsTrue(increment.Is(ActionIncrement::TypeIdClass()));

			ActionIncrement& castedIncrement = static_cast<ActionIncrement&>(increment);
			*castedIncrement.Find(castedIncrement.OperandKey) = "Integer1"s;

			increment = entity.CreateChild("ActionIncrement"s, "Increment2"s);
			Assert::IsTrue(increment.Is(ActionIncrement::TypeIdClass()));

			castedIncrement = static_cast<ActionIncrement&>(increment);
			*castedIncrement.Find(castedIncrement.OperandKey) = "Integer2"s;

			WorldState worldState;
			worldState.Entity = &entity;

			Assert::AreEqual(0, integer1);
			Assert::AreEqual(0, integer2);

			entity.Update(worldState);

			Assert::AreEqual(1, integer1);
			Assert::AreEqual(1, integer2);

			*castedIncrement.Find(castedIncrement.IncrementStepKey) = -1;
			entity.Update(worldState);

			Assert::AreEqual(2, integer1);
			Assert::AreEqual(0, integer2);

			castedIncrement.Update(worldState);

			Assert::AreEqual(2, integer1);
			Assert::AreEqual(-1, integer2);
		}

		TEST_METHOD(ToString)
		{
			ActionIncrement actionIncrement("Increment");
			Assert::AreEqual("Increment (ActionIncrement)"s, actionIncrement.ToString());
		}

	private:
		static _CrtMemState sStartMemState;

		ActionIncrementFactory actionIncrementFactory;
		EntityFactory entityFactory;
	};

	_CrtMemState ActionIncrementTest::sStartMemState;
}