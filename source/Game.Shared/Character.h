#pragma once
#include "Entity.h"
#include "Vector.h"
#include "HashMap.h"

#include "Attributed.h"
#include "WorldState.h"
#include "Factory.h"

#include "GameObject.h"
#include "PowerUp.h"
#include "Bomb.h"

#include <glm/glm.hpp>

#include <functional>
#include <chrono>

namespace GameDesktop
{
	class Character : public GameObject
	{
		RTTI_DECLARATIONS(Character, GameObject);

	public:
		//Usings
		using EPowerUp = PowerUp::EPowerUp;

		//Keys
		inline static const std::string SpawnTileKey = "SpawnTile";
		inline static const std::string BombCountKey = "BombCount";
		inline static const std::string BombCapacityKey = "BombCapacity";
		inline static const std::string BombResetTimerKey = "BombResetTimer";
		inline static const std::string ExplosionDistanceKey = "ExplosionDistance";
		inline static const std::string SpeedBoostAmountKey = "SpeedBoostAmount";
		inline static const std::string CanKickKey = "CanKick";
		inline static const std::string CanPunchKey = "CanPunch";
		inline static const std::string SpeedKey = "Speed";
		inline static const std::string DeathSpriteKey = "DeathSprite";

		explicit Character(const std::string& name = std::string());
		Character(const Character&) = default;
		Character(Character&&) = default;
		Character& operator=(const Character&) = default;
		Character& operator=(Character&&) = default;
		virtual ~Character() = default;

		void Update(Library::WorldState& worldState) override;

		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the Entity.</returns>
		virtual gsl::owner<GameObject*> Clone() const override;

		/// <summary>
		/// Adds a powerup to the player inventory and runs the associated powerup function.
		/// </summary>
		/// <param name="power"></param>
		void PickupPower(EPowerUp power);

		/// <summary>
		/// Clears the player's power up inventory
		/// </summary>
		void DropInventory();

		void Move(const glm::vec4& input, std::chrono::milliseconds deltaTime = std::chrono::milliseconds(0));

		void HandleCollsion(GameObject* object = nullptr, MapTiles::TileType tileType = MapTiles::TileType::UNKNOWN) override;

		void SetInputVelocity(const glm::vec2& input);

		Bomb* GetKickedBombRef();
		const Bomb* GetKickedBombRef() const;

		Bomb* SetBombRef(Bomb* bomb);

		void SetSpeed(float speed);
		float GetSpeed() const;

		void AddBombRef(Bomb& bomb);
		void RemoveBombRef(Bomb& bomb);

		static const Library::TypeManager::TypeInfo& TypeInfo();

		std::string ToString();

		void Die(WorldState& worldState);

		bool IsDying();
		inline static int32_t sCharacterCount = 0;

	private:
		int mID = -1;

		int mBombCount = 1;								// The current bomb count
		int mBombCapacity = 1;							// The current number of bombs that can be carried
		inline static const int mMaxBombCapacity = 5;	// The max number of bombs that can be carried (Upper Limit)
		float mBombResetTimer = 1.0f;					// Number of seconds before getting another bomb

		int mExplosionDistance = 2;
		inline static const int mMaxExplosionDistance = 5;

		inline static const float mBaseSpeed = 200.0f;		// Base speed
		float mSpeed = mBaseSpeed;
		float mSpeedBoostAmount = 100.0f;					// Amount speed powerup increases base speed
		inline static const float mMaxSpeed = 600.0f;		// Upper speed limit

		inline static const int mDeathAnimRows = 1;
		inline static const int mDeathAnimCols = 6;
		inline static const int mDeathAnimDelay = 200;


		bool mCanKick = false;
		bool mCanPunch = false;


		inline static int mDeathAnimationFrames = 6;
		inline static int mDeathAnimationDelay = 200;
		inline static int mDeathDelay = mDeathAnimationFrames * mDeathAnimationDelay - 100;
		std::string mDeathSprite;

		bool mIsDying = false;

		Bomb* mKickedBombRef = nullptr;

		Library::Vector<Bomb*> mBombs;

		glm::vec4 mSpawnTile;

		Library::Vector<EPowerUp> mHeldPowerUps;
	
		static Library::HashMap<EPowerUp, std::function<void(Character&)>> sPowerEffects;
		static Library::HashMap<EPowerUp, std::function<void(Character&)>> sInvertedPowerEffects;
	};

	ConcreteFactory(Character, Library::Scope)
}