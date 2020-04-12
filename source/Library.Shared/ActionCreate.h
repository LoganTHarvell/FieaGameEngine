#pragma once

#pragma region Includes
// First Party
#include "Action.h"
#include "WorldState.h"
#include "Factory.h"
#pragma endregion Includes

namespace Library
{
	/// <summary>
	/// Represents an Action for creating Scopes.
	/// </summary>
	class ActionCreate final : public Action
	{
		RTTI_DECLARATIONS(ActionCreate, Action)

#pragma region Type Definitions, Constants
	public:
		/// <summary>
		/// Key for the name of the Attribute to append the created Attribute onto.
		/// </summary>
		inline static const std::string AttributeKey = "Attribute";

		/// <summary>
		/// Key for the created Attribute.
		/// </summary>
		inline static const std::string NewScopeKey = "Scope";

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
		explicit ActionCreate(const std::string& name=std::string());

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
		virtual gsl::owner<Library::Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor

#pragma region Game Loop
	public:
		/// <summary>
		/// Virtual update method called by the containing object.
		/// </summary>
		/// <param name="worldState">WorldState context for the current processing step.</param>
		virtual void Update(WorldState & worldState);
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
		/// KeyType valuefor the Attribute of the Attribute to create.
		/// </summary>
		std::string mAttributeKey;
#pragma endregion Data Members
	};

#pragma region Factory
	/// <summary>
	/// ActionCreateFactory class declaration.
	/// </summary>
	ConcreteFactory(ActionCreate, Scope)
#pragma endregion Factory
}

