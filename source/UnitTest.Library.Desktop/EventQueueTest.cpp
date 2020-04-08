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
			Data() = int(static_cast<Event<Foo>&>(eventPublisher).Message().Data());
		}
	};


	TEST_CLASS(EventQueueTest)
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
			queue.Clear();
			queue.ShrinkToFit();

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

		TEST_METHOD(Accessors)
		{
			Assert::AreEqual(0_z, queue.Size());
			Assert::IsTrue(queue.IsEmpty());
			Assert::AreEqual(0_z, queue.Capacity());

			GameClock clock;
			GameTime gameTime;

			clock.UpdateGameTime(gameTime);

			auto fooEvent = std::make_shared<Event<Foo>>();
			queue.Enqueue(fooEvent, gameTime);
			
			Assert::AreEqual(1_z, queue.Size());
			Assert::IsFalse(queue.IsEmpty());
			Assert::AreNotEqual(0_z, queue.Capacity());
		}

		TEST_METHOD(Enqueue)
		{
			GameTime gameTime;
			gameTime.SetCurrentTime(std::chrono::high_resolution_clock::now());

			auto fooEvent = std::make_shared<Event<Foo>>();
			queue.Enqueue(fooEvent, gameTime);
			Assert::AreEqual(1_z, queue.Size());

			Assert::ExpectException<std::runtime_error>([&]
			{
				queue.Enqueue(fooEvent, gameTime);
			});
		}

		TEST_METHOD(Update)
		{
			GameClock clock;
			GameTime gameTime;

			clock.UpdateGameTime(gameTime);

			auto fooEvent1 = std::make_shared<Event<Foo>>(Foo(10));
			auto fooEvent2 = std::make_shared<Event<Foo>>(Foo(20));
			queue.Enqueue(fooEvent1, gameTime);
			queue.Enqueue(fooEvent2, gameTime, std::chrono::milliseconds(60 * 1000));
			queue.Enqueue(std::make_shared<Event<Foo>>(Foo(30)), gameTime);

			TestEventSubscriber subscriber;
			Event<Foo>::Subscribe(subscriber);

			queue.Update(gameTime);
			Assert::AreEqual(10, subscriber.Data());
			Assert::AreEqual(1_z, queue.Size());
		}

		TEST_METHOD(ClearAndShrinkToFit)
		{
			GameTime gameTime;
			gameTime.SetCurrentTime(std::chrono::high_resolution_clock::now());

			auto fooEvent1 = std::make_shared<Event<Foo>>();
			auto fooEvent2 = std::make_shared<Event<Foo>>();
			queue.Enqueue(fooEvent1, gameTime);
			queue.Enqueue(fooEvent2, gameTime);
			
			queue.Clear();
			Assert::IsTrue(queue.IsEmpty());

			queue.ShrinkToFit();
			Assert::AreEqual(0_z, queue.Capacity());
		}

	private:
		static _CrtMemState sStartMemState;

		EventQueue queue;
	};

	_CrtMemState EventQueueTest::sStartMemState;
}