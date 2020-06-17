#pragma once
#include "Entity.h"
#include "Factory.h"
#include "Quad.h"
#include "Transform.h"
#include "Texture.h"
#include "RenderManager.h"
#include "Shader.h"
#include "RenderComponent.h"
#include "Map.h"
#include "MapTiles.h"

#include <chrono>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include <glm/glm.hpp>
using namespace Library;

namespace GameDesktop
{
	class GameObject : public Entity
	{
		RTTI_DECLARATIONS(GameObject, Entity)
	public:
		inline static const std::string TransformKey = "Transform";
		inline static const std::string RenderComponentKey = "RenderComponent";
		inline static const std::string VelocityKey = "Velocity";

		static const TypeManager::TypeInfo& TypeInfo();

	public:
		explicit GameObject(const std::string& name = std::string());
		GameObject(const GameObject&) = default;
		GameObject(GameObject&&) = default;
		GameObject& operator=(const GameObject&) = default;
		GameObject& operator=(GameObject&&) = default;
		virtual ~GameObject() = default;

		virtual gsl::owner<GameObject*> Clone() const override;

		void Update(WorldState& worldState) override;

		void Move(const std::chrono::milliseconds& deltaTime, Map& map);

		Transform& GetTransform();
		const Transform& GetTransform() const;

		glm::vec2 GetVelocity() const;
		void SetVelocity(const glm::vec2& vel);
		
		Transform& SetTransform(Transform& transform);

		RenderComponent& GetRenderComponent();
		const RenderComponent& GetRenderComponent() const;
		RenderComponent& AddRenderComponent(RenderComponent& renderComponent);

		Quad& GetQuad();
		const Quad& GetQuad() const;

		bool CheckCollision(MapTiles::TileType tileType = MapTiles::TileType::UNKNOWN);

		virtual void HandleCollsion(GameObject* object = nullptr, MapTiles::TileType tileType = MapTiles::TileType::UNKNOWN);

	protected:
		glm::vec4 mVelocity;
		glm::vec2 mBoundingBox;
		float mBoundingOffset;

		Quad mQuad;

		GameObject(const RTTI::IdType typeId, const std::string& name);
	};

	ConcreteFactory(GameObject, Scope)
}
