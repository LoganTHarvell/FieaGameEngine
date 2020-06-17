#include "pch.h"
#include "Bomb.h"
#include "ActionRender.h"
#include "ActionAnimate.h"
#include "Character.h"
#include "World.h"
#include "WorldState.h"
#include "Sector.h"
#include "Explosion.h"
#include "Map.h"
#include "AudioManager.h"
#include "DestructibleWall.h"

using namespace std::chrono;
using namespace std::chrono_literals;
using namespace Library;

namespace GameDesktop
{
	const Library::TypeManager::TypeInfo& Bomb::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ DelayKey, Types::Integer, false, 1, offsetof(Bomb, mDelay) },
				{ ExplosionDistanceKey, Types::Integer, false, 1, offsetof(Bomb, mExplosionDistance) },
				{ IsPunchedKey, Types::Float, false, 1, offsetof(Bomb, mIsPunched) },
			},

			GameObject::TypeIdClass()
		};

		return typeInfo;
	}

	Bomb::Bomb(const GameTime& gameTime, Character& character, const std::string& name) :
		GameObject(TypeIdClass(), name), mCharacterRef(&character), mDelay(3000), mExpiredTime(gameTime.CurrentTime() + milliseconds(mDelay))
	{
		SetTransform(*(new Transform)).SetScale(glm::vec2(Bomb::Size, Bomb::Size));
		AddRenderComponent(*(new RenderComponent(SpriteKey, 4, 1, 0, 0)));
		Adopt(*(new ActionRender("Render", 1)), "Actions");
		Adopt(*(new ActionAnimate(300, "Animate")), "Actions");
	}
	void Bomb::Update(Library::WorldState& worldState)
	{
		GameObject::Update(worldState);

		if (!mIsPunched && worldState.GameTime->CurrentTime() > mExpiredTime)
		{
			Explode(worldState);
		}
	}

	void Bomb::Explode(WorldState& state)
	{
		if (mCharacterRef != nullptr)
		{
			mCharacterRef->RemoveBombRef(*this);
			Data* bombCount = mCharacterRef->Search(Character::BombCountKey);
			assert(bombCount != nullptr);
			bombCount->Set<int>(bombCount->Get<int>() + 1);
		}

		Game* game = static_cast<Game*>(state.World->Find(Game::GameKey)->Get<RTTI*>());
		Map& map = game->GetMap();
		glm::vec3 loc = GetTransform().GetLocation();
		
		map.SetTile(static_cast<int>(loc.x), static_cast<int>(loc.y), MapTiles::TileType::EMPTY);

		World::PendingChild childToRemove =
		{
			*this,
			World::PendingChild::State::ToRemove,
			*state.Sector,
			std::string()
		};

		state.World->PendingChildren().EmplaceBack(std::move(childToRemove));

		AudioManager::Instance()->PlaySound("BombExplode");

		SpawnExplosions(state);
	}

	void Bomb::SetExplosionDistance(int distance)
	{
		mExplosionDistance = distance;
	}

	void Bomb::Kick(glm::vec2& dir)
	{
		SetVelocity(dir * KickSpeed);
	}

	void Bomb::SpawnExplosions(WorldState& state)
	{
		Game* game = static_cast<Game*>(state.World->Find(Game::GameKey)->Get<RTTI*>());
		Map& map = game->GetMap();
		int bombPixelX = static_cast<int>(map.GetTileSnappedLocation(*this)[0]);
		int bombPixelY = static_cast<int>(map.GetTileSnappedLocation(*this)[1]);
		int bombTileX;
		int bombTileY;
		map.PixelConversion(bombPixelX, bombPixelY, &bombTileX, &bombTileY);

		Explosion* explosion = new Explosion("Explosion");
		state.Sector->Adopt(*explosion, Sector::EntitiesKey);
		explosion->SetExplosionChunk(Explosion::EExplosionChunk::Center);
		glm::vec2 newTransform = map.GetTileSnappedLocation(bombTileX, bombTileY);
		explosion->GetTransform().SetLocation(newTransform);
		map.SetTile(static_cast<int>(newTransform[0]), static_cast<int>(newTransform[1]), MapTiles::TileType::EXPLOSION, explosion);

		bool keepExpanding = true;
		Map::tile newTile;

		//Up
		for (int i = 1; i < mExplosionDistance; ++i)
		{
			newTransform = map.GetTileSnappedLocation(bombTileX, bombTileY - i);

			if (bombTileY - i < 0 || bombTileY - i >= map.GetHeight())
			{
				keepExpanding = false;
				break;
			}

			newTile = map.GetTile(static_cast<int>(newTransform[0]), static_cast<int>(newTransform[1]));

			if (newTile.base == MapTiles::TileType::DESTRUCTIBLE_WALL)
			{
				GameObject* wall = newTile.object;

				assert(wall->Is(DestructibleWall::TypeIdClass()));

				static_cast<DestructibleWall*>(wall)->Die();

				keepExpanding = false;
				break;
			}

			if (newTile.base != MapTiles::TileType::PLAYER && newTile.base != MapTiles::TileType::EMPTY && newTile.base != MapTiles::TileType::NO_SPAWN && newTile.base != MapTiles::TileType::UNKNOWN)
			{
				keepExpanding = false;
				break;
			}

			explosion = new Explosion("Explosion");
			state.Sector->Adopt(*explosion, Sector::EntitiesKey);
			explosion->SetExplosionChunk(Explosion::EExplosionChunk::VerticalUpMiddle);
			explosion->GetTransform().SetLocation(newTransform);
			map.SetTile(static_cast<int>(newTransform[0]), static_cast<int>(newTransform[1]), MapTiles::TileType::EXPLOSION, explosion);
		}

		if (keepExpanding && !(bombTileY - mExplosionDistance < 0) && !(bombTileY - mExplosionDistance > map.GetHeight()))
		{
			newTransform = map.GetTileSnappedLocation(bombTileX, bombTileY - mExplosionDistance);
			newTile = map.GetTile(static_cast<int>(newTransform[0]), static_cast<int>(newTransform[1]));

			if (keepExpanding && newTile.base == MapTiles::TileType::DESTRUCTIBLE_WALL)
			{
				GameObject* wall = newTile.object;

				assert(wall->Is(DestructibleWall::TypeIdClass()));

				static_cast<DestructibleWall*>(wall)->Die();

				keepExpanding = false;
			}

			if (keepExpanding && !(newTile.base != MapTiles::TileType::PLAYER && newTile.base != MapTiles::TileType::EMPTY && newTile.base != MapTiles::TileType::NO_SPAWN && newTile.base != MapTiles::TileType::UNKNOWN))
			{
				explosion = new Explosion("Explosion");
				state.Sector->Adopt(*explosion, Sector::EntitiesKey);
				explosion->SetExplosionChunk(Explosion::EExplosionChunk::VerticalUpTop);
				explosion->GetTransform().SetLocation(newTransform);
				map.SetTile(static_cast<int>(newTransform[0]), static_cast<int>(newTransform[1]), MapTiles::TileType::EXPLOSION, explosion);
			}
		}
		
		
		keepExpanding = true;

		//Left
		for (int i = 1; i < mExplosionDistance; ++i)
		{
			newTransform = map.GetTileSnappedLocation(bombTileX - i, bombTileY);

			if (bombTileX - i < 0 || bombTileX - i >= map.GetWidth())
			{
				keepExpanding = false;
				break;
			}

			newTile = map.GetTile(static_cast<int>(newTransform[0]), static_cast<int>(newTransform[1]));

			if (newTile.base == MapTiles::TileType::DESTRUCTIBLE_WALL)
			{
				GameObject* wall = newTile.object;

				assert(wall->Is(DestructibleWall::TypeIdClass()));

				static_cast<DestructibleWall*>(wall)->Die();

				keepExpanding = false;
				break;
			}

			if (newTile.base != MapTiles::TileType::PLAYER && newTile.base != MapTiles::TileType::EMPTY && newTile.base != MapTiles::TileType::NO_SPAWN && newTile.base != MapTiles::TileType::UNKNOWN)
			{
				keepExpanding = false;
				break;
			}

			

			explosion = new Explosion("Explosion");
			state.Sector->Adopt(*explosion, Sector::EntitiesKey);
			explosion->SetExplosionChunk(Explosion::EExplosionChunk::HorizontalLeftMiddle);
			explosion->GetTransform().SetLocation(newTransform);
			map.SetTile(static_cast<int>(newTransform[0]), static_cast<int>(newTransform[1]), MapTiles::TileType::EXPLOSION, explosion);
		}

		
		if (keepExpanding && !(bombTileX - mExplosionDistance < 0) && !(bombTileX - mExplosionDistance >= map.GetWidth()))
		{
			newTransform = map.GetTileSnappedLocation(bombTileX - mExplosionDistance, bombTileY);
			newTile = map.GetTile(static_cast<int>(newTransform[0]), static_cast<int>(newTransform[1]));

			if (newTile.base == MapTiles::TileType::DESTRUCTIBLE_WALL)
			{

				GameObject* wall = newTile.object;

				assert(wall->Is(DestructibleWall::TypeIdClass()));

				static_cast<DestructibleWall*>(wall)->Die();

				keepExpanding = false;
			}

			if (!(newTile.base != MapTiles::TileType::PLAYER && newTile.base != MapTiles::TileType::EMPTY && newTile.base != MapTiles::TileType::NO_SPAWN && newTile.base != MapTiles::TileType::UNKNOWN))
			{
				explosion = new Explosion("Explosion");
				state.Sector->Adopt(*explosion, Sector::EntitiesKey);
				explosion->SetExplosionChunk(Explosion::EExplosionChunk::HorizontalLeftEnd);
				explosion->GetTransform().SetLocation(newTransform);
				map.SetTile(static_cast<int>(newTransform[0]), static_cast<int>(newTransform[1]), MapTiles::TileType::EXPLOSION, explosion);
			}
		}
		

		keepExpanding = true;

		//Down
		for (int i = 1; i < mExplosionDistance; ++i)
		{
			newTransform = map.GetTileSnappedLocation(bombTileX, bombTileY + i);

			if (bombTileY + i < 0 || bombTileY + i >= map.GetHeight())
			{
				keepExpanding = false;
				break;
			}

			newTile = map.GetTile(static_cast<int>(newTransform[0]), static_cast<int>(newTransform[1]));
			
			if (newTile.base == MapTiles::TileType::DESTRUCTIBLE_WALL)
			{
				GameObject* wall = newTile.object;

				assert(wall->Is(DestructibleWall::TypeIdClass()));

				static_cast<DestructibleWall*>(wall)->Die();

				keepExpanding = false;
				break;
			}

			if (newTile.base != MapTiles::TileType::PLAYER && newTile.base != MapTiles::TileType::EMPTY && newTile.base != MapTiles::TileType::NO_SPAWN && newTile.base != MapTiles::TileType::UNKNOWN)
			{
				keepExpanding = false;
				break;
			}

			

			explosion = new Explosion("Explosion");
			state.Sector->Adopt(*explosion, Sector::EntitiesKey);
			explosion->SetExplosionChunk(Explosion::EExplosionChunk::VerticalDownMiddle);
			explosion->GetTransform().SetLocation(newTransform);
			map.SetTile(static_cast<int>(newTransform[0]), static_cast<int>(newTransform[1]), MapTiles::TileType::EXPLOSION, explosion);
		}

		
		if (keepExpanding && !(bombTileY + mExplosionDistance < 0) && !(bombTileY + mExplosionDistance > map.GetHeight()))
		{
			newTransform = map.GetTileSnappedLocation(bombTileX, bombTileY + mExplosionDistance);
			newTile = map.GetTile(static_cast<int>(newTransform[0]), static_cast<int>(newTransform[1]));


			if (newTile.base == MapTiles::TileType::DESTRUCTIBLE_WALL)
			{
				
				GameObject* wall = newTile.object;

				assert(wall->Is(DestructibleWall::TypeIdClass()));

				static_cast<DestructibleWall*>(wall)->Die();

				keepExpanding = false;
			}

			if (!(newTile.base != MapTiles::TileType::PLAYER && newTile.base != MapTiles::TileType::EMPTY && newTile.base != MapTiles::TileType::NO_SPAWN && newTile.base != MapTiles::TileType::UNKNOWN))
			{
				
				explosion = new Explosion("Explosion");
				state.Sector->Adopt(*explosion, Sector::EntitiesKey);
				explosion->SetExplosionChunk(Explosion::EExplosionChunk::VerticalDownBot);
				explosion->GetTransform().SetLocation(newTransform);
				map.SetTile(static_cast<int>(newTransform[0]), static_cast<int>(newTransform[1]), MapTiles::TileType::EXPLOSION, explosion);
			}
		}

		

		keepExpanding = true;

		//Right
		for (int i = 1; i < mExplosionDistance; ++i)
		{
			newTransform = map.GetTileSnappedLocation(bombTileX + i, bombTileY);

			if (bombTileX + i < 0 || bombTileX + i >= map.GetWidth())
			{
				keepExpanding = false;
				break;
			}

			newTile = map.GetTile(static_cast<int>(newTransform[0]), static_cast<int>(newTransform[1]));

			if (newTile.base == MapTiles::TileType::DESTRUCTIBLE_WALL)
			{
				GameObject* wall = newTile.object;

				assert(wall->Is(DestructibleWall::TypeIdClass()));

				static_cast<DestructibleWall*>(wall)->Die();

				keepExpanding = false;
				break;
			}

			if (newTile.base != MapTiles::TileType::PLAYER && newTile.base != MapTiles::TileType::EMPTY && newTile.base != MapTiles::TileType::NO_SPAWN && newTile.base != MapTiles::TileType::UNKNOWN)
			{
				keepExpanding = false;
				break;
			}

			

			explosion = new Explosion("Explosion");
			state.Sector->Adopt(*explosion, Sector::EntitiesKey);
			explosion->SetExplosionChunk(Explosion::EExplosionChunk::HorizontalRightMiddle);
			explosion->GetTransform().SetLocation(newTransform);
			map.SetTile(static_cast<int>(newTransform[0]), static_cast<int>(newTransform[1]), MapTiles::TileType::EXPLOSION, explosion);
		}

		

		if (keepExpanding && !(bombTileX + mExplosionDistance < 0) && !(bombTileX + mExplosionDistance >= map.GetWidth()))
		{
			newTransform = map.GetTileSnappedLocation(bombTileX + mExplosionDistance, bombTileY);
			newTile = map.GetTile(static_cast<int>(newTransform[0]), static_cast<int>(newTransform[1]));

			if (newTile.base == MapTiles::TileType::DESTRUCTIBLE_WALL)
			{
				

				GameObject* wall = newTile.object;

				assert(wall->Is(DestructibleWall::TypeIdClass()));

				static_cast<DestructibleWall*>(wall)->Die();

				keepExpanding = false;
			}

			if (!(newTile.base != MapTiles::TileType::PLAYER && newTile.base != MapTiles::TileType::EMPTY && newTile.base != MapTiles::TileType::NO_SPAWN && newTile.base != MapTiles::TileType::UNKNOWN))
			{

				explosion = new Explosion("Explosion");
				state.Sector->Adopt(*explosion, Sector::EntitiesKey);
				explosion->SetExplosionChunk(Explosion::EExplosionChunk::HorizontalRightEnd);
				explosion->GetTransform().SetLocation(newTransform);
				map.SetTile(static_cast<int>(newTransform[0]), static_cast<int>(newTransform[1]), MapTiles::TileType::EXPLOSION, explosion);
			}
		}
		
	}

	void Bomb::SetCharacterRef(Character* character)
	{
		mCharacterRef = character;
	}

	gsl::owner<GameObject*> Bomb::Clone() const
	{
		return new Bomb(*this);
	}
}
