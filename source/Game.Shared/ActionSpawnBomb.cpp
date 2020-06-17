#include "pch.h"
#include "ActionSpawnBomb.h"
#include "Sector.h"
#include "Character.h"
#include "Bomb.h"
#include "Sector.h"
#include "WorldState.h"
#include "Transform.h"
#include "Game.h"
#include "World.h"
#include "Map.h"
#include "AudioManager.h"

namespace GameDesktop
{
	ActionSpawnBomb::ActionSpawnBomb(const std::string& name) :
		Action(TypeIdClass(), name)
	{
	}

	gsl::owner<ActionSpawnBomb*> ActionSpawnBomb::Clone() const
	{
		return new ActionSpawnBomb(*this);
	}

	void ActionSpawnBomb::Update(WorldState& worldState)
	{
		Data* bombCount = Search(Character::BombCountKey);
		assert(bombCount != nullptr);

		if (bombCount->Get<int>() != 0)
		{
			AudioManager::Instance()->PlaySound("PlaceBomb");

			Character* character = static_cast<Character*>(worldState.Entity);
			assert(character != nullptr);

			// bombCount--
			bombCount->Set<int>(bombCount->Get<int>() - 1);

			Bomb* bomb = new Bomb(*worldState.GameTime, *character, "Bomb");
			
			bomb->SetExplosionDistance(Search(Character::ExplosionDistanceKey)->Get<int>());
			worldState.Sector->Adopt(*bomb, Sector::EntitiesKey);


			//Data* transform = Search(TransformKey);
			//assert(transform != nullptr);
			Game* game = static_cast<Game*>(worldState.World->Find(Game::GameKey)->Get<RTTI*>());
			Map& map = game->GetMap();


			bomb->GetTransform().SetLocation(map.GetTileSnappedLocation(*character));
			map.SetTile(*bomb);
			character->AddBombRef(*bomb);
		}
	}

	std::string ActionSpawnBomb::ToString() const
	{
		std::ostringstream oss;
		oss << Name() << " (ActionSpawnBomb)";
		return oss.str();
	}
}