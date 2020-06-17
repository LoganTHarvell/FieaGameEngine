#include "pch.h"
#include "Character.h"
#include "Game.h"
#include "Bomb.h"
#include "ActionExpireEntity.h"
#include "AudioManager.h"
#include "ActionUpdateAnimationState.h"
#include "ActionAnimate.h"
#include "GameMessage.h"
#include "Event.h"

namespace GameDesktop
{
	using namespace Library;

	HashMap<Character::EPowerUp, std::function<void(Character&)>> Character::sPowerEffects =
	{
		{EPowerUp::ExtraBomb, [&](Character& c) {c.mBombCount = std::min(c.mBombCount + 1, c.mMaxBombCapacity); }},
		{EPowerUp::ExplosionExpansion, [&](Character& c) {c.mExplosionDistance = std::min(c.mExplosionDistance + 1, c.mMaxExplosionDistance); }},
		{EPowerUp::Accelerator, [&](Character& c) {c.mSpeed = std::min(c.mSpeed + c.mSpeedBoostAmount, c.mMaxSpeed); }},
		{EPowerUp::RemoteControl, [&](Character&) {/* blank */}},
		{EPowerUp::MaxExplosionExpansion, [&](Character& c) { c.mExplosionDistance = c.mMaxExplosionDistance; }},
		{EPowerUp::Kick, [&](Character& c) { c.mCanKick = true; }},
		{EPowerUp::BoxingGlove, [&](Character& c) {c.mCanPunch = true; }},
		{EPowerUp::Skull, [&](Character&) {/* blank */}}
	};

	HashMap<Character::EPowerUp, std::function<void(Character&)>> Character::sInvertedPowerEffects =
	{
		{EPowerUp::ExtraBomb, [&](Character& c) {c.mBombCount = std::max(c.mBombCount - 1, 1); }},
		{EPowerUp::ExplosionExpansion, [&](Character& c) { c.mExplosionDistance = std::max(c.mExplosionDistance - 1, 2); }},
		{EPowerUp::Accelerator, [&](Character& c) {c.mSpeed = std::max(c.mSpeed - c.mSpeedBoostAmount, c.mBaseSpeed);}},
		{EPowerUp::RemoteControl, [&](Character&) {/* blank */}},
		{EPowerUp::MaxExplosionExpansion, [&](Character& c) {c.mExplosionDistance = 2; }},
		{EPowerUp::Kick, [&](Character& c) { c.mCanKick = false; }},
		{EPowerUp::BoxingGlove, [&](Character& c) { c.mCanPunch = false; }},
		{EPowerUp::Skull, [&](Character&) {/* blank */}}
	};

	Character::Character(const std::string& name)
		: GameObject(TypeIdClass(), name)
	{
	}

	void Character::Update(WorldState& worldState)
	{
		if (!mIsDying)
		{
			if (mSpawnTile != glm::vec4(0))
			{
				Game* game = static_cast<Game*>(worldState.World->Find(Game::GameKey)->Get<RTTI*>());
				Map& map = game->GetMap();
				GetTransform().SetLocation(map.GetTileSnappedLocation(static_cast<int>(mSpawnTile.x), static_cast<int>(mSpawnTile.y)));
				mSpawnTile = glm::vec4(0);
			}

			Game* game = static_cast<Game*>(worldState.World->Find(Game::GameKey)->Get<RTTI*>());
			Map& map = game->GetMap();
			int gameObjectX = static_cast<int>(GetTransform().GetLocation()[0]);
			int gameObjectY = static_cast<int>(GetTransform().GetLocation()[1]);
			MapTiles::TileType tileType = map.GetTile(gameObjectX, gameObjectY).base;

			if (tileType == MapTiles::TileType::EXPLOSION)
			{
				Die(worldState);
			}

			GameObject::Update(worldState);
		}
		else
		{
			Entity::Update(worldState);
		}
	}

	gsl::owner<GameObject*> Character::Clone() const
	{
		return new Character(*this);
	}

	void Character::PickupPower(EPowerUp power)
	{
		sPowerEffects[power](*this);
		mHeldPowerUps.PushBack(power);
	}

	void Character::DropInventory()
	{
		for (auto& power : mHeldPowerUps)
			sInvertedPowerEffects[power](*this);
		
		mHeldPowerUps.Clear();
	}

	void Character::Move(const glm::vec4& input, [[maybe_unused]] std::chrono::milliseconds deltaTime)
	{
		glm::vec2 newVel(input * mSpeed);
		SetVelocity(newVel);
	}

	void Character::HandleCollsion(GameObject* object, MapTiles::TileType tileType)
	{
		if (tileType == MapTiles::TileType::BOMB)
		{
			if (mCanKick)
			{
				glm::vec2 dir(GetVelocity());
				dir = glm::normalize(dir);
				mKickedBombRef = static_cast<Bomb*>(object);
				mKickedBombRef->Kick(dir);
			}
		}
		else if (tileType == MapTiles::TileType::ACCELERATOR || tileType == MapTiles::TileType::EXTRA_BOMB
			|| tileType == MapTiles::TileType::EXPLOSINON_EXPANDER || tileType == MapTiles::TileType::MAXIMUM_EXPLOSION
			|| tileType == MapTiles::TileType::KICK || tileType == MapTiles::TileType::BOXING_GLOVE)
		{
			if (object != nullptr)
			{
				PickupPower(PowerUp::TileTypeToEPowerUp(tileType));
				object->Adopt(*(new ActionExpireEntity(0)), "Actions");
			}
		}
	}

	void Character::SetInputVelocity(const glm::vec2& input)
	{
		GameObject::SetVelocity(input * mSpeed);
	}

	Bomb* Character::GetKickedBombRef()
	{
		return mKickedBombRef;
	}

	const Bomb* Character::GetKickedBombRef() const
	{
		return mKickedBombRef;
	}

	Bomb* Character::SetBombRef(Bomb* bomb)
	{
		return mKickedBombRef = bomb;
	}

	void Character::SetSpeed(float speed)
	{
		mSpeed = speed;
	}

	float Character::GetSpeed() const
	{
		return mSpeed;
	}

	void Character::AddBombRef(Bomb& bomb)
	{
		mBombs.EmplaceBack(&bomb);
	}

	void Character::RemoveBombRef(Bomb& bomb)
	{
		mBombs.Remove(&bomb);
	}

	const TypeManager::TypeInfo& Character::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ SpawnTileKey, Types::Vector, false, 1, offsetof(Character, mSpawnTile) },
				{ BombCountKey, Types::Integer, false, 1, offsetof(Character, mBombCount) },
				{ BombCapacityKey, Types::Integer, false, 1, offsetof(Character, mBombCapacity) },
				{ BombResetTimerKey, Types::Float, false, 1, offsetof(Character, mBombResetTimer) },
				{ ExplosionDistanceKey, Types::Integer, false, 1, offsetof(Character, mExplosionDistance) },
				{ SpeedBoostAmountKey, Types::Float, false, 1, offsetof(Character, mSpeedBoostAmount) },
				{ CanKickKey, Types::Integer, false, 1, offsetof(Character, mCanKick) },
				{ CanPunchKey, Types::Integer, false, 1, offsetof(Character, mCanPunch) },
				{ SpeedKey, Types::Float, false, 1, offsetof(Character, mSpeed)},
				{ DeathSpriteKey, Types::String, false, 1, offsetof(Character, mDeathSprite)},
			},

			GameObject::TypeIdClass()
		};

		return typeInfo;
	}
	std::string Character::ToString()
	{
		std::ostringstream output;
		output << Name() << "\n";
		output << GetVelocity().x << ", " << GetVelocity().y << "\n";
		output << GetTransform().ToString() << "\n";

		return output.str();
	}

	void Character::Die(WorldState& worldState)
	{
		if (!mIsDying)
		{
			for (Bomb* bomb : mBombs)
			{
				bomb->SetCharacterRef(nullptr);
			}

			AudioManager::Instance()->PlaySound("Death");

			Adopt(*(new ActionExpireEntity(mDeathDelay)), "Actions");
			RenderComponent& renderComp = GetRenderComponent();
			renderComp.SetCurrentSprite(mDeathSprite);
			renderComp.SetColumnCount(6);
			renderComp.SetRowCount(1);
			renderComp.SetColumnIndex(0);
			renderComp.SetRowIndex(0);

			Data& datum = (*this)[ActionsKey];
			assert(datum.Size() > 0);

			for (size_t i = 0; i < datum.Size(); ++i)
			{
				Scope& scope = datum[i];
				assert(scope.Is(Action::TypeIdClass()));

				if (scope.Is(ActionAnimate::TypeIdClass()))
				{
					ActionAnimate* animation = static_cast<ActionAnimate*>(&scope);

					animation->Reset(worldState);
					animation->SetDelay(mDeathAnimationDelay);
				}
			}

			--sCharacterCount;

			if (sCharacterCount == 1)
			{
				GameMessage message(Game::MenuFilename, StateManager::GameStates::Menu);
				const auto event = std::make_shared<Event<GameMessage>>(message);
				worldState.EventQueue->Enqueue(event, worldState.GameTime->CurrentTime() + std::chrono::milliseconds(mDeathAnimationDelay * renderComp.GetColumnCount()));
			}

			mIsDying = true;
		}
	}

	bool Character::IsDying()
	{
		return mIsDying;
	}
}