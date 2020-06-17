#pragma region Includes
#include "pch.h"
#include "ActionListForEach.h"
#pragma endregion Includes

namespace Library
{
	const TypeManager::TypeInfo& ActionListForEach::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{OperandKey, Types::String, false, 1, offsetof(ActionListForEach, mOperand)},
				{IndexKey, Types::String, false, 1, offsetof(ActionListForEach, mIndex)}
			},
			ActionList::TypeIdClass()
		};
		return typeInfo;
	}

	ActionListForEach::ActionListForEach(const std::string& name) : ActionList(TypeIdClass(), name)
	{
	}

	gsl::owner<Scope*> ActionListForEach::Clone() const
	{
		return new ActionListForEach(*this);
	}

	void ActionListForEach::Update(WorldState& worldState)
	{
		Data* operand = Search(mOperand);
		Data* index = Search(mIndex);

		assert(operand != nullptr);
		assert(index != nullptr && index->Type() == Types::Integer);

		for (size_t i = 0; i < operand->Size(); ++i)
		{
			*index = static_cast<int32_t>(i);
			ActionList::Update(worldState);
		}
	}

	std::string ActionListForEach::ToString() const
	{
		std::ostringstream oss;
		oss << Name() << " (ActionListForEach)";
		return oss.str();
	}
	
}