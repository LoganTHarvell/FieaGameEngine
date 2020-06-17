#include "pch.h"
#include "PowerUp.h"

#include <cstdlib>
#include <iostream>
#include <ctime>

#include "Character.h"
#include "WorldState.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "EventQueue.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "WorldHelpers.h"
#include "Vector.h"
#include "ActionAnimate.h"
#include "RenderComponent.h"
#include "ActionExpireEntity.h"
#include "ActionRender.h"

namespace GameDesktop
{
	using namespace Library;

	HashMap<std::string, PowerUp::EPowerUp> PowerUp::sStringToPowerUp =
	{
		{"ExtraBomb", EPowerUp::ExtraBomb},
		{"ExplosionExpansion", EPowerUp::ExplosionExpansion},
		{"Accelerator", EPowerUp::Accelerator},
		{"RemoteControl", EPowerUp::RemoteControl},
		{"MaxExplosionExpansion", EPowerUp::MaxExplosionExpansion},
		{"Kick", EPowerUp::Kick},
		{"BoxingGlove", EPowerUp::BoxingGlove},
		{"Skull", EPowerUp::Skull}
	};

	HashMap<PowerUp::EPowerUp, std::string> PowerUp::sPowerUpToString =
	{
		{EPowerUp::ExtraBomb, "ExtraBomb"},
		{EPowerUp::ExplosionExpansion, "ExplosionExpansion"},
		{EPowerUp::Accelerator, "Accelerator"},
		{EPowerUp::RemoteControl, "RemoteControl"},
		{EPowerUp::MaxExplosionExpansion, "MaxExplosionExpansion"},
		{EPowerUp::Kick, "Kick"},
		{EPowerUp::BoxingGlove, "BoxingGlove"},
		{EPowerUp::Skull, "Skull"}
	};

	HashMap<MapTiles::TileType, PowerUp::EPowerUp> PowerUp::sTileToPowerUp =
	{
		{MapTiles::TileType::EXTRA_BOMB, EPowerUp::ExtraBomb},
		{MapTiles::TileType::EXPLOSINON_EXPANDER, EPowerUp::ExplosionExpansion},
		{MapTiles::TileType::ACCELERATOR, EPowerUp::Accelerator},
		{MapTiles::TileType::MAXIMUM_EXPLOSION, EPowerUp::MaxExplosionExpansion},
		{MapTiles::TileType::KICK, EPowerUp::Kick},
		{MapTiles::TileType::BOXING_GLOVE, EPowerUp::BoxingGlove},
		{MapTiles::TileType::UNKNOWN, EPowerUp::Unknown},
		{MapTiles::TileType::EMPTY, EPowerUp::Unknown},
		{MapTiles::TileType::INDESTRUCTIBLE_WALL, EPowerUp::Unknown},
		{MapTiles::TileType::DESTRUCTIBLE_WALL, EPowerUp::Unknown},
		{MapTiles::TileType::PLAYER, EPowerUp::Unknown},
		{MapTiles::TileType::NO_SPAWN, EPowerUp::Unknown},
		{MapTiles::TileType::BOMB, EPowerUp::Unknown},
		{MapTiles::TileType::EXPLOSION, EPowerUp::Unknown}
	};

	HashMap<PowerUp::EPowerUp, PowerUp::RenderInfo> PowerUp::sPowerUpToRender =
	{
		{EPowerUp::Unknown, PowerUp::RenderInfo(0, 6)},
		{EPowerUp::ExtraBomb, PowerUp::RenderInfo(0, 0)},
		{EPowerUp::ExplosionExpansion, PowerUp::RenderInfo(0, 1)},
		{EPowerUp::Accelerator, PowerUp::RenderInfo(0, 2)},
		{EPowerUp::MaxExplosionExpansion, PowerUp::RenderInfo(0, 3)},
		{EPowerUp::Kick, PowerUp::RenderInfo(0, 5)},
		{EPowerUp::BoxingGlove, PowerUp::RenderInfo(0, 4)}
	};

	PowerUp::EPowerUp PowerUp::StringToEPowerUp(const std::string& input)
	{
		return sStringToPowerUp[input];
	}

	std::string PowerUp::EPowerUpToString(EPowerUp input)
	{
		return sPowerUpToString[input];
	}

	PowerUp::EPowerUp PowerUp::TileTypeToEPowerUp(const MapTiles::TileType& input)
	{
		return sTileToPowerUp[input];
	}

	void PowerUp::SetPowerUpType(EPowerUp powerUp)
	{
		mPowerUpType = powerUp;
		RenderInfo info = sPowerUpToRender[powerUp];
		
		RenderComponent& renderComp = GetRenderComponent();
		renderComp.SetColumnIndex(info.ColumnIndex);
		renderComp.SetRowIndex(info.RowIndex);
	}

	void PowerUp::SetPowerUpType(MapTiles::TileType tile)
	{
		SetPowerUpType(TileTypeToEPowerUp(tile));
	}

	PowerUp::PowerUp(const std::string& name)
		: GameObject(TypeIdClass(), name)
	{
		SetTransform(*(new Transform)).SetScale(glm::vec2(PowerUp::Size, PowerUp::Size));
		RenderComponent* newRender = new RenderComponent("PowerUps", 2, 7, 0, 0); 
		Adopt(*newRender, "RenderComponent");
		Adopt(*(new ActionAnimate(20)), "Actions");
		Adopt(*(new ActionRender("Render", 50)), "Actions");
	}

	void PowerUp::Die()
	{
		Adopt(*(new ActionExpireEntity(0)), "Actions");
	}

	void PowerUp::Update(WorldState& worldState)
	{
		// Late update for rending correct sprite
		if (mFirstFrame)
		{
			mFirstFrame = false;
			SetPowerUpType(mPowerUpType); 
		}

		GameObject::Update(worldState);
	}

	gsl::owner<GameObject*> PowerUp::Clone() const
	{
		return new PowerUp(*this);
	}

	const TypeManager::TypeInfo& PowerUp::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ PowerUpTypeKey, Types::String, false, 1, offsetof(PowerUp, mPowerUpString) }
			},
			GameObject::TypeIdClass()
		};

		return typeInfo;
	}
}
