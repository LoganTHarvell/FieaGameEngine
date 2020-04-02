#pragma once

#pragma region Includes
// First Party
#include "Attributed.h"
#include "WorldState.h"
#include "Factory.h"
#pragma endregion Includes

namespace Library
{
	// Forwarded Classes
	class Entity;

	/// <summary>
	/// Represents a base action object within the reflection system.
	/// Serves to perform a function during update based on associated attributes.
	/// </summary>
	class Action : public Attributed
	{
		RTTI_DECLARATIONS_ABSTRACT(Action, Attributed)

#pragma region Type Definitions, Constants
	public:
		/// <summary>
		/// Key for the Name attribute in the Action.
		/// </summary>
		inline static const std::string NameKey = "Name";

	public:
		/// <summary>
		/// Getter for the class TypeInfo, used for registration with the TypeManager.
		/// </summary>
		static const TypeManager::TypeInfo& TypeInfo();
#pragma endregion Type Definitions, Constants

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Action() = delete;

		/// <summary>
		/// Default destructor.
		/// </summary>
		virtual ~Action() = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">Action to be copied.</param>
		Action(const Action& rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">Action to be copied.</param>
		/// <returns>Newly copied into left hand side Action.</returns>
		Action& operator=(const Action& rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">Action to be moved.</param>
		Action(Action&& rhs) noexcept = default;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">Action to be moved.</param>
		/// <returns>Newly moved into left hand side Action.</returns>
		Action& operator=(Action&& rhs) noexcept = default;

	protected:
		/// <summary>
		/// Specialized constructor for use by derived classes to ensure correct attribute population.
		/// </summary>
		/// <param name="typeId">Type ID of the derived class.</param>
		explicit Action(const RTTI::IdType typeId, const std::string& name=std::string());
#pragma endregion Special Members

#pragma region Accessors
	public:
		/// <summary>
		/// Name of the Action.
		/// </summary>
		/// <returns>Name of the Action as a std::string.</returns>
		const std::string& Name() const;

		/// <summary>
		/// Sets the name of the Action.
		/// </summary>
		/// <param name="name">String to use as the name of the Action.</param>
		void SetName(const std::string& name);

		/// <summary>
		/// Gets the Entity that owns this Action.
		/// </summary>
		/// <returns>Pointer to the Entity that owns this Action.</returns>
		Entity* GetEntity() const;

		/// <summary>
		/// Sets the Entity that owns this Action.
		/// </summary>
		/// <param name="sector">Entity to adopt this Action.</param>
		void SetEntity(Entity* sector);
#pragma endregion Accessors

#pragma region Game Loop
	public:
		/// <summary>
		/// Virtual update method called by the containing object.
		/// </summary>
		/// <param name="worldState">WorldState context for the current processing step.</param>
		/// <remarks>
		virtual void Update(WorldState& worldState)=0;
#pragma endregion Game Loop

#pragma region RTTI Overrides
	public:
		/// <summary>
		/// Virtual override for representing the Action as a std::string.
		/// </summary>
		/// <returns></returns>
		virtual std::string ToString() const override;
#pragma endregion RTTI Overrides

#pragma region Data Members
	private:
		/// <summary>
		/// Name of the Action, reflected as a prescribed attribute.
		/// </summary>
		std::string mName;
#pragma endregion Data Members
	};
}