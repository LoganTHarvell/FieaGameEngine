#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "MapTiles.h"
#include "Vector.h"
#include "Attributed.h"
#include "TypeManager.h"
#include "World.h"

#define VALID_HEADER "BOMBERMAN_LEVEL"

using namespace std::string_literals;
using namespace Library;

namespace GameDesktop
{
	class GameObject;

	class Map final : public Attributed
	{
		RTTI_DECLARATIONS(Map, Attributed)

	public:

		static const TypeManager::TypeInfo& TypeInfo();

		inline static const std::string DestructibleWallsKey = "DestructibleWalls"s;
		inline static const std::string ExtraBombsKey = "ExtraBombs"s;
		inline static const std::string ExplosionExpandersKey = "ExplosionExpanders"s;
		inline static const std::string AcceleratorsKey = "Accelerators"s;
		inline static const std::string MaximumExplosionsKey = "MaximumExplosions"s;
		inline static const std::string KicksKey = "Kicks"s;
		inline static const std::string BoxingGlovesKey = "BoxingGloves"s;

		inline static float scalingFactorX;
		inline static float scalingFactorY;

		inline static const int TileSize = 16;

		struct tile final
		{
			MapTiles::TileType base = MapTiles::TileType::EMPTY;
			MapTiles::TileType power = MapTiles::TileType::EMPTY;
			MapTiles::TileType initial = MapTiles::TileType::EMPTY;
			GameObject* object = nullptr;

			bool operator==(const tile& rhs) const
			{
				return (this->base == rhs.base && this->power == rhs.power /*&& this->object == rhs.object*/);
			}
		};

		Map(World* world = nullptr);

		/// <summary>
		/// Constructor
		/// </summary>
		explicit Map(World* world, int width, int height);

		/// <summary>
		/// Destructor
		/// </summary>
		~Map() = default;

		Map(const Map& rhs) = default;
		Map(Map&& rhs) = default;
		Map& operator=(const Map& rhs) = default;
		Map& operator=(Map&& rhs) = default;

		/// <summary>
		/// Loads level from .txt file
		/// </summary>
		/// <param name="filename">Path to level</param>
		/// <returns>True if loaded file, false if not</returns>
		bool LoadFromFile(const std::string& filename);

		/// <summary>
		/// Populates map with destructible walls 
		/// </summary>
		void PopulateMap();

		/// <summary>
		/// Gets TileType at pixel(x,y)
		/// </summary>
		/// <param name="x">Xth index</param>
		/// <param name="y">Yth index</param>
		/// <returns></returns>
		tile GetTile(int x, int y);

		tile GetTileFromId(int x, int y);

		/// <summary>
		/// Get width of map in tiles (currently 13)
		/// </summary>
		/// <returns>Width</returns>
		int GetWidth() const;

		/// <summary>
		/// Get heightof map in tiles (currently 11)
		/// </summary>
		/// <returns></returns>
		int GetHeight() const;
		
		/// <summary>
		/// Sets tile given a GameObject 
		/// </summary>
		/// <param name="object"></param>
		/// <returns></returns>
		bool SetTile(GameObject& object);

		/// <summary>
		/// Sets tile at specific position
		/// </summary>
		/// <param name="x">Xth index</param>
		/// <param name="y">Yth index</param>
		/// <returns>True if tile can be destroyed, false if not</returns>
		bool SetTile(int x, int y, MapTiles::TileType type, GameObject* object = nullptr);

		/// <summary>
		/// Sets tile at specific position
		/// </summary>
		/// <param name="x">Xth index</param>
		/// <param name="y">Yth index</param>
		/// <returns>True if tile can be destroyed, false if not</returns>
		bool SetTileFromId(int x, int y, MapTiles::TileType type, GameObject* object = nullptr);

		/// <summary>
		/// Spawns powerup at specific position
		/// </summary>
		/// <param name="x">Xth index</param>
		/// <param name="y">Yth index</param>
		/// <param name="powerup">Powerup</param>
		/// <returns>True if powerup can be spawned, false if not
		bool SpawnPowerup(int x, int y);

		GameObject* SpawnObject(const std::string& type, const std::string& name, int x, int y);

		/// <summary>
		/// Resets map to original state 
		/// </summary>
		void ResetMap();

		/// <summary>
		/// Returns GameObject for a given tile
		/// </summary>
		/// <param name="position">Vec3 position</param>
		/// <returns></returns>
		GameObject* GetGameObject(const glm::vec3 position);

		virtual gsl::owner<Scope*> Clone() const override;

		void PixelConversion(int x, int y, int* tile_x, int* tile_y);

		glm::vec2 GetTileSnappedLocation(GameObject& object);
		glm::vec2 GetTileSnappedLocation(int tile_x, int tile_y);

	private:

		/// <summary>
		/// Checks to see if a powerup should be spawned at a newly destroyed tile
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <returns></returns>
		bool PowerupCheck(int x, int y) const;

		void PopulatePowerup(int numberOfPowerups, MapTiles::TileType powerupType);

		int unscaledPixelHeight = 208;

		float unscaledPixelWidth = 240.0;

		int mTileWidth = 15;
		int mTileHeight = 13;

		int mMapWidth;
		int mMapHeight;

		int mScreenWidth;
		int mScreenHeight;

		int mDestructibleWalls = 50;
		int mExtraBombs = 2;
		int mExplosionExpanders = 2;
		int mAccelerators = 2;
		int mMaximumExplosions = 2;
		int mKicks = 2;
		int mBoxingGloves = 2;
		int mTotalPowerups;
		int powerupsSpawned = 0;

		World* mWorld;

		Vector<Vector<tile>> mTiles;
	};
}

