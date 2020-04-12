#pragma once

#pragma region Includes
// Standard
#include <memory>

// First Party
#include "RTTI.h"
#include "Vector.h"
#pragma endregion Includes

namespace Library
{
	// Forwarded Classes
	class IEventSubscriber;

	/// <summary>
	/// Abstract base class wrapping shared Event functionality,
	/// notably delivery of an Event instance to IEventSubscriber subclasses.
	/// </summary>
	class EventPublisher : public RTTI 
	{
		RTTI_DECLARATIONS_ABSTRACT(EventPublisher, RTTI)

		friend class EventQueue;

#pragma region Type Definitions
	protected:
		/// <summary>
		/// Represents data associated with an entry in SubscriberList.
		/// </summary>
		struct SubscriberEntry final
		{
#pragma region Data Members
		public:
			/// <summary>
			/// Pointer to an IEventSubscriber instance.
			/// </summary>
			IEventSubscriber* Subscriber{ nullptr };

			/// <summary>
			/// Represents if the Subscriber is pending to be removed.
			/// </summary>
			bool PendingRemove{ false };
#pragma endregion Data Members

#pragma region Relational Operators
		public:
			/// <summary>
			/// Equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side EventEntry to be compared against.</param>
			/// <returns>
			/// True if both EventEntry instance point to the same Event. Otherwise false.
			/// Always returns false if either EventPublisher pointer is expired.
			/// </returns>
			bool operator==(const SubscriberEntry& rhs) const noexcept;

			/// <summary>
			/// Not equal operator.
			/// </summary>
			/// <param name="rhs">Right hand side EventEntry to be compared against.</param>
			/// <returns>
			/// True if both EventEntry instance point to the same Event. Otherwise false.
			/// Always returns false if either EventPublisher pointer is expired.
			/// </returns>
			bool operator!=(const SubscriberEntry& rhs) const noexcept;
#pragma endregion Relational Operators
		};

		/// <summary>
		/// List of subscribers to an Event subclass.
		/// </summary>
		using SubscriberList = Vector<SubscriberEntry>;
#pragma endregion Type Definitions

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		EventPublisher() = delete;

		/// <summary>
		/// Virtual default destructor.
		/// </summary>
		virtual ~EventPublisher() override = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		EventPublisher(const EventPublisher&) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		EventPublisher& operator=(const EventPublisher&) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		EventPublisher(EventPublisher&&) noexcept;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		EventPublisher& operator=(EventPublisher&&) noexcept;

	protected:
		/// <summary>
		/// Specialized constructor used to initialize a SubscriberList reference.
		/// Meant to be called from within the Event subclass constructor.
		/// </summary>
		/// <param name="subscribers">Reference to a list of subscribers.</param>
		explicit EventPublisher(SubscriberList& subscribers, SubscriberList& subscribersPendingAdd);
#pragma endregion Special Members

#pragma region Event Publishing
	private:
		/// <summary>
		/// Delivers the underlying Event instance.
		/// Calls Notify on each IEventSubscriber in the SubscriberList for this Event type.
		/// </summary>
		void Publish();
#pragma endregion Event Publishing

#pragma region RTTI Overrides
	public:
		/// <summary>
		/// Virtual override for representing the Entity as a std::string.
		/// </summary>
		/// <returns></returns>
		virtual std::string ToString() const override;
#pragma endregion RTTI Overrides

#pragma region Data Members
	private:
		/// <summary>
		/// A pointer to a static list of IEventSubscriber instances subscribed to the Event type
		/// of the underlying Event instance for the underlying Event instance.
		/// </summary>
		SubscriberList* mSubscriberList;

		/// <summary>
		/// A pointer to a static list of IEventSubscriber instances pending subscription.
		/// </summary>
		SubscriberList* mSubscribersPendingAdd;
#pragma endregion Data Members
	};
}

#include "EventPublisher.inl"