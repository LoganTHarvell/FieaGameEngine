#include "pch.h"

#include "ToStringSpecialization.h"
#include "ReactionAttributed.h"
#include "ActionEvent.h"
#include "ActionIncrement.h"
#include "Entity.h"

using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;


namespace EntitySystemTests::ActionTests
{
	struct ActionTestReaction final : public Action 
	{
		RTTI_DECLARATIONS(ActionTestReaction, Action)

	public:
		explicit ActionTestReaction(const std::string& name=std::string()) : Action(TypeIdClass(), name)
		{
		}

		ActionTestReaction(const ActionTestReaction& rhs) = default;

		virtual gsl::owner<Library::Scope*> Clone() const override
		{
			return new ActionTestReaction(*this);
		}

		virtual void Update(Library::WorldState&) override
		{
			auto intParameter = Search("IntegerParameter");
			if (intParameter) Data = *intParameter;
		}

	public:
		DataType Data{ 0 };
	};

	ConcreteFactory(ActionTestReaction, Scope)


	TEST_CLASS(ActionEventTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			TypeManager::Create();

			RegisterType<World>();
			RegisterType<Sector>();
			RegisterType<Entity>();

			RegisterType<Action>();
			RegisterType<ActionList>();
			RegisterType<ActionIncrement>();

			RegisterType<Reaction>();
			RegisterType<ReactionAttributed>();
			RegisterType<EventMessageAttributed>();
			RegisterType<ActionEvent>();

			RegisterType<ActionTestReaction>();


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
			ActionEvent createA;
			ActionEvent createB;

			Assert::IsTrue(createA.Is(Action::TypeIdClass()));
			Assert::IsTrue(createA.Equals(&createB));

			Action* newCreate = new ActionEvent();
			bool isAction = newCreate->Is(Action::TypeIdClass());

			Action* createdActionEvent = isAction ? createdActionEvent = newCreate->CreateAs<Action>() : nullptr;
			bool wasCreated = createdActionEvent != nullptr;

			bool  isActionEvent = wasCreated ? createdActionEvent->Is(ActionEvent::TypeIdClass()) : false;

			delete newCreate;
			delete createdActionEvent;

			Assert::IsTrue(isAction && wasCreated && isActionEvent);
		}

		TEST_METHOD(Constructor)
		{
			ActionEvent actionEvent("ActionEvent");
			Assert::AreEqual("ActionEvent"s, actionEvent.Name());
			Assert::IsNotNull(actionEvent.Find("Name"));
			Assert::AreEqual("ActionEvent"s, actionEvent.Find("Name")->Get<std::string>());

			auto actionSubtypeKey = actionEvent.Find(actionEvent.SubtypeKey);
			Assert::IsNotNull(actionSubtypeKey);
			Assert::AreEqual(Scope::Types::String, actionSubtypeKey->Type());
			Assert::AreEqual(std::string(), actionSubtypeKey->Get<std::string>());

			auto delay = actionEvent.Find(actionEvent.DelayKey);
			Assert::IsNotNull(delay);
			Assert::AreEqual(Scope::Types::Integer, delay->Type());
			Assert::AreEqual(1_z, delay->Size());
			Assert::AreEqual(0, delay->Get<int>());
		}

		TEST_METHOD(Clone)
		{
			ActionEvent actionEvent;
			Scope* clone = actionEvent.Clone();

			bool notNull = clone;
			bool isActionEvent = notNull ? clone->Is(ActionEvent::TypeIdClass()) : false;
			bool equal = *actionEvent.As<Action>() == *clone->As<Action>();

			delete clone;

			Assert::IsTrue(notNull && isActionEvent && equal);
		}

		TEST_METHOD(Update)
		{
			World world;
			Entity* entity = world.CreateSector("Sector").CreateEntity("Entity", "Entity");

			ReactionAttributed reaction;
			Action* testReaction = reaction.CreateAction("ActionTestReaction"s, "TestReaction"s);

			Action* create = entity->CreateAction("ActionEvent"s, "CreateEvent"s);
			Assert::IsNotNull(create);
			Assert::IsTrue(create->Is(ActionEvent::TypeIdClass()));

			ActionEvent* event = create->As<ActionEvent>();
			auto parameter = event->AppendAuxiliaryAttribute("IntegerParameter") = 1;

			Assert::AreEqual(0_z, world.GetEventQueue().Size());

			world.Update();
			Assert::AreEqual(1_z, world.GetEventQueue().Size());
			Assert::IsNull(reaction.Search("IntegerParameter"));

			world.Update();
			Assert::AreEqual(1_z, world.GetEventQueue().Size());
			Assert::AreEqual(parameter, testReaction->As<ActionTestReaction>()->Data);

			Event<EventMessageAttributed>::UnsubscribeAll();
			Event<EventMessageAttributed>::SubscriberShrinkToFit();

			world.GetEventQueue().Clear();
			world.GetEventQueue().ShrinkToFit();
		}

		TEST_METHOD(ToString)
		{
			ActionEvent actionEvent("CreateEvent");
			Assert::AreEqual("CreateEvent (ActionEvent)"s, actionEvent.ToString());
		}

	private:
		static _CrtMemState sStartMemState;

		ActionEventFactory actionEventFactory;
		ActionIncrementFactory actionIncrementFactory;
		SectorFactory sectorFactory;
		EntityFactory entityFactory;

		ActionTestReactionFactory actionTestReactionFactory;
	};

	_CrtMemState ActionEventTest::sStartMemState;
}