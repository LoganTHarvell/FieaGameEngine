#pragma region Includes
#include "pch.h"
#include "ActionCharacterMovementInput.h"

// First Party
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "GameTime.h"

#include "Character.h"
#include "PlayerInputReaction.h"
#include "WorldHelpers.h"

#pragma endregion Includes

using namespace Library;

namespace GameDesktop
{
	const TypeManager::TypeInfo& ActionCharacterMovementInput::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ GameObjectNameKey, Types::String, false, 1, offsetof(ActionCharacterMovementInput, mCharacterName) },
				{ InputVec4Key, Types::Vector, false, 1, offsetof(ActionCharacterMovementInput, mInputVec4) }
			},

			Action::TypeIdClass()
		};

		return typeInfo;
	}

	ActionCharacterMovementInput::ActionCharacterMovementInput(const std::string& name) 
		: Action(TypeIdClass(), name)
	{
	}

	gsl::owner<Scope*> ActionCharacterMovementInput::Clone() const
	{
		return new ActionCharacterMovementInput(*this);
	}

	void ActionCharacterMovementInput::Update(WorldState& worldState)
	{
		Character* character = WorldHelpers::WorldSearchCharacter(worldState, mCharacterName);
		assert(character != nullptr);

		auto* myCharacter = static_cast<Character*>(character);
		myCharacter->SetInputVelocity(glm::vec2(Search(PlayerInputReaction::XAxisKey)->Get<float>(), Search(PlayerInputReaction::YAxisKey)->Get<float>()));
	}

	std::string ActionCharacterMovementInput::ToString() const
	{
		return "ActionCharacterMovementInput"; // Unused
	}
}