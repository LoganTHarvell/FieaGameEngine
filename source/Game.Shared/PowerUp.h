#pragma once
#include "Entity.h"
#include "Vector.h"
#include "HashMap.h"

#include "Attributed.h"
#include "WorldState.h"
#include "Factory.h"

#include "GameObject.h"
#include "MapTiles.h"

namespace GameDesktop
{
	class PowerUp final : public GameObject
	{
		RTTI_DECLARATIONS(PowerUp, GameObject);

	public:
		enum class EPowerUp : int
		{
			Unknown = 0,
			ExtraBomb,
			ExplosionExpansion,
			Accelerator,
			RemoteControl, // Not used
			MaxExplosionExpansion,
			Kick,
			BoxingGlove, // Not used
			Skull, // Not used
			End
		};

		static Library::HashMap<std::string, EPowerUp> sStringToPowerUp;
		static EPowerUp StringToEPowerUp(const std::string& input);

		static Library::HashMap<EPowerUp, std::string> sPowerUpToString;
		static std::string EPowerUpToString(EPowerUp input);

		static Library::HashMap<MapTiles::TileType, EPowerUp> sTileToPowerUp;
		static EPowerUp TileTypeToEPowerUp(const MapTiles::TileType& input);
		
		struct RenderInfo
		{
			std::string CurrentSprite = "PowerUps";
			static const int32_t ColumnCount = 2;
			static const int32_t RowCount = 6;
			int32_t ColumnIndex = 0;
			int32_t RowIndex = 0;

			RenderInfo() {};
			RenderInfo(int32_t column, int32_t row)
			{
				ColumnIndex = column;
				RowIndex = row;
			};
		};
		static Library::HashMap<EPowerUp, RenderInfo> sPowerUpToRender;

		inline static const int Size = 16;
		
		// Message Keys
		inline static const std::string CharacterNameKey = "CharacterName";
		inline static const std::string PowerUpKey = "PowerUp";

		// Personal Keys
		inline static const std::string PowerUpTypeKey = "PowerUpType";

		void SetPowerUpType(EPowerUp powerUp);
		void SetPowerUpType(MapTiles::TileType tile);

		explicit PowerUp(const std::string& name = std::string());
		PowerUp(const PowerUp&) = default;
		PowerUp(PowerUp&&) = default;
		PowerUp& operator=(const PowerUp&) = default;
		PowerUp& operator=(PowerUp&&) = default;
		virtual ~PowerUp() = default;

		void Update(Library::WorldState& worldState) override;

		void Die();

		virtual gsl::owner<GameObject*> Clone() const override;

		static const Library::TypeManager::TypeInfo& TypeInfo();

	private:
		EPowerUp mPowerUpType = EPowerUp::Unknown;
		std::string mPowerUpString{ "Unknown" };
		bool mFirstFrame = true;
	};

	ConcreteFactory(PowerUp, Library::Scope)
}