#include "pch.h"
#include "Map.h"
#include "ActionRender.h"
#include "Sector.h"
#include "GameObject.h"
#include "PowerUp.h"

namespace GameDesktop
{
	const TypeManager::TypeInfo& Map::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ DestructibleWallsKey, Types::Integer, false, 1, offsetof(Map, mDestructibleWalls) },
				{ ExtraBombsKey, Types::Integer, false, 1, offsetof(Map, mExtraBombs) },
				{ ExplosionExpandersKey, Types::Integer, false, 1, offsetof(Map, mExplosionExpanders) },
				{ AcceleratorsKey, Types::Integer, false, 1, offsetof(Map, mAccelerators) },
				{ MaximumExplosionsKey, Types::Integer, false, 1, offsetof(Map, mMaximumExplosions) },
				{ KicksKey, Types::Integer, false, 1, offsetof(Map, mKicks) },
				{ BoxingGlovesKey, Types::Integer, false, 1, offsetof(Map, mBoxingGloves) }
			},
			Attributed::TypeIdClass()
		};
		return typeInfo;
	}

	Map::Map(World* world) : Attributed(TypeIdClass()), mWorld(world)
	{
	
	}


	Map::Map(World* world, int width, int height) : Attributed(TypeIdClass()), mWorld(world), 
		mMapWidth(width), mScreenWidth(width), mScreenHeight(height), mMapHeight(height)
	{
		scalingFactorX = mMapWidth / unscaledPixelWidth;
		scalingFactorY = mMapHeight / unscaledPixelWidth;

		mMapHeight = height * mTileHeight / mTileWidth;

		mTotalPowerups = mExtraBombs + mExplosionExpanders + mAccelerators + mMaximumExplosions + mKicks + mBoxingGloves;
		assert(mDestructibleWalls >= mTotalPowerups);
	}

	bool Map::LoadFromFile(const std::string& filename)
	{
		std::fstream file(filename, std::ios::in);

		if (!file.is_open())
		{
			return false;
		}

		std::string header;
		file >> header;

		if (header != VALID_HEADER)
		{
			return false;
		}

		int specifiedWidth;
		int specifiedHeight;

		file >> specifiedWidth;
		file >> specifiedHeight;

		if (mTileHeight != specifiedHeight || mTileWidth != specifiedWidth)
		{
			return false;
		}

		mTiles.Resize(mTileHeight);

		for (int i = 0; i < mTileHeight; ++i)
		{
			mTiles[i].Resize(mTileWidth);
		}

		int tile;

		for (int i = 0; i < mTileHeight; ++i)
		{
			for (int j = 0; j < mTileWidth; ++j)
			{
				file >> tile;
				mTiles[i][j].initial = static_cast<MapTiles::TileType>(tile);
				mTiles[i][j].base = mTiles[i][j].initial;
			}
		}

		return true;
	}

	void Map::ResetMap()
	{
		for (int i = 0; i < mTileHeight; ++i)
		{
			for (int j = 0; j < mTileWidth; ++j)
			{
				mTiles[i][j].base = mTiles[i][j].initial;
				mTiles[i][j].power = MapTiles::TileType::EMPTY;
				mTiles[i][j].object = nullptr;
			}
		}
	}

	int Map::GetHeight() const
	{
		return mTileHeight;
	}

	int Map::GetWidth() const
	{
		return mTileWidth;
	}

	Map::tile Map::GetTile(int x, int y)
	{
		int tile_x, tile_y;
		PixelConversion(x, y, &tile_x, &tile_y);		
		return mTiles[tile_y][tile_x];
	}

	Map::tile Map::GetTileFromId(int x, int y)
	{
		return mTiles[y][x];
	}

	bool Map::SetTile(GameObject& object)
	{
		glm::vec2 position = object.GetTransform().GetLocation();
		int x = static_cast<int>(position.x);
		int y = static_cast<int>(position.y);
		const auto& it = MapTiles::sObjectToTileType.Find(object.TypeIdInstance());
		if (it != MapTiles::sObjectToTileType.end())
		{
			MapTiles::TileType tile = (*it).second;
			return SetTile(x, y, tile, &object);
		}
		else
		{
			return false;
		}
	}

	bool Map::SetTile(int x, int y, MapTiles::TileType type, GameObject* object)
	{
		int tile_x, tile_y;
		PixelConversion(x, y, &tile_x, &tile_y);

		//If you are trying to destroy a destructible wall and set it to empty, 
		//we need to see if the game should spawn a powerup instead 
		if (mTiles[tile_y][tile_x].base == MapTiles::TileType::DESTRUCTIBLE_WALL && type == MapTiles::TileType::EMPTY)
		{
			bool result = SpawnPowerup(tile_x, tile_y);
			if (!result)
			{
				mTiles[tile_y][tile_x].base = MapTiles::TileType::EMPTY;
			}
			//mTiles[tile_y][tile_x].object = nullptr;
		}
		else
		{
			mTiles[tile_y][tile_x].base = type;
			mTiles[tile_y][tile_x].object = object; 
		}
		return true;
	}

	bool Map::SetTileFromId(int x, int y, MapTiles::TileType type, GameObject* object)
	{
		if (mTiles[y][x].base == MapTiles::TileType::DESTRUCTIBLE_WALL && type == MapTiles::TileType::EMPTY)
		{
			bool result = SpawnPowerup(x, y);
			if (!result)
			{
				mTiles[y][x].base = MapTiles::TileType::EMPTY;
			}
			//mTiles[y][x].object = nullptr;
		}
		else
		{
			mTiles[y][x].base = type;
			mTiles[y][x].object = object;
		}
		return true;
	}

	void Map::PixelConversion(int x, int y, int* tile_x, int* tile_y)
	{
		*tile_x = static_cast<int>((x / scalingFactorX) / TileSize);
		*tile_y = static_cast<int>((y / scalingFactorY) / TileSize);
	}

	glm::vec2 Map::GetTileSnappedLocation(GameObject& object)
	{
		glm::vec2 ret;
		glm::vec2 position = object.GetTransform().GetLocation();
		int tile_x, tile_y;
		tile_x = static_cast<int>((position.x / scalingFactorX) / TileSize);
		tile_y = static_cast<int>((position.y / scalingFactorY) / TileSize);

		ret.x = tile_x * scalingFactorX * TileSize + (TileSize * scalingFactorX / 2);
		ret.y = tile_y * scalingFactorY * TileSize + (TileSize * scalingFactorY / 2);

		return ret;
	}

	glm::vec2 Map::GetTileSnappedLocation(int tile_x, int tile_y)
	{
		glm::vec2 ret;
		ret.x = tile_x * scalingFactorX * TileSize + (TileSize * scalingFactorX / 2);
		ret.y = tile_y * scalingFactorY * TileSize + (TileSize * scalingFactorY / 2);
		return ret;
	}

	bool Map::SpawnPowerup(int x, int y)
	{
		if (Map::PowerupCheck(x, y))
		{
			mTiles[y][x].base = mTiles[y][x].power;
			mTiles[y][x].power = MapTiles::TileType::EMPTY;

			PowerUp* power = static_cast<PowerUp*>(SpawnObject("PowerUp","power", x, y));
			power->SetPowerUpType(mTiles[y][x].base);
			mTiles[y][x].object = power;
			return true;
		}
		return false;
	}

	GameObject* Map::SpawnObject(const std::string& type, const std::string& name, int x, int y)
	{
		Sector* sector = static_cast<Sector*>(mWorld->Sectors().Get<Scope*>(0));
		GameObject* obj = static_cast<GameObject*>(sector->CreateEntity(type, name));
		obj->GetTransform().SetLocation(GetTileSnappedLocation(x, y));
		//obj->GetRenderComponent()["Priority"] = 2;
		return obj;
	}

	GameObject* Map::GetGameObject(const glm::vec3 pos)
	{
		int tile_x, tile_y;
		int pos_x = static_cast<int>(pos.x);
		int pos_y = static_cast<int>(pos.y);
		PixelConversion(pos_x, pos_y, &tile_x, &tile_y);
		return (mTiles[tile_y][tile_x].object);
	}

	gsl::owner<Scope*> Map::Clone() const
	{
		return new Map(*this);
	}

	bool Map::PowerupCheck(int x, int y) const
	{
		if (mTiles[y][x].power == MapTiles::TileType::EMPTY)
		{
			return false;
		}
		return true;
	}

	void Map::PopulateMap()
	{
		// Spawn background
		GameObject* background = new GameObject("Level 1 Background");
		background->SetTransform(*(new Transform(glm::vec4(mMapWidth / 2, mMapHeight / 2, 0, 0), 0.0f, glm::vec4(unscaledPixelWidth, unscaledPixelHeight, 1, 0))));
		background->AddRenderComponent(*(new RenderComponent("Level1")));
		background->Adopt(*(new ActionRender), Entity::ActionsKey);
		mWorld->Sectors().Get<Scope*>()->Adopt(*background, Sector::EntitiesKey);

		int wallsToSpawn = mDestructibleWalls;

		//Populates map with destructible walls
		while (wallsToSpawn != 0)
		{
			bool wallSpawned = false;
			while (!wallSpawned)
			{
				int x = rand() % mTileWidth;
				int y = rand() % mTileHeight;
				if (mTiles[y][x].base == MapTiles::TileType::EMPTY)
				{
					mTiles[y][x].base = MapTiles::TileType::DESTRUCTIBLE_WALL;
					mTiles[y][x].object = SpawnObject("DestructibleWall", "Dwall", x, y);
					wallSpawned = true;
				}
			}
			--wallsToSpawn;
		}

		for (int i = 0; i < mTileHeight; ++i)
		{
			for (int j = 0; j < mTileWidth; ++j)
			{
				if (mTiles[i][j].base == MapTiles::TileType::NO_SPAWN)
				{
					mTiles[i][j].base = MapTiles::TileType::EMPTY;
				}
			}
		}

		//Populates map with powerups, on spots with destructible walls
		PopulatePowerup(mExtraBombs, MapTiles::TileType::EXTRA_BOMB);
		PopulatePowerup(mExplosionExpanders, MapTiles::TileType::EXPLOSINON_EXPANDER);
		PopulatePowerup(mAccelerators, MapTiles::TileType::ACCELERATOR);
		PopulatePowerup(mMaximumExplosions, MapTiles::TileType::MAXIMUM_EXPLOSION);
		PopulatePowerup(mKicks, MapTiles::TileType::KICK);
		PopulatePowerup(mBoxingGloves, MapTiles::TileType::BOXING_GLOVE);
	}

	void Map::PopulatePowerup(int numberOfPowerups, MapTiles::TileType powerupType)
	{
		while (numberOfPowerups != 0)
		{
			bool powerupSpawned = false;
			while (!powerupSpawned)
			{
				int x = rand() % mTileWidth;
				int y = rand() % mTileHeight;
				if (mTiles[y][x].base == MapTiles::TileType::DESTRUCTIBLE_WALL && mTiles[y][x].power == MapTiles::TileType::EMPTY)
				{
					mTiles[y][x].power = powerupType;
					powerupSpawned = true;
				}
			}
			--numberOfPowerups;
			++powerupsSpawned;
		}
	}
}
