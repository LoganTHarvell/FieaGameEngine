#pragma once

#pragma region Includes
// First Party
#include "EventPublisher.h"
#pragma endregion Includes

namespace Library
{
	/// <summary>
	/// Represents an Event type that can be subscribed to by IEventSubscriber subclasses
	/// such that the subscriber will be notified if an Event instance is published.
	/// </summary>
	/// <typeparam name="MessageT">Data type contained by the Event as a message for subscribers.</typeparam>
	template<typename MessageT>
	class Event final : public EventPublisher
	{
		RTTI_DECLARATIONS(Event, EventPublisher)

#pragma region Static Members
	public:
		/// <summary>
		/// Subscribes an IEvenSubscriber instance to an Event type.
		/// </summary>
		/// <param name="eventSubscriber">IEventSubscriber instance to add to the SubscriberList.</param>
		static void Subscribe(IEventSubscriber& eventSubscriber);

		/// <summary>
		/// Unsubscribes an IEvenSubscriber instance to this Event type.
		/// </summary>
		/// <param name="eventSubscriber">IEventSubscriber instance to remove from the SubscriberList.</param>
		static void Unsubscribe(IEventSubscriber& eventSubscriber);

		/// <summary>
		/// Unsubscribes all IEvenSubscriber instances in the SubscriberList of an Event type.
		/// </summary>
		static void UnsubscribeAll();

	private:
		/// <summary>
		/// Static list of IEventSubsriber instances associated with an Event type.
		/// </summary>
		static SubscriberList sSubscribers;
#pragma endregion Static Members

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="message">Data contained by this Event instance.</param>
		explicit Event(MessageT message=MessageT());

		/// <summary>
		/// Default destructor.
		/// </summary>
		~Event() = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">Event instance to be copied.</param>
		Event(const Event& rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">Event instance to be copied.</param>
		/// <returns>Newly copied into Event instance.</returns>
		Event& operator=(const Event& rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">Event instance to be moved.</param>
		/// <returns>Newly moved into Event instance.</returns>
		Event(Event&& rhs) noexcept= default;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">Event instance to be moved.</param>
		/// <returns>Newly moved into Event instance.</returns>
		Event& operator=(Event&& rhs) noexcept = default;
#pragma endregion Special Members

#pragma region Accessors
		MessageT& Message();
		const MessageT& Message() const;
#pragma endregion Accessors

#pragma region Data Members
	private:
		/// <summary>
		/// Data contained by the Event.
		/// </summary>
		MessageT mMessage;
#pragma endregion Data Members
	};
}

#include "Event.inl"