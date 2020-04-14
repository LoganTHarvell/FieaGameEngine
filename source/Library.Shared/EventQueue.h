#pragma once

#pragma region Includes
// Standard
#include <chrono>
#include <memory>

// First Party
#include "Vector.h"
#include "GameTime.h"
#pragma endregion Includes

namespace Library
{
	// Forwarded Classes
	class EventPublisher;

	/// <summary>
	/// Queued list of Event instances that need to be published to their EventSubscriber list.
	/// </summary>
	class EventQueue final 
	{
#pragma region Type Definitions, Constants
	private:
		/// <summary>
		/// Type definition for a point in time.
		/// </summary>
		using TimePoint = std::chrono::high_resolution_clock::time_point;

		/// <summary>
		/// Type definition for a duration of time.
		/// </summary>
		using Duration = std::chrono::milliseconds;
		
		/// <summary>
		/// Data needed to publish an Event.
		/// </summary>
		struct EventEntry final
		{
#pragma region Special Members
		public:
			/// <summary>
			/// Default constructor.
			/// </summary>
			/// <param name="publisher">Weak pointer to an Event as a EventPublisher.</param>
			/// <param name="expireTime">TimePoint when the Event should expire.</param>
			/// <param name="isExpired">Boolean denoting the EventEntry is expired.</param>
			explicit EventEntry(const std::shared_ptr<EventPublisher> publisher=std::shared_ptr<EventPublisher>(), const TimePoint& expireTime=TimePoint());

			/// <summary>
			/// Default destructor.
			/// </summary>
			~EventEntry() = default;

			/// <summary>
			/// Copy constructor.
			/// </summary>
			/// <param name="rhs">EventEntry to be copied.</param>
			EventEntry(const EventEntry& rhs) = default;

			/// <summary>
			/// Copy assignment operator.
			/// </summary>
			/// <param name="rhs">EventEntry to be copied.</param>
			/// <returns>Newly copied into EventEntry.</returns>
			EventEntry& operator=(const EventEntry& rhs) = default;

			/// <summary>
			/// Move constructor.
			/// </summary>
			/// <param name="rhs">Event Entry to be moved.</param>
			EventEntry(EventEntry&& rhs) noexcept = default;

			/// <summary>
			/// Move assignment operator.
			/// </summary>
			/// <param name="rhs">EventEntry to be moved.</param>
			/// <returns>Newly moved into EventEntry.</returns>
			EventEntry& operator=(EventEntry&& rhs) noexcept = default;
#pragma endregion Special Members

#pragma region Data Members
		public:
			/// <summary>
			/// Weak pointer to an Event as EventPublisher.
			/// </summary>
			std::shared_ptr<EventPublisher> Publisher;

			/// <summary>
			/// Time point at which the Event should be published.
			/// </summary>
			TimePoint ExpireTime;
#pragma endregion Data Members

		};
#pragma endregion Type Definitions, Constants

#pragma region Static Members
		public:
			/// <summary>
			/// Wrapper for EventPublisher Publish.
			/// </summary>
			/// <param name="event">Reference to Event to publish as an EventPublisher.</param>
			static void Publish(EventPublisher& event);
#pragma endregion Static Members

#pragma region Accessors
	public:
		/// <summary>
		/// Getter method for the number of elements in the EventQueue.
		/// </summary>
		/// <returns>Number of queued Event instances.</returns>
		std::size_t Size() const;

		/// <summary>
		/// Checks the size of the EventQueue, returns true if greater than zero, indicating it is non-empty.
		/// </summary>
		/// <returns>True if the EventQueue contains no Event instances, otherwise false.</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Gets the max number of subscribers for which memory is already allocated.
		/// </summary>
		/// <returns>Max number of subscribers for which memory is already allocated.</returns>
		/// <remarks>This value may be inaccurate between calls to Enqueue and Update.</remarks>
		std::size_t Capacity() const;
#pragma endregion Accessors

#pragma region Modifiers
	public:
		/// <summary>
		/// Adds an EventEntry to the EventQueue.
		/// </summary>
		/// <param name="eventPublisher">EventPublisher reference to the Event instance to be queued.</param>
		/// <param name="gameTime">Reference to a GameTime instance used to calculate the Event expire time.</param>
		/// <param name="delay">Duration value in milliseconds to delay publishing the Event. Defaults to zero.</param>
		/// <exception cref="std::runtime_error">Attempted to Enqueue null pointer.</exception>
		/// <remarks>EventQueue capacity will not increase until the next call to Update.</remarks>
		void Enqueue(const std::shared_ptr<EventPublisher> eventPublisher, const TimePoint& expireTime=TimePoint());

		/// <summary>
		/// Publishes all expired events, then dequeues expired EventEntry instances.
		/// Also dequeues any EventEntry instances with null EventPublisher references.
		/// </summary>
		/// <param name="gameTime">Reference to a GameTime instance used when calculating EventEntry expiration.</param>
		void Update(const GameTime& gameTime);

		/// <summary>
		/// Removes all EventEntry instances from the EventQueue, resetting the size to zero.
		/// </summary>
		void Clear();

		/// <summary>
		/// Resizes the capacity of the EventQueue to the size.
		/// </summary>
		void ShrinkToFit();
#pragma endregion Modifiers

#pragma region Data Members
	private:
		/// <summary>
		/// Queue of EventEntry data used to publish Event instances to their EventSubscriber lists.
		/// </summary>
		Vector<EventEntry> mQueue{ Vector(Vector<EventEntry>::EqualityFunctor()) };

		/// <summary>
		/// Queue of EventEntry data pending to be Enqueued during the next Update call.
		/// </summary>
		Vector<EventEntry> mPendingQueue{ Vector(Vector<EventEntry>::EqualityFunctor()) };

		/// <summary>
		/// Represents when a call to Update is currently in progress.
		/// </summary>
		bool mUpdating{ false };

		/// <summary>
		/// Represents when a call to Clear needs to be deferred while Update is in progress.
		/// </summary>
		bool mPendingClear{ false };

		/// <summary>
		/// Represents when a call to ShrinkToFit needs to be deferred while Update is in progress.
		/// </summary>
		bool mPendingShrinkToFit{ false };
#pragma endregion Data Members
	};
}

#include "EventQueue.inl"