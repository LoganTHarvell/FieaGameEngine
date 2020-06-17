#include "pch.h"
#include "ActionExpireEntity.h"

#include "WorldState.h"
#include "GameTime.h"
#include "World.h"
#include "WorldState.h"
#include "Sector.h"
#include "Game.h"

namespace GameDesktop
{
	const TypeManager::TypeInfo& GameDesktop::ActionExpireEntity::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ExpireTimeKey, Types::Integer, false, 1, offsetof(ActionExpireEntity, mExpireTime)},
			},
			Action::TypeIdClass()
		};
		return typeInfo;
	}

	gsl::owner<ActionExpireEntity*> ActionExpireEntity::Clone() const
	{
		return new ActionExpireEntity(*this);
	}

	void ActionExpireEntity::Update(WorldState& worldState)
	{
		mElapsedTime += static_cast<int>(worldState.GameTime->ElapsedGameTime().count());

		if (mElapsedTime > mExpireTime)
		{
			World::PendingChild childToRemove =
			{
				*worldState.Entity,
				World::PendingChild::State::ToRemove,
				*worldState.Sector,
				""
			};
			worldState.World->PendingChildren().EmplaceBack(std::move(childToRemove));

			Game* game = static_cast<Game*>(worldState.World->Find(Game::GameKey)->Get<RTTI*>());
			Map& map = game->GetMap();
			GameObject* gameObject = static_cast<GameObject*>(worldState.Entity);
			int gameObjectX = static_cast<int>(gameObject->GetTransform().GetLocation()[0]);
			int gameObjectY = static_cast<int>(gameObject->GetTransform().GetLocation()[1]);
			map.SetTile(gameObjectX, gameObjectY, MapTiles::TileType::EMPTY);
		}
	}

	std::string ActionExpireEntity::ToString() const
	{
		std::ostringstream oss;
		oss << Name() << " (ActionExpireEntity)";
		return oss.str();
	}

	ActionExpireEntity::ActionExpireEntity(const std::string& name) 
		: Action(TypeIdClass(), name)
	{
	}

	ActionExpireEntity::ActionExpireEntity(int expireTime, const std::string& name) 
		: Action(TypeIdClass(), name), mExpireTime(expireTime)
	{
	}
}
