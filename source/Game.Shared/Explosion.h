#pragma once
#include "GameObject.h"
#include "GameTime.h"
#include <chrono>

#include "Factory.h"

namespace GameDesktop
{
	class Explosion final : public GameObject
	{
		RTTI_DECLARATIONS(Explosion, GameObject)

	public:
		inline static const std::string SizeKey = "Size";

		enum class EExplosionChunk : int
		{
			Unknown = -1,
			Center = 0,
			VerticalUpMiddle = 1,
			VerticalUpTop = 2,
			VerticalDownMiddle = 5,
			VerticalDownBot = 6,
			HorizontalLeftMiddle = 3,
			HorizontalLeftEnd = 4,
			HorizontalRightMiddle = 7,
			HorizontalRightEnd = 8,
			End
		};

		static int ExplosionChunkToInt(EExplosionChunk chunk);

		explicit Explosion(std::string name = std::string());
		Explosion(const Explosion&) = default;
		Explosion(Explosion&&) = default;
		Explosion& operator=(const Explosion&) = default;
		Explosion& operator=(Explosion&&) = default;
		virtual ~Explosion() = default;

		void Update(Library::WorldState & worldState) override;

		void SetExplosionChunk(EExplosionChunk explosionChunk);

		virtual gsl::owner<GameObject*> Clone() const override;

		static const Library::TypeManager::TypeInfo& TypeInfo();

	private:
		int mSize = 16;
		EExplosionChunk mExplosionChunk = EExplosionChunk::Unknown;

		void Die();

		inline static const std::string CurrentSprite = "Explosion";
		inline static const int32_t ColumnCount = 9;
		inline static const int32_t RowCount = 9;
		inline static const int32_t StartColumnIndex = 0;
		inline static const int32_t StartRowIndex = 0;

		int elapsedTime = 0;
		inline static const int mAnimationFrameDelay = 100; // miliseconds
		inline static const int mAnimationFrameCount = 9;
		inline static const int mDieTime = mAnimationFrameDelay * mAnimationFrameCount;
	};

	ConcreteFactory(Explosion, Scope);
}

