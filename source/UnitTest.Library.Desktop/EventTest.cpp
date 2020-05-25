#include "pch.h"

#include "ToStringSpecialization.h"
#include "Event.h"
#include "IEventSubscriber.h"
#include "EventQueue.h"

using namespace std::string_literals;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;

namespace EventTests
{
	class TestEventSubscriber final : public Foo, public IEventSubscriber
	{
	public:
		virtual void Notify(EventPublisher& eventPublisher) override
		{
			Data() = static_cast<Event<Foo>&>(eventPublisher).Message.Data();
		}
	};

	class TestSubscriberException final : public IEventSubscriber
	{
	public:
		virtual void Notify(EventPublisher&) override
		{
			throw std::runtime_error("Test exception.");
		}

	public:
		EventQueue* queue{ nullptr };
	};

	TEST_CLASS(EventTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			Event<Foo>::UnsubscribeAll();
			Event<Foo>::SubscriberShrinkToFit();

#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(Subscription)
		{
			TestEventSubscriber testEventSubscriber1;
			TestEventSubscriber testEventSubscriber2;
			TestEventSubscriber testEventSubscriber3;

			Assert::AreEqual(0_z, Event<Foo>::SubscriberCount());
			
			auto capacity = Event<Foo>::SubscriberCapacity();
			Assert::AreEqual(0_z, capacity);

			Event<Foo>::Subscribe(testEventSubscriber1);
			Assert::AreEqual(1_z, Event<Foo>::SubscriberCount());

			Assert::IsTrue(capacity < Event<Foo>::SubscriberCapacity());
			capacity = Event<Foo>::SubscriberCapacity();

			Assert::ExpectException<std::runtime_error>([&] 
			{
				Event<Foo>::Subscribe(testEventSubscriber1); 
			});

			Assert::AreEqual(capacity, Event<Foo>::SubscriberCapacity());
			
			Event<Foo> fooEvent;
			EventQueue::Publish(fooEvent);
			
			Assert::ExpectException<std::runtime_error>([&]
			{
				Event<Foo>::Subscribe(testEventSubscriber1);
			});

			Assert::AreEqual(capacity, Event<Foo>::SubscriberCapacity());

			Event<Foo>::Subscribe(testEventSubscriber2);
			Assert::AreEqual(2_z, Event<Foo>::SubscriberCount());
			Event<Foo>::Subscribe(testEventSubscriber3);
			Assert::AreEqual(3_z, Event<Foo>::SubscriberCount());

			EventQueue::Publish(fooEvent);
			capacity = Event<Foo>::SubscriberCapacity();

			Event<Foo>::Unsubscribe(testEventSubscriber1);
			Assert::AreEqual(2_z, Event<Foo>::SubscriberCount());
			Assert::AreEqual(capacity, Event<Foo>::SubscriberCapacity());

			Event<Foo>::UnsubscribeAll();
			Assert::AreEqual(0_z, Event<Foo>::SubscriberCount());
			Assert::AreEqual(capacity, Event<Foo>::SubscriberCapacity());

			Event<Foo>::SubscriberShrinkToFit();
			Assert::AreEqual(0_z, Event<Foo>::SubscriberCapacity());
		}

		TEST_METHOD(Constructor)
		{
			{
				const Event<Foo> fooEvent;
				Assert::AreEqual(Foo(), fooEvent.Message);
			}

			const Event<Foo> fooEvent(Foo(10));
			Assert::AreEqual(Foo(10), fooEvent.Message);

			const Event<Foo> constFooEvent = fooEvent;
			Assert::AreEqual(Foo(10), constFooEvent.Message);
		}

		TEST_METHOD(Publish)
		{
			Event<Foo> fooEvent(Foo(10));
			Vector<TestEventSubscriber> subscribers;
			subscribers.Resize(1000);

			for (auto& subscriber : subscribers)
			{
				Event<Foo>::Subscribe(subscriber);
				Assert::AreEqual(0, subscriber.Data());
			}
			
			EventQueue::Publish(fooEvent);

			for (auto& subscriber : subscribers)
			{
				Assert::AreEqual(fooEvent.Message.Data(), subscriber.Data());
			}
			
			TestSubscriberException subscriberWithException;
			Event<Foo>::Subscribe(subscriberWithException);

			Assert::ExpectException<std::runtime_error>([&fooEvent]{ EventQueue::Publish(fooEvent); });
		}

		TEST_METHOD(RTTITest)
		{
			const Event<Foo> a;
			Event<Foo> b;

			Assert::IsTrue(a.Is(EventPublisher::TypeIdClass()));

			EventPublisher* fooEvent = new Event<Foo>();
			const bool isEvent = fooEvent->Is(Event<Foo>::TypeIdClass());

			Event<Foo>* createdFooEvent = isEvent ? createdFooEvent = fooEvent->CreateAs<Event<Foo>>() : nullptr;
			const bool wasCreated = createdFooEvent != nullptr;

			const bool isFooEvent = wasCreated ? createdFooEvent->Is(Event<Foo>::TypeIdClass()) : false;

			delete fooEvent;
			delete createdFooEvent;

			Assert::IsTrue(isEvent);
			Assert::IsTrue(wasCreated);
			Assert::IsTrue(isFooEvent);
		}

		TEST_METHOD(ToString)
		{
			const Event<Foo> fooEvent;
			Assert::AreEqual("Event"s, fooEvent.ToString());
			Assert::AreEqual("EventPublisher"s, fooEvent.EventPublisher::ToString());
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState EventTest::sStartMemState;
}