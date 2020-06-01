#pragma once

#pragma region Includes
// First Party
#include "Entity.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "WorldState.h"
#include "Factory.h"
#pragma endregion Includes

namespace Library
{
	/// <summary>
	/// Represents an Action for creating Scopes.
	/// </summary>
	class ActionEvent final : public Entity
	{
		RTTI_DECLARATIONS(ActionEvent, Entity)

#pragma region Static Members
	public:
		/// <summary>
		/// Key for the Subtype Attribute used to set the created EventMessageAttributed Subtype.
		/// </summary>
		inline static const std::string SubtypeKey = "Subtype";

		/// <summary>
		/// Key for the Delay Attribute used to specify how long in milliseconds to delay the created Event.
		/// </summary>
		inline static const std::string DelayKey = "Delay";

	public:
		/// <summary>
		/// Getter for the class TypeInfo, used for registration with the TypeManager.
		/// </summary>
		static const TypeManager::TypeInfo& TypeInfo();
#pragma endregion Static Members

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		explicit ActionEvent(std::string name=std::string(), std::string subtype=std::string(), const int delay=0);

		/// <summary>
		/// Default destructor.
		/// </summary>
		~ActionEvent() = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">ActionEvent to be copied.</param>
		ActionEvent(const ActionEvent& rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">ActionEvent to be copied.</param>
		/// <returns>Newly copied into left hand side ActionEvent.</returns>
		ActionEvent& operator=(const ActionEvent& rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">ActionEvent to be moved.</param>
		ActionEvent(ActionEvent&& rhs) noexcept = default;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">ActionEvent to be moved.</param>
		/// <returns>Newly moved into left hand side ActionEvent.</returns>
		ActionEvent& operator=(ActionEvent&& rhs) noexcept = default;
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the ActionEvent.</returns>
		virtual gsl::owner<Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor

#pragma region Game Loop
	public:
		/// <summary>
		/// Virtual update method called by the containing object.
		/// </summary>
		/// <param name="worldState">WorldState context for the current processing step.</param>
		virtual void Update(WorldState& worldState) override;
#pragma endregion Game Loop

#pragma region RTTI Overrides
	public:
		/// <summary>
		/// Virtual override for representing the ActionEvent as a std::string.
		/// </summary>
		/// <returns></returns>
		virtual std::string ToString() const override;
#pragma endregion RTTI Overrides

#pragma region Data Members
	private:
		/// <summary>
		/// Key value for the Attribute of the Attribute to create.
		/// </summary>
		std::string mSubtype;

		/// <summary>
		/// Delay for the created Event.
		/// </summary>
		int mDelay;
#pragma endregion Data Members
	};

#pragma region Factory
	/// <summary>
	/// ActionEventFactory class declaration.
	/// </summary>
	ConcreteFactory(ActionEvent, Entity)
#pragma endregion Factory
}

