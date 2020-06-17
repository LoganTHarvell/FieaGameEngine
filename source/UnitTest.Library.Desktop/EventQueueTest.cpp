#include "pch.h"

#include "ToStringSpecialization.h"
#include "Event.h"
#include "IEventSubscriber.h"
#include "EventQueue.h"

using namespace std::string_literals;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;

using namespace std::chrono_literals;

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

	class TestUpdateEnqueue final : public Foo, public IEventSubscriber
	{
	public:
		virtual void Notify(EventPublisher& eventPublisher) override
		{
			Data() = static_cast<Event<Foo>&>(eventPublisher).Message.Data();
			queue->Enqueue(std::make_shared<Event<Foo>>(Foo(20)));
		}

	public:
		EventQueue* queue{ nullptr };
	};

	class TestUpdateClear final : public Foo, public IEventSubscriber
	{
	public:
		virtual void Notify(EventPublisher& eventPublisher) override
		{
			Data() = static_cast<Event<Foo>&>(eventPublisher).Message.Data();
			queue->Clear();
		}

	public:
		EventQueue* queue{ nullptr };
	};

	class TestUpdateShrinkToFit final : public Foo, public IEventSubscriber
	{
	public:
		virtual void Notify(EventPublisher& eventPublisher) override
		{
			Data() = static_cast<Event<Foo>&>(eventPublisher).Message.Data();
			queue->Clear();
			queue->ShrinkToFit();
		}

	public:
		EventQueue* queue{ nullptr };
	};

	class TestUpdateException final : public IEventSubscriber
	{
	public:
		virtual void Notify(EventPublisher&) override
		{
			throw std::runtime_error("Test exception.");
		}

	public:
		EventQueue* queue{ nullptr };
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

			const auto fooEvent = std::make_shared<Event<Foo>>();
			queue.Enqueue(fooEvent);
			
			Assert::AreEqual(1_z, queue.Size());
			Assert::IsFalse(queue.IsEmpty());
			Assert::AreEqual(1_z, queue.Capacity());

			queue.Update(gameTime);
		}

		TEST_METHOD(Enqueue)
		{
			GameTime gameTime;
			gameTime.SetCurrentTime(std::chrono::high_resolution_clock::now());

			const auto fooEvent = std::make_shared<Event<Foo>>();
			queue.Enqueue(fooEvent);
			Assert::AreEqual(1_z, queue.Size());
		}

		TEST_METHOD(Update)
		{
			GameTime gameTime;

			const auto fooEvent1 = std::make_shared<Event<Foo>>(Foo(10));
			const auto fooEvent2 = std::make_shared<Event<Foo>>(Foo(20));
			queue.Enqueue(fooEvent1);
			queue.Enqueue(fooEvent2, gameTime.CurrentTime() + 10ms);

			Assert::ExpectException<std::runtime_error>([this]
			{
				queue.Enqueue(nullptr);
			});

			Vector<TestEventSubscriber> subscribers;
			subscribers.Resize(1000);

			for (auto& subscriber : subscribers)
			{
				Event<Foo>::Subscribe(subscriber);
			}

			queue.Update(gameTime);

			for (auto& subscriber : subscribers)
			{
				Assert::AreEqual(10, subscriber.Data());
			}

			Assert::AreEqual(1_z, queue.Size());

			gameTime.SetCurrentTime(std::chrono::steady_clock::time_point(5ms));
			queue.Update(gameTime);

			for (auto& subscriber : subscribers)
			{
				Assert::AreEqual(10, subscriber.Data());
			}

			Assert::AreEqual(1_z, queue.Size());

			gameTime.SetCurrentTime(std::chrono::steady_clock::time_point(10ms));
			queue.Update(gameTime);

			for (auto& subscriber : subscribers)
			{
				Assert::AreEqual(20, subscriber.Data());
			}
			Assert::AreEqual(0_z, queue.Size());
		}

		TEST_METHOD(ClearAndShrinkToFit)
		{
			const auto fooEvent1 = std::make_shared<Event<Foo>>();
			const auto fooEvent2 = std::make_shared<Event<Foo>>();
			queue.Enqueue(fooEvent1);
			queue.Enqueue(fooEvent2);
			
			queue.Clear();
			Assert::IsTrue(queue.IsEmpty());

			queue.ShrinkToFit();
			Assert::AreEqual(0_z, queue.Capacity());
		}

		TEST_METHOD(EnqueueInUpdate)
		{
			const GameTime gameTime;

			const auto fooEvent = std::make_shared<Event<Foo>>(Foo(10));
			queue.Enqueue(fooEvent);

			Vector<TestUpdateEnqueue> subscribers;
			subscribers.Resize(1000);

			for (auto& subscriber : subscribers)
			{
				subscriber.queue = &queue;
				Event<Foo>::Subscribe(subscriber);
			}

			queue.Update(gameTime);

			for (auto& subscriber : subscribers)
			{
				Assert::AreEqual(10, subscriber.Data());
			}
			
			Assert::AreEqual(1000_z, queue.Size());
		}

		TEST_METHOD(ClearInUpdate)
		{
			const GameTime gameTime;

			const auto fooEvent = std::make_shared<Event<Foo>>(Foo(10));
			queue.Enqueue(fooEvent);

			Vector<TestUpdateClear> subscribers;
			subscribers.Resize(1000);

			for (auto& subscriber : subscribers)
			{
				subscriber.queue = &queue;
				Event<Foo>::Subscribe(subscriber);
			}
			
			queue.Update(gameTime);

			for (auto& subscriber : subscribers)
			{
				Assert::AreEqual(10, subscriber.Data());
			}

			Assert::AreEqual(0_z, queue.Size());
		}

		TEST_METHOD(ShrinkToFitInUpdate)
		{
			const GameTime gameTime;

			const auto fooEvent = std::make_shared<Event<Foo>>(Foo(10));
			queue.Enqueue(fooEvent);

			Vector<TestUpdateShrinkToFit> subscribers;
			subscribers.Resize(1000);

			for (auto& subscriber : subscribers)
			{
				subscriber.queue = &queue;
				Event<Foo>::Subscribe(subscriber);
			}

			queue.Update(gameTime);

			for (auto& subscriber : subscribers)
			{
				Assert::AreEqual(10, subscriber.Data());
			}

			Assert::AreEqual(0_z, queue.Capacity());
		}

		TEST_METHOD(ExceptionInUpdate)
		{
			const GameTime gameTime;

			const auto fooEvent = std::make_shared<Event<Foo>>(Foo(10));
			queue.Enqueue(fooEvent);

			TestUpdateException subscriber;
			subscriber.queue = &queue;
			Event<Foo>::Subscribe(subscriber);

			Assert::ExpectException<std::runtime_error>([this, &gameTime] { queue.Update(gameTime); });
		}

	private:
		static _CrtMemState sStartMemState;

		EventQueue queue;
	};

	_CrtMemState EventQueueTest::sStartMemState;
}