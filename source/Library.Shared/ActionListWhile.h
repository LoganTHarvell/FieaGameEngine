#pragma once

#pragma region Includes
// First Party
#include "Entity.h"
#include "WorldState.h"
#include "Factory.h"
#pragma endregion Includes

namespace Library
{
	/// <summary>
	/// Represents an Action for looping while a condition is true.
	/// </summary>
	class ActionListWhile final : public Entity
	{
		RTTI_DECLARATIONS(ActionListWhile, Entity)

#pragma region Type Definitions, Constants
	public:
		/// <summary>
		/// Key for the loop condition.
		/// </summary>
		inline static const std::string ConditionKey = "Condition";

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
		explicit ActionListWhile(std::string name=std::string());

		/// <summary>
		/// Default destructor.
		/// </summary>
		~ActionListWhile() = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">ActionListWhile to be copied.</param>
		ActionListWhile(const ActionListWhile& rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">ActionListWhile to be copied.</param>
		/// <returns>Newly copied into left hand side ActionListWhile.</returns>
		ActionListWhile& operator=(const ActionListWhile& rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">ActionListWhile to be moved.</param>
		ActionListWhile(ActionListWhile&& rhs) noexcept = default;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">ActionListWhile to be moved.</param>
		/// <returns>Newly moved into left hand side ActionListWhile.</returns>
		ActionListWhile& operator=(ActionListWhile&& rhs) noexcept = default;
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the ActionListWhile.</returns>
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
		/// Virtual override for representing the Action as a std::string.
		/// </summary>
		/// <returns></returns>
		virtual std::string ToString() const override;
#pragma endregion RTTI Overrides

#pragma region Data Members
	private:
		/// <summary>
		/// Condition determining whether the loop runs. Runs while condition is not zero.
		/// </summary>
		int mCondition{ 0 };
#pragma endregion Data Members
	};

#pragma region Factory
	/// <summary>
	/// ActionListWhileFactory class declaration.
	/// </summary>
	ConcreteFactory(ActionListWhile, Entity)
#pragma endregion Factory
}

