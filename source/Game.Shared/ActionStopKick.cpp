#include "pch.h"
#include "ActionStopKick.h"
#include "Entity.h"
#include "Character.h"
#include "Game.h"

using namespace Library;

namespace GameDesktop
{
	ActionStopKick::ActionStopKick(const std::string& name) :
		Action(TypeIdClass(), name)
	{
	}

	gsl::owner<ActionStopKick*> ActionStopKick::Clone() const
	{
		return new ActionStopKick(*this);
	}

	void ActionStopKick::Update(WorldState& worldState)
	{
		Entity* entity = worldState.Entity;
		assert(entity != nullptr);
		assert(entity->Is(Character::TypeIdClass()));

		Character* character = static_cast<Character*>(entity);

		Bomb* bombRef = character->GetKickedBombRef();
		if (bombRef != nullptr)
		{
			Game* game = static_cast<Game*>(worldState.World->Find(Game::GameKey)->Get<RTTI*>());
			Map& map = game->GetMap();

			bombRef->SetVelocity(glm::vec2(0));
			bombRef->GetTransform().SetLocation(map.GetTileSnappedLocation(*bombRef));
			character->SetBombRef(nullptr);
		}
	}

	std::string ActionStopKick::ToString() const
	{
		return "ActionStopKick";
	}
}