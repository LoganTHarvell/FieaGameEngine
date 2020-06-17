#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "ActionIncrement.h"

// First Party
#include "Entity.h"
#pragma endregion Includes

namespace Library
{
	const TypeManager::TypeInfo& ActionIncrement::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ OperandKey, Types::String, false, 1, offsetof(ActionIncrement, mOperand) },
				{ IncrementStepKey, Types::Integer, false, 1, offsetof(ActionIncrement, mIncrementStep) },
				{ IndexKey, Types::String, false, 1, offsetof(ActionIncrement, mIndex) }
			},

			Action::TypeIdClass()
		};

		return typeInfo;
	}

	ActionIncrement::ActionIncrement(const std::string& name) : Action(TypeIdClass(), name)
	{
	}

	gsl::owner<Scope*> ActionIncrement::Clone() const
	{
		return new ActionIncrement(*this);
	}

	void ActionIncrement::Update(WorldState&)
	{
		Data* operand = Search(mOperand);
		Data* index = Search(mIndex);
#ifdef _DEBUG
		if (index != nullptr)
		{
			assert(index->Type() == Types::Integer);
		}
#endif		
		if (operand && operand->Type() == Types::Integer && operand->Size() > 0)
		{
			const int32_t i = index == nullptr ? 0 : index->Get<int32_t>();
			operand->Get<int>(i) += mIncrementStep;
		}
	}

	std::string ActionIncrement::ToString() const
	{
		//std::ostringstream oss;
		//oss << Name() << " (ActionIncrement)";
		//return oss.str();
		return Name() + " (ActionIncrement)";
	}
}