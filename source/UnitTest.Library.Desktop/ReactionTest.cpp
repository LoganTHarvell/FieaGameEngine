#include "pch.h"

#include "ToStringSpecialization.h"

#include "ReactionAttributed.h"
#include "ActionEvent.h"
#include "ActionIncrement.h"
#include "Entity.h"
#include "Event.h"

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
			auto parameter = Search("Parameter");
			if (parameter) Parameter += parameter->Get<int>();
		}

	public:
		int Parameter{ 0 };
	};

	struct ActionTestParameterStack final : public Action 
	{
		RTTI_DECLARATIONS(ActionTestParameterStack, Action)

	public:
		explicit ActionTestParameterStack(const std::string& name=std::string()) : Action(TypeIdClass(), name)
		{
		}

		ActionTestParameterStack(const ActionTestParameterStack& rhs) = default;

		virtual gsl::owner<Library::Scope*> Clone() const override
		{
			return new ActionTestParameterStack(*this);
		}

		virtual void Update(Library::WorldState& worldState) override
		{
			if (StackLevel++ < 1)
			{
				EventMessageAttributed message;
				message.SetWorld(worldState.World);
				message.AppendAuxiliaryAttribute("Parameter") = 1;
				Event<EventMessageAttributed> event(message);
				worldState.EventQueue->Publish(event);
			}
		}

	public:
		int StackLevel{ 0 };
	};

	ConcreteFactory(ActionTestReaction, Scope)
	ConcreteFactory(ActionTestParameterStack, Scope)


	TEST_CLASS(ReactionTest)
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
			RegisterType<ActionTestParameterStack>();


#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			bool memCheckPass = true;
			Event<EventMessageAttributed>::UnsubscribeAll();
			Event<EventMessageAttributed>::SubscriberShrinkToFit();

#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				memCheckPass = false;
			}
#endif

			TypeManager::Destroy();
			Assert::IsTrue(memCheckPass, L"Memory Leaks!");
		}

		TEST_METHOD(RTTITest)
		{
			/* ReactionAttributed */

			ReactionAttributed reactionA;
			ReactionAttributed reactionB;

			Assert::IsTrue(reactionA.Is(Action::TypeIdClass()));
			Assert::IsTrue(reactionA.Equals(&reactionB));

			Action* newAction = new ReactionAttributed();
			bool isReaction = newAction->Is(Reaction::TypeIdClass());

			Action* createdReactionAttributed = isReaction ? createdReactionAttributed = newAction->CreateAs<Action>() : nullptr;
			bool wasCreated = createdReactionAttributed != nullptr;

			bool  isReactionAttributed = wasCreated ? createdReactionAttributed->Is(ReactionAttributed::TypeIdClass()) : false;

			delete newAction;
			delete createdReactionAttributed;

			Assert::IsTrue(isReaction && wasCreated && isReactionAttributed);

			/* ActionEvent */

			ActionEvent actionEventA;
			ActionEvent actionEventB;

			Assert::IsTrue(actionEventA.Is(Action::TypeIdClass()));
			Assert::IsTrue(actionEventA.Equals(&actionEventB));

			newAction = new ActionEvent();
			bool isAction = newAction->Is(Action::TypeIdClass());

			Action* createdActionEvent = isAction ? createdActionEvent = newAction->CreateAs<Action>() : nullptr;
			wasCreated = createdActionEvent != nullptr;

			bool  isActionEvent = wasCreated ? createdActionEvent->Is(ActionEvent::TypeIdClass()) : false;

			delete newAction;
			delete createdActionEvent;

			Assert::IsTrue(isAction && wasCreated && isActionEvent);
		}

		TEST_METHOD(Constructor)
		{
			/* ReactionAttributed */

			Assert::AreEqual(0_z, Event<EventMessageAttributed>::SubscriberCount());

			{
				const auto reaction = ReactionAttributed();
				Assert::AreEqual(1_z, Event<EventMessageAttributed>::SubscriberCount());
			}

			const auto event = std::make_shared<Event<EventMessageAttributed>>();
			EventQueue queue;
			queue.Enqueue(event);
			queue.Update(GameTime());

			Assert::AreEqual(0_z, Event<EventMessageAttributed>::SubscriberCount());

			ReactionAttributed reactionAttributed("ReactionAttributed");
			Assert::AreEqual("ReactionAttributed"s, reactionAttributed.Name());
			Assert::IsNotNull(reactionAttributed.Find("Name"));
			Assert::AreEqual("ReactionAttributed"s, reactionAttributed.Find("Name")->Get<std::string>());

			auto actionSubtypeKey = reactionAttributed.Find(reactionAttributed.SubtypeKey);
			Assert::IsNotNull(actionSubtypeKey);
			Assert::AreEqual(Scope::Types::String, actionSubtypeKey->Type());
			Assert::AreEqual(std::string(), actionSubtypeKey->Get<std::string>());			

			/* ActionEvent */

			ActionEvent actionEvent("ActionEvent");
			Assert::AreEqual("ActionEvent"s, actionEvent.Name());
			Assert::IsNotNull(actionEvent.Find("Name"));
			Assert::AreEqual("ActionEvent"s, actionEvent.Find("Name")->Get<std::string>());

			actionSubtypeKey = actionEvent.Find(actionEvent.SubtypeKey);
			Assert::IsNotNull(actionSubtypeKey);
			Assert::AreEqual(Scope::Types::String, actionSubtypeKey->Type());
			Assert::AreEqual(std::string(), actionSubtypeKey->Get<std::string>());

			auto delay = actionEvent.Find(actionEvent.DelayKey);
			Assert::IsNotNull(delay);
			Assert::AreEqual(Scope::Types::Integer, delay->Type());
			Assert::AreEqual(1_z, delay->Size());
			Assert::AreEqual(0, delay->Get<int>());
		}

		TEST_METHOD(Copy)
		{
			Assert::AreEqual(0_z, Event<EventMessageAttributed>::SubscriberCount());

			{
				const auto reaction = ReactionAttributed();
				auto copy = ReactionAttributed(reaction);
				Assert::AreEqual(2_z, Event<EventMessageAttributed>::SubscriberCount());
			}

			const auto event = std::make_shared<Event<EventMessageAttributed>>();
			EventQueue queue;
			queue.Enqueue(event);
			queue.Update(GameTime());

			Assert::AreEqual(0_z, Event<EventMessageAttributed>::SubscriberCount());

			ActionEvent actionEvent("ActionEvent");
			*actionEvent.Find(actionEvent.SubtypeKey) = "subtype";
			*actionEvent.Find(actionEvent.DelayKey) = 10;

			Assert::AreEqual(*actionEvent.As<Action>(), *ActionEvent(actionEvent).As<Action>());
		}

		TEST_METHOD(Move)
		{
			Assert::AreEqual(0_z, Event<EventMessageAttributed>::SubscriberCount());

			{
				auto reaction = ReactionAttributed();
				auto move = ReactionAttributed(std::move(reaction));
				Assert::AreEqual(2_z, Event<EventMessageAttributed>::SubscriberCount());
			}

			auto event = std::make_shared<Event<EventMessageAttributed>>();
			EventQueue queue;
			queue.Enqueue(event);
			queue.Update(GameTime());

			Assert::AreEqual(0_z, Event<EventMessageAttributed>::SubscriberCount());

			ActionEvent actionEvent("ActionEvent");
			*actionEvent.Find(actionEvent.SubtypeKey) = "subtype";
			*actionEvent.Find(actionEvent.DelayKey) = 10;

			auto copy = ActionEvent(actionEvent);
			Assert::AreEqual(*copy.As<Action>(), *ActionEvent(std::move(actionEvent)).As<Action>());
		}

		TEST_METHOD(Clone)
		{
			/* ReactionAttributed */

			ReactionAttributed reactionAttributed;
			Scope* clone = reactionAttributed.Clone();

			bool notNull = clone;
			bool isReactionAttributed = notNull ? clone->Is(ReactionAttributed::TypeIdClass()) : false;
			bool equal = *reactionAttributed.As<Action>() == *clone->As<Action>();

			delete clone;

			Assert::IsTrue(notNull && isReactionAttributed && equal);

			/* ActionEvent */

			ActionEvent actionEvent;
			clone = actionEvent.Clone();

			notNull = clone;
			bool isActionEvent = notNull ? clone->Is(ActionEvent::TypeIdClass()) : false;
			equal = *actionEvent.As<Action>() == *clone->As<Action>();

			delete clone;

			Assert::IsTrue(notNull && isActionEvent && equal);

			/* EventMessageAttributed */

			EventMessageAttributed eventMessageAttributed;
			clone = eventMessageAttributed.Clone();

			notNull = clone;
			bool isEventMessageAttributed = notNull ? clone->Is(EventMessageAttributed::TypeIdClass()) : false;
			equal = *eventMessageAttributed.As<Attributed>() == *clone->As<Attributed>();

			delete clone;

			Assert::IsTrue(notNull && isEventMessageAttributed && equal);
		}

		TEST_METHOD(Update)
		{
			const auto gameTime = std::make_shared<GameTime>();
			const auto eventQueue = std::make_shared<EventQueue>();

			World world("World", gameTime.get(), eventQueue.get());
			Entity* entity = world.CreateSector("Sector").CreateEntity("Entity", "Entity");

			Action* reactionAsAction = entity->CreateAction("ReactionAttributed", "Reaction");
			Assert::IsNotNull(reactionAsAction);
			ReactionAttributed& reaction = *reactionAsAction->As<ReactionAttributed>();

			Action* testReaction = reaction.CreateAction("ActionTestReaction"s, "TestReaction"s);

			Action* create = entity->CreateAction("ActionEvent"s, "CreateEvent"s);
			Assert::IsNotNull(create);
			Assert::IsTrue(create->Is(ActionEvent::TypeIdClass()));

			ActionEvent* event = create->As<ActionEvent>();
			auto parameter = event->AppendAuxiliaryAttribute("Parameter") = 1;

			Assert::AreEqual(0_z, world.GetWorldState().EventQueue->Size());

			world.Update();
			Assert::AreEqual(1_z, world.GetWorldState().EventQueue->Size());

			entity->Update(world.GetWorldState());
			Assert::AreEqual(2_z, world.GetWorldState().EventQueue->Size());
			Assert::AreEqual(0, testReaction->As<ActionTestReaction>()->Parameter);

			// TODO: Figure out why parameter stack fails here
			world.Update();
			Assert::AreEqual(1_z, world.GetWorldState().EventQueue->Size());
			Assert::AreEqual(2, testReaction->As<ActionTestReaction>()->Parameter);

			world.GetWorldState().EventQueue->Clear();
			world.GetWorldState().EventQueue->ShrinkToFit();
		}

		TEST_METHOD(ParameterStack)
		{
			const auto gameTime = std::make_shared<GameTime>();
			const auto eventQueue = std::make_shared<EventQueue>();

			World world("World", gameTime.get(), eventQueue.get());
			Entity* entity = world.CreateSector("Sector").CreateEntity("Entity", "Entity");

			ReactionAttributed& testReaction = *entity->CreateAction("ReactionAttributed", "TestReaction")->As<ReactionAttributed>();
			ActionTestParameterStack& testParameterStack = *testReaction.CreateAction("ActionTestParameterStack"s, "TestParameterStack"s)->As<ActionTestParameterStack>();
			Action* testAction = testReaction.CreateAction("ActionTestReaction"s, "TestReaction"s);
			
			Action* create = entity->CreateAction("ActionEvent"s, "CreateEvent"s);
			Assert::IsNotNull(create);
			Assert::IsTrue(create->Is(ActionEvent::TypeIdClass()));

			ActionEvent* event = create->As<ActionEvent>();
			auto parameter = event->AppendAuxiliaryAttribute("Parameter") = 1;

			Assert::AreEqual(0_z, world.GetWorldState().EventQueue->Size());

			world.Update();
			Assert::AreEqual(1_z, world.GetWorldState().EventQueue->Size());
			Assert::AreEqual(0, testParameterStack.StackLevel);
			Assert::AreEqual(0, testAction->As<ActionTestReaction>()->Parameter);
			
			world.Update();
			Assert::AreEqual(1_z, world.GetWorldState().EventQueue->Size());
			Assert::AreEqual(2, testAction->As<ActionTestReaction>()->Parameter);

			world.GetWorldState().EventQueue->Clear();
			world.GetWorldState().EventQueue->ShrinkToFit();
		}

		TEST_METHOD(ToString)
		{
			/* ActionEvent */
			const ReactionAttributed reactionAttributed("ReactionAttributed");
			Assert::AreEqual("ReactionAttributed (Reaction)"s, reactionAttributed.ToString());

			/* ActionEvent */
			const ActionEvent actionEvent("CreateEvent");
			Assert::AreEqual("CreateEvent (ActionEvent)"s, actionEvent.ToString());
		}

	private:
		static _CrtMemState sStartMemState;

		ReactionAttributedFactory reactionAttributedFactory;
		ActionEventFactory actionEventFactory;
		ActionIncrementFactory actionIncrementFactory;
		SectorFactory sectorFactory;
		EntityFactory entityFactory;

		ActionTestReactionFactory actionTestReactionFactory;
		ActionTestParameterStackFactory actionTestParameterFactory;
	};

	_CrtMemState ReactionTest::sStartMemState;
}