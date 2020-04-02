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
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action)

#pragma region Static Members
	public:
		/// <summary>
		/// Key for the Actions attribute in the ActionList.
		/// </summary>
		inline static const std::string ActionsKey = "Actions";

		/// <summary>
		/// Index of the Actions attribute in the ActionList.
		/// </summary>
		inline static const std::size_t ActionsIndex = 2;

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
		explicit ActionList(const std::string& name=std::string());

		/// <summary>
		/// Default destructor.
		/// </summary>
		~ActionList() = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">ActionList to be copied.</param>
		ActionList(const ActionList& rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">ActionList to be copied.</param>
		/// <returns>Newly copied into left hand side ActionList.</returns>
		ActionList& operator=(const ActionList& rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">ActionList to be moved.</param>
		ActionList(ActionList&& rhs) noexcept = default;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">ActionList to be moved.</param>
		/// <returns>Newly moved into left hand side ActionList.</returns>
		ActionList& operator=(ActionList&& rhs) = default;

	protected:
		/// <summary>
		/// Specialized constructor for use by derived classes to ensure correct attribute population.
		/// </summary>
		/// <param name="typeId">Type ID of the derived class.</param>
		explicit ActionList(const RTTI::IdType typeId, const std::string& name = std::string());
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the ActionList.</returns>
		virtual gsl::owner<Library::Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor

#pragma region Accessors
	public:
		/// <summary>
		/// Gets the data handle to the Action objects contained in this ActionList.
		/// </summary>
		/// <returns>Reference to the Action objects.</returns>
		DataType& Actions();

		/// <summary>
		/// Gets the data handle to the Action objects contained in this ActionList.
		/// </summary>
		/// <returns>Reference to the Action objects.</returns>
		const DataType& Actions() const;
#pragma endregion Accessors

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
		/// Collection of Action objects within the Actions prescribed attribute.
		/// </summary>
		DataType& mActions;
#pragma endregion Data Members
	};

#pragma region Factory
	/// <summary>
	/// ActionListFactory class declaration.
	/// </summary>
	ConcreteFactory(ActionList, Scope)
#pragma endregion Factory
}

