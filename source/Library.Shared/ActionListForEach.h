#pragma once

#pragma region Includes
#include "ActionList.h"
#include "WorldState.h"
#include "Factory.h"
#pragma endregion Includes

namespace Library
{
	class ActionListForEach final : public ActionList
	{
		RTTI_DECLARATIONS(ActionListForEach, ActionList)

#pragma region Type Definitions, Constants
	public:
		inline static const std::string OperandKey = "Operand";
		inline static const std::string IndexKey = "Index";
	public:
		static const TypeManager::TypeInfo& TypeInfo();
#pragma endregion Type Definitions, Constants

#pragma region Special Members
	public:
		explicit ActionListForEach(const std::string& name = std::string());
		~ActionListForEach() = default;
		ActionListForEach(const ActionListForEach&) = default;
		ActionListForEach(ActionListForEach&&) = default;
		ActionListForEach& operator=(const ActionListForEach&) = default;
		ActionListForEach& operator=(ActionListForEach&&) = default;
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		virtual gsl::owner<Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor

#pragma region Game Loop
	public:
		virtual void Update(WorldState& worldState);
#pragma endregion Game Loop

#pragma region RTTI Overrides
	public:
		virtual std::string ToString() const override;
#pragma endregion RTTI Overrides

#pragma region Data Members
	private:
		std::string mOperand;
		std::string mIndex;
#pragma endregion Data Members
	};

#pragma region Factory
	ConcreteFactory(ActionListForEach, Scope)
#pragma region Factory
}
