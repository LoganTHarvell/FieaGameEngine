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
		/// Getter method for the SubscriberList size for the Event type.
		/// </summary>
		/// <returns>Number of subscribers to the Event type.</returns>
		static std::size_t SubscriberCount();

		/// <summary>
		/// Getter method for the SubscriberList capacity for the Event type.
		/// </summary>
		/// <returns>Number of subscribers for which the Subscriber list has memory allocated.</returns>
		/// <remarks>
		/// Note this is not equivalent to a max size, but only the number of 
		/// subscribers that the SubscriberList may hold before allocating new memory.
		/// </remarks>
		static std::size_t SubscriberCapacity();

		/// <summary>
		/// Subscribes an IEventSubscriber instance to an Event type.
		/// </summary>
		/// <param name="eventSubscriber">IEventSubscriber instance to add to the SubscriberList.</param>
		static void Subscribe(IEventSubscriber& eventSubscriber);

		/// <summary>
		/// Unsubscribes an IEventSubscriber instance to this Event type.
		/// </summary>
		/// <param name="eventSubscriber">IEventSubscriber instance to remove from the SubscriberList.</param>
		/// <remarks>Does not reduce the Subscriber count until an Event of this type is published.</remarks>
		static void Unsubscribe(IEventSubscriber& eventSubscriber);

		/// <summary>
		/// Unsubscribes all IEventSubscriber instances in the SubscriberList of an Event type.
		/// </summary>
		/// <remarks>This should never be called during Publish.</remarks>
		static void UnsubscribeAll();

		/// <summary>
		/// Resizes the capacity of the SubscriberList to the size.
		/// </summary>
		/// <remarks>This should never be called during Publish.</remarks>
		static void SubscriberShrinkToFit();

	private:
		/// <summary>
		/// Static list of IEventSubsriber instances associated with an Event type.
		/// </summary>
		inline static SubscriberList sSubscriberList;

		/// <summary>
		/// Static list of IEventSubsriber instances pending addition to the SubscriberList.
		/// </summary>
		inline static SubscriberList sSubscribersPendingAdd;
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
		/// <summary>
		/// Gets a reference to the data contained by the Event instance.
		/// </summary>
		/// <returns>Reference to the MessageT data contained by the Event instance.</returns>
		MessageT& Message();

		/// <summary>
		/// Gets a reference to the data contained by the Event instance.
		/// </summary>
		/// <returns>Reference to the MessageT data contained by the Event instance.</returns>
		const MessageT& Message() const;
#pragma endregion Accessors

#pragma region RTTI Overrides
	public:
		/// <summary>
		/// Virtual override for representing the Entity as a std::string.
		/// </summary>
		/// <returns></returns>
		virtual std::string ToString() const override;

		/// <summary>
		/// RTTI override that determines if the Scope is equal to a given RTTI derived class instance.
		/// </summary>
		/// <param name="rhs">RTTI derived class instance to be compared against.</param>
		/// <returns>True if the Scope is equivalent to the given RTTI derived class instance.</returns>
		virtual bool Equals(const RTTI* rhs) const override;
#pragma endregion RTTI Overrides

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