#include "pch.h"
#include "MapTiles.h"
#include "Bomb.h"
#include "DestructibleWall.h"
#include "Explosion.h"

namespace GameDesktop
{
	const HashMap<RTTI::IdType, MapTiles::TileType> MapTiles::sObjectToTileType =
	{
		{Bomb::TypeIdClass(), TileType::BOMB},
		{DestructibleWall::TypeIdClass(), TileType::DESTRUCTIBLE_WALL},
		{Explosion::TypeIdClass(), TileType::EXPLOSION}
	};
}