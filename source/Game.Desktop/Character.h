#pragma once
#include "Entity.h"
#include "Vector.h"

namespace Game
{
	enum class EPowerUp : int
	{
		Unknown = 0,
		ExplosionExpansion,
		Accelerator,
		RemoteControl, // Not used
		MaxExplosionExpansion,
		Kick, 
		BoxingGlove, // Not used
		Skull, // Not used
		End
	};

	class Character : Library::Entity
	{
	public:
		// The Register Method
		// The Update Method
		// Derive from GameObect, or have a gameobject instead
		// 

		void PickupPower(EPowerUp power)
		{
			mHeldPowerUps.PushBack(power);
		}

		// Returns the current player inventory so it can be dispersed amoung nearby tiles
		Library::Vector<EPowerUp> DropInventory()
		{
			mHeldPowerUps.Clear();
		}

	private:
		int mID = -1;

		// Transform
		// SpriteSheet

		int mBombCount = 1;						// The current bomb count
		int mBombCapacity = 1;					// The current number of bombs that can be carried
		const int mMaxBombCapacity = 5;			// The max number of bombs that can be carried (Upper Limit)
		float mBombResetTimer = 1.0f;			// Number of seconds before getting another bomb

		int mExplosionDistance = 1;
		const int mMaxExplosionDistance = 5;

		float mSpeed = 1.0f;					// Base speed
		float mSpeedBoostAmount = .5f;			// Amount speed powerup increases base speed
		const float mMaxSpeed = 5.0f;			// Upper speed limit

		bool mCanKick = false;
		bool mCanPunch = false;

		Library::Vector<EPowerUp> mHeldPowerUps;
	};
}