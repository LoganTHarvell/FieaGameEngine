#pragma once
#include "RTTI.h"
#include "HashMap.h"

namespace GameDesktop
{
	using namespace Library;

	struct MapTiles
	{
		enum class TileType : int
		{
			UNKNOWN = -1,
			EMPTY,
			INDESTRUCTIBLE_WALL,
			DESTRUCTIBLE_WALL,
			OUTER_WALL,
			PLAYER,
			NO_SPAWN,
			BOMB,
			EXPLOSION,
			EXTRA_BOMB,
			EXPLOSINON_EXPANDER,
			ACCELERATOR,
			MAXIMUM_EXPLOSION,
			KICK,
			BOXING_GLOVE
		};

		static const HashMap<RTTI::IdType, TileType> sObjectToTileType;
	};
}