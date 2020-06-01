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
	struct ActionTestReaction final : public Entity 
	{
		RTTI_DECLARATIONS(ActionTestReaction, Entity)

	public:
		explicit ActionTestReaction(const std::string& name=std::string()) : Entity(TypeIdClass(), name)
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

	ConcreteFactory(ActionTestReaction, Entity)


	TEST_CLASS(ReactionTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			TypeManager::Create();

			RegisterType<Entity>();
			RegisterType<World>();
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

			Assert::IsTrue(reactionA.Is(Entity::TypeIdClass()));
			Assert::IsTrue(reactionA.Equals(&reactionB));

			Entity* newAction = new ReactionAttributed();
			bool isReaction = newAction->Is(Reaction::TypeIdClass());

			Entity* createdReactionAttributed = isReaction ? createdReactionAttributed = newAction->CreateAs<Entity>() : nullptr;
			bool wasCreated = createdReactionAttributed != nullptr;

			bool  isReactionAttributed = wasCreated ? createdReactionAttributed->Is(ReactionAttributed::TypeIdClass()) : false;

			delete newAction;
			delete createdReactionAttributed;

			Assert::IsTrue(isReaction && wasCreated && isReactionAttributed);

			/* ActionEvent */

			ActionEvent actionEventA;
			ActionEvent actionEventB;

			Assert::IsTrue(actionEventA.Is(Entity::TypeIdClass()));
			Assert::IsTrue(actionEventA.Equals(&actionEventB));

			newAction = new ActionEvent();
			bool isAction = newAction->Is(Entity::TypeIdClass());

			Entity* createdActionEvent = isAction ? createdActionEvent = newAction->CreateAs<Entity>() : nullptr;
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

			auto actionSubtypeKey = reactionAttributed.Find(reactionAttributed.SubtypeKey);
			Assert::IsNotNull(actionSubtypeKey);
			Assert::AreEqual(Scope::Types::String, actionSubtypeKey->Type());
			Assert::AreEqual(std::string(), actionSubtypeKey->Get<std::string>());			

			/* ActionEvent */

			ActionEvent actionEvent("ActionEvent");
			Assert::AreEqual("ActionEvent"s, actionEvent.Name());

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

			Assert::AreEqual(*actionEvent.As<Entity>(), *ActionEvent(actionEvent).As<Entity>());
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
			Assert::AreEqual(*copy.As<Entity>(), *ActionEvent(std::move(actionEvent)).As<Entity>());
		}

		TEST_METHOD(Clone)
		{
			/* ReactionAttributed */

			ReactionAttributed reactionAttributed;
			Scope* clone = reactionAttributed.Clone();

			bool notNull = clone;
			bool isReactionAttributed = notNull ? clone->Is(ReactionAttributed::TypeIdClass()) : false;
			bool equal = *reactionAttributed.As<Entity>() == *clone->As<Entity>();

			delete clone;

			Assert::IsTrue(notNull && isReactionAttributed && equal);

			/* ActionEvent */

			ActionEvent actionEvent;
			clone = actionEvent.Clone();

			notNull = clone;
			bool isActionEvent = notNull ? clone->Is(ActionEvent::TypeIdClass()) : false;
			equal = *actionEvent.As<Entity>() == *clone->As<Entity>();

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
			Entity& entity = world.CreateChild("Entity", "Sector").CreateChild("Entity", "Entity");

			Entity& reactionAsAction = entity.CreateChild("ReactionAttributed", "Reaction");
			ReactionAttributed& reaction = *reactionAsAction.As<ReactionAttributed>();

			Entity& testReaction = reaction.CreateChild("ActionTestReaction"s, "TestReaction"s);

			Entity& create = entity.CreateChild("ActionEvent"s, "CreateEvent"s);
			Assert::IsTrue(create.Is(ActionEvent::TypeIdClass()));

			ActionEvent* event = create.As<ActionEvent>();
			auto parameter = event->AppendAuxiliaryAttribute("Parameter") = 1;

			Assert::AreEqual(0_z, world.GetWorldState().EventQueue->Size());

			world.Update();
			Assert::AreEqual(1_z, world.GetWorldState().EventQueue->Size());

			entity.Update(world.GetWorldState());
			Assert::AreEqual(2_z, world.GetWorldState().EventQueue->Size());
			Assert::AreEqual(0, testReaction.As<ActionTestReaction>()->Parameter);

			world.Update();
			Assert::AreEqual(1_z, world.GetWorldState().EventQueue->Size());
			Assert::AreEqual(2, testReaction.As<ActionTestReaction>()->Parameter);

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
		EntityFactory entityFactory;

		ActionTestReactionFactory actionTestReactionFactory;
	};

	_CrtMemState ReactionTest::sStartMemState;
}