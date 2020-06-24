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
	const SignatureList& ActionIncrement::Signatures()
	{
		static const SignatureList signatures =
		{
			{ OperandKey, Types::String, false, 1, offsetof(ActionIncrement, mOperand) },
			{ IncrementStepKey, Types::Integer, false, 1, offsetof(ActionIncrement, mIncrementStep) }
		};

		return signatures;
	}

	ActionIncrement::ActionIncrement(std::string name) : Entity(TypeIdClass(), std::move(name))
	{
	}

	gsl::owner<Scope*> ActionIncrement::Clone() const
	{
		return new ActionIncrement(*this);
	}

	void ActionIncrement::Update(WorldState&)
	{
		Data* operand = Search(mOperand);

		if (operand && operand->Type() == Types::Integer && operand->Size() > 0)
		{
			operand->Get<int>() += mIncrementStep;
		}
	}

	std::string ActionIncrement::ToString() const
	{
		std::ostringstream oss;
		oss << Name() << " (ActionIncrement)";
		return oss.str();
	}
}