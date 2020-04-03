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
	/// Represents an Action for destroying Scope objects.
	/// </summary>
	class ActionDestroy final : public Action
	{
		RTTI_DECLARATIONS(ActionDestroy, Action)

#pragma region Type Definitions, Constants
	public:
		/// <summary>
		/// Key for the name of the attribute to destroy the Target.
		/// </summary>
		inline static const std::string AttributeKey = "Attribute";

		/// <summary>
		/// Key for the name of the Target to destroy.
		/// </summary>
		inline static const std::string TargetKey = "Target";

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
		explicit ActionDestroy(const std::string& name=std::string());

		/// <summary>
		/// Default destructor.
		/// </summary>
		~ActionDestroy() = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">ActionDestroy to be copied.</param>
		ActionDestroy(const ActionDestroy& rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">ActionDestroy to be copied.</param>
		/// <returns>Newly copied into left hand side ActionDestroy.</returns>
		ActionDestroy& operator=(const ActionDestroy& rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">ActionDestroy to be moved.</param>
		ActionDestroy(ActionDestroy&& rhs) noexcept = default;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">ActionDestroy to be moved.</param>
		/// <returns>Newly moved into left hand side ActionDestroy.</returns>
		ActionDestroy& operator=(ActionDestroy&& rhs) noexcept = default;
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the ActionDestroy.</returns>
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
		/// Name of the Action to be destroyed.
		/// </summary>
		std::string mAttributeName;

		/// <summary>
		/// Name for the attribute of the attribute to create.
		/// </summary>
		std::string mTargetName;
#pragma endregion Data Members
	};

#pragma region Factory
	/// <summary>
	/// ActionDestroyFactory class declaration.
	/// </summary>
	ConcreteFactory(ActionDestroy, Scope)
#pragma endregion Factory
}

