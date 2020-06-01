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
	/// Represents an Action for creating Scopes.
	/// </summary>
	class ActionCreate final : public Entity
	{
		RTTI_DECLARATIONS(ActionCreate, Entity)

#pragma region Type Definitions, Constants
	public:
		/// <summary>
		/// Key for the created Attribute.
		/// </summary>
		inline static const std::string EntityPrototypeKey = "Entity";

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
		explicit ActionCreate(std::string name=std::string());

		/// <summary>
		/// Default destructor.
		/// </summary>
		~ActionCreate() = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">ActionCreate to be copied.</param>
		ActionCreate(const ActionCreate& rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">ActionCreate to be copied.</param>
		/// <returns>Newly copied into left hand side ActionCreate.</returns>
		ActionCreate& operator=(const ActionCreate& rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">ActionCreate to be moved.</param>
		ActionCreate(ActionCreate&& rhs) noexcept = default;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">ActionCreate to be moved.</param>
		/// <returns>Newly moved into left hand side ActionCreate.</returns>
		ActionCreate& operator=(ActionCreate&& rhs) noexcept = default;
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the ActionCreate.</returns>
		virtual gsl::owner<Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor

#pragma region Game Loop
	public:
		/// <summary>
		/// Virtual update method called by the containing object.
		/// </summary>
		virtual void Update(WorldState&) override;
#pragma endregion Game Loop

#pragma region RTTI Overrides
	public:
		/// <summary>
		/// Virtual override for representing the Action as a std::string.
		/// </summary>
		/// <returns></returns>
		virtual std::string ToString() const override;
#pragma endregion RTTI Overrides
	};

#pragma region Factory
	/// <summary>
	/// ActionCreateFactory class declaration.
	/// </summary>
	ConcreteFactory(ActionCreate, Entity)
#pragma endregion Factory
}

