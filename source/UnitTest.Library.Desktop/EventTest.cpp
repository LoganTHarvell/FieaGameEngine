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
			Assert::AreEqual(0_z, Event<Foo>::SubscriberCapacity());

			Event<Foo>::Subscribe(testEventSubscriber1);
			Assert::AreEqual(1_z, Event<Foo>::SubscriberCount());
			Assert::AreEqual(0_z, Event<Foo>::SubscriberCapacity());

			Assert::ExpectException<std::runtime_error>([&] 
			{
				Event<Foo>::Subscribe(testEventSubscriber1); 
			});

			Event<Foo> fooEvent;
			EventQueue::Publish(fooEvent);
			
			Assert::AreNotEqual(0_z, Event<Foo>::SubscriberCapacity());

			Assert::ExpectException<std::runtime_error>([&]
			{
				Event<Foo>::Subscribe(testEventSubscriber1);
			});

			Event<Foo>::Subscribe(testEventSubscriber2);
			Assert::AreEqual(2_z, Event<Foo>::SubscriberCount());
			Event<Foo>::Subscribe(testEventSubscriber3);
			Assert::AreEqual(3_z, Event<Foo>::SubscriberCount());

			EventQueue::Publish(fooEvent);
			std::size_t capacity = Event<Foo>::SubscriberCapacity();

			Event<Foo>::Unsubscribe(testEventSubscriber1);
			Assert::AreEqual(3_z, Event<Foo>::SubscriberCount());
			Assert::AreEqual(capacity, Event<Foo>::SubscriberCapacity());

			EventQueue::Publish(fooEvent);
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
				Event<Foo> fooEvent;
				Assert::AreEqual(Foo(), fooEvent.Message);
			}

			Event<Foo> fooEvent(Foo(10));
			Assert::AreEqual(Foo(10), fooEvent.Message);

			const Event<Foo> constFooEvent = fooEvent;
			Assert::AreEqual(Foo(10), constFooEvent.Message);
		}

		TEST_METHOD(Publish)
		{
			Event<Foo> fooEvent(Foo(10));
			TestEventSubscriber subscriber;
			Event<Foo>::Subscribe(subscriber);

			Assert::AreEqual(0, subscriber.Data());
			EventQueue::Publish(fooEvent);
			Assert::AreEqual(fooEvent.Message.Data(), subscriber.Data());
		}

		TEST_METHOD(RTTITest)
		{
			Event<Foo> a;
			Event<Foo> b;

			Assert::IsTrue(a.Is(EventPublisher::TypeIdClass()));

			EventPublisher* fooEvent = new Event<Foo>();
			bool isEvent = fooEvent->Is(Event<Foo>::TypeIdClass());

			Event<Foo>* createdFooEvent = isEvent ? createdFooEvent = fooEvent->CreateAs<Event<Foo>>() : nullptr;
			bool wasCreated = createdFooEvent != nullptr;

			bool isFooEvent = wasCreated ? createdFooEvent->Is(Event<Foo>::TypeIdClass()) : false;

			delete fooEvent;
			delete createdFooEvent;

			Assert::IsTrue(isEvent);
			Assert::IsTrue(wasCreated);
			Assert::IsTrue(isFooEvent);
		}

		TEST_METHOD(ToString)
		{
			Event<Foo> fooEvent;
			Assert::AreEqual("Event"s, fooEvent.ToString());
			Assert::AreEqual("EventPublisher"s, fooEvent.EventPublisher::ToString());
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState EventTest::sStartMemState;
}