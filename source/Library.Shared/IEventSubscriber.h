#pragma once

// Standard
#include <memory>

namespace Library
{
	// Forwarded Classes
	class EventPublisher;

	/// <summary>
	/// Interface for classes that intend to subscribe to an Event.
	/// </summary>
	class IEventSubscriber
	{
		friend EventPublisher;

#pragma region Special Member Functions
	public:
		/// <summary>
		/// Virtual default destructor.
		/// </summary>
		virtual ~IEventSubscriber() = default;

	protected:
		/// <summary>
		/// Default constructor.
		/// </summary>
		IEventSubscriber() = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		IEventSubscriber(const IEventSubscriber&) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		IEventSubscriber& operator=(const IEventSubscriber&) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		IEventSubscriber(IEventSubscriber&&) noexcept = default;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		IEventSubscriber& operator=(IEventSubscriber&&) noexcept = default;
#pragma endregion Special Member Functions

#pragma region Event Notification
	protected:
		/// <summary>
		/// Pure virtual method called to receive a published Event.
		/// Implement to customize the behavior that occurs when the Event is received.
		/// </summary>
		/// <param name="eventPublisher">Reference to an Event as an EventPublisher.</param>
		/// <remarks>Overrides must be thread safe.</remarks>
		virtual void Notify(EventPublisher& eventPublisher)=0;
#pragma endregion Event Notification
	};
}