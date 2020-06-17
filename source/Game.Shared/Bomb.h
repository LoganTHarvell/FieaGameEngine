#pragma once
#include "GameObject.h"
#include "GameTime.h"
#include <chrono>

namespace GameDesktop
{
	using namespace Library;
	class Character;

	class Bomb final : public GameObject
	{
		RTTI_DECLARATIONS_ABSTRACT(Bomb, GameObject)

	public:
		inline static const float KickSpeed = 600.0f;
		inline static const int Size = 16;
		inline static const std::string SpriteKey = "Bomb";
		inline static const std::string DelayKey = "Delay";
		inline static const std::string ExplosionDistanceKey = "ExplosionDistance";
		inline static const std::string IsPunchedKey = "IsPunchedKey";

		static const Library::TypeManager::TypeInfo& TypeInfo();

		explicit Bomb(const GameTime& gameTime, Character& character, const std::string& name = std::string());
		Bomb(const Bomb&) = default;
		Bomb(Bomb&&) = default;
		Bomb& operator=(const Bomb&) = default;
		Bomb& operator=(Bomb&&) = default;
		virtual ~Bomb() = default;

		void Update(Library::WorldState & worldState) override;

		void SetExplosionDistance(int distance);

		void Kick(glm::vec2& dir);
		void SpawnExplosions(WorldState& state);

		void SetCharacterRef(Character* character);

		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the Bomb.</returns>
		virtual gsl::owner<GameObject*> Clone() const override;

	private:
		void Explode(WorldState& state);

		int mDelay = 3000;
		std::chrono::steady_clock::time_point mExpiredTime;
		int mExplosionDistance = 2;
		bool mIsPunched = false;
		Character* mCharacterRef;
	};
}

