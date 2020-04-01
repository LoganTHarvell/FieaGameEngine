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
	/// Represents a contained collection of Action objects.
	/// </summary>
	class ActionCreateAction final : public Action
	{
		RTTI_DECLARATIONS(ActionCreateAction, Action)

#pragma region Static Members
	public:
		/// <summary>
		/// Key for the name of the Target for the created Action attribute.
		/// </summary>
		inline static const std::string TargetKey = "Target";

		/// <summary>
		/// Key for the created Action attribute.
		/// </summary>
		inline static const std::string ActionKey = "Action";

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
		explicit ActionCreateAction(const std::string& name=std::string());

		/// <summary>
		/// Default destructor.
		/// </summary>
		~ActionCreateAction() = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">ActionCreateAction to be copied.</param>
		ActionCreateAction(const ActionCreateAction& rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">ActionCreateAction to be copied.</param>
		/// <returns>Newly copied into left hand side ActionCreateAction.</returns>
		ActionCreateAction& operator=(const ActionCreateAction& rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">ActionCreateAction to be moved.</param>
		ActionCreateAction(ActionCreateAction&& rhs) noexcept = default;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">ActionCreateAction to be moved.</param>
		/// <returns>Newly moved into left hand side ActionCreateAction.</returns>
		ActionCreateAction& operator=(ActionCreateAction&& rhs) noexcept = default;
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the ActionCreateAction.</returns>
		virtual gsl::owner<Library::Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor

#pragma region Game Loop
	public:
		/// <summary>
		/// Virtual update method to be called every frame.
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
		/// Name of an attribute where the created Action should be added.
		/// </summary>
		std::string mTarget;
#pragma endregion Data Members
	};

#pragma region Factory
	/// <summary>
	/// ActionCreateActionFactory class declaration.
	/// </summary>
	ConcreteFactory(ActionCreateAction, Scope)
#pragma endregion Factory
}

