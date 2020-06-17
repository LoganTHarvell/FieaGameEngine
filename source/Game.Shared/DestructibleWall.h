#pragma once
#include "GameObject.h"
#include "GameTime.h"
#include <chrono>

#include "Factory.h"

namespace GameDesktop
{
	class DestructibleWall final : public GameObject
	{
		RTTI_DECLARATIONS(DestructibleWall, GameObject)

	public:
		inline static const std::string SizeKey = "Size";

		explicit DestructibleWall(std::string name = std::string());
		DestructibleWall(const DestructibleWall&) = default;
		DestructibleWall(DestructibleWall&&) = default;
		DestructibleWall& operator=(const DestructibleWall&) = default;
		DestructibleWall& operator=(DestructibleWall&&) = default;
		virtual ~DestructibleWall() = default;

		void Update(Library::WorldState & worldState) override;

		virtual gsl::owner<GameObject*> Clone() const override;

		static const Library::TypeManager::TypeInfo& TypeInfo();

		void CheckPlayerCollision(WorldState& worldState); //HACK: remove after testing

		void Die();
	private:
		int mSize = 16;
		bool mIsDying = false;


		inline static const std::string CurrentSprite = "DestructibleWall";
		inline static const int32_t ColumnCount = 7;
		inline static const int32_t RowCount = 1;
		inline static const int32_t StartColumnIndex = 0;
		inline static const int32_t StartRowIndex = 0;

		int elapsedTime = 0;
		inline static const int mAnimationFrameDelay = 100; // miliseconds
		inline static const int mAnimationFrameCount = 6;
		inline static const int mDieTime = mAnimationFrameDelay * mAnimationFrameCount;
	};

	ConcreteFactory(DestructibleWall, Scope);
}

