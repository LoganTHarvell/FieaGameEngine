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
			const std::weak_ptr<EventPublisher> EventPublisher;
			const TimePoint ExpireTime;
			bool IsExpired;
		};
#pragma endregion Type Definitions, Constants

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
#pragma endregion Accessors

#pragma region Modifiers
	public:
		/// <summary>
		/// Adds an EventEntry to the EventQueue.
		/// </summary>
		/// <param name="eventPublisher">EventPublisher reference to the Event instance to be queued.</param>
		/// <param name="gameTime">Reference to a GameTime instance used to calculate the Event expire time.</param>
		/// <param name="delay">Duration value in milliseconds to delay publishing the Event. Defaults to zero.</param>
		void Enqueue(const std::weak_ptr<EventPublisher> eventPublisher, const GameTime& gameTime, Duration delay=Duration(0));

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
#pragma endregion Modifiers

#pragma region Data Members
	private:
		/// <summary>
		/// Queue of EventEntry data used to publish Event instances to their EventSubscriber lists.
		/// </summary>
		Vector<EventEntry> mQueue;
#pragma endregion Data Members
	};
}

#include "EventQueue.inl"