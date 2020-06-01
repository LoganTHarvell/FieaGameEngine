#pragma once

#pragma region Includes
// First Party
#include "Entity.h"
#include "IEventSubscriber.h"
#pragma endregion Includes

namespace Library
{
	/// <summary>
	/// Abstract base class for an ActionList that supports Event subscription.
	/// </summary>
	class Reaction : public Entity, public IEventSubscriber
	{
		RTTI_DECLARATIONS_ABSTRACT(Reaction, Entity)

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Reaction() = delete;

		/// <summary>
		/// Default destructor.
		/// </summary>
		virtual ~Reaction() override = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">Reaction to be copied.</param>
		Reaction(const Reaction& rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">Reaction to be copied.</param>
		/// <returns>Newly copied into left hand side Reaction.</returns>
		Reaction& operator=(const Reaction& rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">Reaction to be moved.</param>
		Reaction(Reaction&& rhs) noexcept = default;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">Reaction to be moved.</param>
		/// <returns>Newly moved into left hand side Reaction.</returns>
		Reaction& operator=(Reaction&& rhs) noexcept = default;

	protected:
		/// <summary>
		/// Specialized constructor for use by derived classes to ensure correct Attribute population.
		/// </summary>
		/// <param name="typeId">Type ID of the derived class.</param>
		/// <param name="name">Name for the Reaction.</param>
		explicit Reaction(const IdType typeId, std::string name=std::string());
#pragma endregion Special Members

#pragma region Action List Overrides
	public:
		/// <summary>
		/// Update method overridden to have a default behavior of doing nothing.
		/// </summary>
		virtual void Update(WorldState&) override {};
#pragma endregion Action List Overrides

#pragma region RTTI Overrides
	public:
		/// <summary>
		/// Virtual override for representing the Entity as a std::string.
		/// </summary>
		/// <returns>Representation of an Event as a std::string.</returns>
		virtual std::string ToString() const override;
#pragma endregion RTTI Overrides
	};
}

#include "Reaction.inl"