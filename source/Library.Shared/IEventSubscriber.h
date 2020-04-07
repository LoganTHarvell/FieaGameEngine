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
#pragma region Special Member Functions
	protected:
		/// <summary>
		/// Default constructor.
		/// </summary>
		IEventSubscriber() = default;

	public:
		/// <summary>
		/// Virtual default destructor.
		/// </summary>
		virtual ~IEventSubscriber() = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		IEventSubscriber(const IEventSubscriber&) = delete;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		IEventSubscriber& operator=(const IEventSubscriber&) = delete;

		/// <summary>
		/// Move constructor.
		/// </summary>
		IEventSubscriber(IEventSubscriber&&) noexcept = delete;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		IEventSubscriber& operator=(IEventSubscriber&&) noexcept = delete;
#pragma endregion Special Member Functions

#pragma region Pure Virtual Methods
	public:
		/// <summary>
		/// Pure virtual method called to receive a published Event.
		/// Implement to customize the behavior that occurs when the Event is received.
		/// </summary>
		/// <param name="eventPublisher">Reference to an Event as an EventPublisher.</param>
		virtual void Notify(EventPublisher& eventPublisher)=0;
#pragma endregion Pure Virtual Methods
	};
}