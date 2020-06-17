#include "pch.h"
#include "GameObject.h"
#include "WorldState.h"
#include "GameTime.h"
#include "Game.h"
#include "Datum.h"
#include "Scope.h"
#include "Character.h"

using namespace Library;

namespace GameDesktop
{
	const TypeManager::TypeInfo& GameDesktop::GameObject::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ TransformKey, Types::Scope, true, 1, 0},
				{ RenderComponentKey, Types::Scope, true, 1, 0 },
				{ VelocityKey, Types::Vector, false, 1, offsetof(GameObject, mVelocity) }
			},

			Entity::TypeIdClass()
		};

		return typeInfo;
	}

	GameDesktop::GameObject::GameObject(const std::string& name) :
		GameObject(TypeIdClass(), name)
	{
	}

	gsl::owner<GameObject*> GameDesktop::GameObject::Clone() const
	{
		return new GameObject(*this);
	}

	void GameObject::Update(WorldState& worldState)
	{
		

		Game* game = static_cast<Game*>(worldState.World->Find(Game::GameKey)->Get<RTTI*>());
		Map& map = game->GetMap();
		Move(worldState.GameTime->ElapsedGameTime(), map);
		Entity::Update(worldState);

		if (mBoundingBox == glm::vec2())
		{
			glm::vec2 scale = GetTransform().GetScale();

			mBoundingBox.x = (scale.x - 1) / 2 * map.scalingFactorX;
			mBoundingBox.y = mBoundingBox.x;

			if (scale.x != scale.y)
			{
				mBoundingOffset = mBoundingBox.y / 2;
			}
		}
	}

	void GameObject::Move(const std::chrono::milliseconds& deltaTime, Map& map)
	{
		glm::vec2 vel = GetVelocity();
		if (vel != glm::vec2())
		{
			bool blockedMove1 = false;
			bool blockedMove2 = false;

			Transform& trans = GetTransform();
			glm::vec3 loc = trans.GetLocation();

			Map::tile currentTile = map.GetTile(static_cast<int>(loc.x), static_cast<int>(loc.y));

			int currentTilePos[2];
			map.PixelConversion(static_cast<int>(loc.x), static_cast<int>(loc.y), &currentTilePos[0], &currentTilePos[1]);

			vel *= static_cast<float>(deltaTime.count() / 1000.0f);
			loc.x += vel.x;
			loc.y += vel.y;

			Map::tile tile1;
			Map::tile tile2;

			if (vel.x > 0)
			{
				tile1 = map.GetTile(static_cast<int>(loc.x + mBoundingBox.x), static_cast<int>(loc.y - mBoundingBox.y + mBoundingOffset));
				tile2 = map.GetTile(static_cast<int>(loc.x + mBoundingBox.x), static_cast<int>(loc.y + mBoundingBox.y + mBoundingOffset));
			}
			else if (vel.x < 0)
			{
				tile1 = map.GetTile(static_cast<int>(loc.x - mBoundingBox.x), static_cast<int>(loc.y - mBoundingBox.y + mBoundingOffset));
				tile2 = map.GetTile(static_cast<int>(loc.x - mBoundingBox.x), static_cast<int>(loc.y + mBoundingBox.y + mBoundingOffset));
			}
			else if (vel.y > 0)
			{
				tile1 = map.GetTile(static_cast<int>(loc.x + mBoundingBox.x), static_cast<int>(loc.y + mBoundingBox.y + mBoundingOffset));
				tile2 = map.GetTile(static_cast<int>(loc.x - mBoundingBox.x), static_cast<int>(loc.y + mBoundingBox.y + mBoundingOffset));
			}
			else if (vel.y < 0)
			{
				tile1 = map.GetTile(static_cast<int>(loc.x + mBoundingBox.x), static_cast<int>(loc.y - mBoundingBox.y + mBoundingOffset));
				tile2 = map.GetTile(static_cast<int>(loc.x - mBoundingBox.x), static_cast<int>(loc.y - mBoundingBox.y + mBoundingOffset));
			}

			if (tile1.object == nullptr || tile1.object != currentTile.object)
			{
				blockedMove1 = CheckCollision(tile1.base);
			}

			if (tile2.object == nullptr || tile2.object != currentTile.object)
			{
				blockedMove2 = CheckCollision(tile2.base);
			}

			if (!blockedMove1 && !blockedMove2)
			{
				HandleCollsion(tile1.object, tile1.base);
			}
			else if (!blockedMove1 && blockedMove2)
			{
				loc = trans.GetLocation();

				if (vel.x != 0)
				{
					loc.y -= std::abs(vel.x);
				}
				else if (vel.y != 0)
				{
					loc.x += std::abs(vel.y);
				}
			}
			else if (!blockedMove2 && blockedMove1)
			{
				loc = trans.GetLocation();

				if (vel.x != 0)
				{
					loc.y += std::abs(vel.x);
				}
				else if (vel.y != 0)
				{
					loc.x -= std::abs(vel.y);
				}
			}
			else
			{
				// Edge case because bombs block but still have actions
				if (tile1.base == MapTiles::TileType::BOMB)
				{
					HandleCollsion(tile1.object, tile1.base);
				}

				loc = trans.GetLocation();
				SetVelocity(glm::vec2(0));
			}

			int newTilePos[2];
			map.PixelConversion(static_cast<int>(loc.x), static_cast<int>(loc.y), &newTilePos[0], &newTilePos[1]);

			if (newTilePos[0] != currentTilePos[0] || newTilePos[1] != currentTilePos[1])
			{
				if (this->TypeIdInstance() != Character::TypeIdClass())
				{
					map.SetTileFromId(currentTilePos[0], currentTilePos[1], MapTiles::TileType::EMPTY, nullptr);
				}
			}

			trans.SetLocation(loc);
			map.SetTile(*this);
		}
	}

	Transform& GameObject::GetTransform()
	{
		Data& datum = (*this)[TransformKey];
		assert(datum.Size() > 0);
		Scope& scope = datum[0];
		assert(scope.Is(Transform::TypeIdClass()));

		return static_cast<Transform&>(scope);
	}

	const Transform& GameObject::GetTransform() const
	{
		return const_cast<GameObject*>(this)->GetTransform();
	}

	Transform& GameObject::SetTransform(Transform& transform)
	{
		Scope::Data* transformDatum = Find(TransformKey);

		if (transformDatum->Size() != 0)
		{
			Scope* scope = transformDatum->Get<Scope*>(0);
			Orphan(*scope);
		}

		return static_cast<Transform&>(Adopt(transform, TransformKey));
	}

	RenderComponent& GameObject::GetRenderComponent()
	{
		Data& datum = (*this)[RenderComponentKey];
		assert(datum.Size() > 0);
		Scope& scope = datum[0];
		assert(scope.Is(RenderComponent::TypeIdClass()));

		return static_cast<RenderComponent&>(scope);
	}

	const RenderComponent& GameObject::GetRenderComponent() const
	{
		return const_cast<GameObject*>(this)->GetRenderComponent();
	}

	RenderComponent& GameObject::AddRenderComponent(RenderComponent& renderComponent)
	{
		return static_cast<RenderComponent&>(Adopt(renderComponent, RenderComponentKey));
	}

	glm::vec2 GameObject::GetVelocity() const
	{
		const Data* datum = Find(VelocityKey);
		assert(datum != nullptr);
		return glm::vec2(datum->Get<glm::vec4>());
	}

	void GameObject::SetVelocity(const glm::vec2& vel)
	{
		glm::vec4& velocity = (*this)[VelocityKey].Get<glm::vec4>();
		velocity.x = vel.x;
		velocity.y = vel.y;
	}

	Quad& GameObject::GetQuad()
	{
		return mQuad;
	}

	const Quad& GameObject::GetQuad() const
	{
		return mQuad;
	}

	bool GameObject::CheckCollision(MapTiles::TileType tileType)
	{
		if (tileType == MapTiles::TileType::INDESTRUCTIBLE_WALL || tileType == MapTiles::TileType::OUTER_WALL
			|| tileType == MapTiles::TileType::DESTRUCTIBLE_WALL || tileType == MapTiles::TileType::BOMB)
		{
			return true;
		}

		return false;
	}

	void GameObject::HandleCollsion([[maybe_unused]] GameObject* object, [[maybe_unused]] MapTiles::TileType tileType)
	{
	}

	GameObject::GameObject(const RTTI::IdType typeId, const std::string& name) : 
		Entity(typeId, name), mVelocity(glm::vec4())
	{
		mQuad.Initialize();
	}
}