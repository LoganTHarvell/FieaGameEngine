#pragma once

#include "Action.h"
#include "GameObject.h"
#include "Factory.h"

using namespace Library;

namespace GameDesktop
{
	class ActionSpawnBomb final : public Action
	{
		RTTI_DECLARATIONS(ActionSpawnBomb, Action)
	public:
		inline static const std::string TransformKey = "Transform";
		inline static const std::string BombCountKey = "BombCount";
		inline static const std::string BombSizeKey = "BombSize";
		inline static const std::string ObjectName = "Bomb";
		inline static const std::string ObjectType = "GameObject";

		explicit ActionSpawnBomb(const std::string& name = std::string());
		~ActionSpawnBomb() = default;
		ActionSpawnBomb(const ActionSpawnBomb&) = default;
		ActionSpawnBomb(ActionSpawnBomb&&) = default;
		ActionSpawnBomb& operator=(const ActionSpawnBomb&) = default;
		ActionSpawnBomb& operator=(ActionSpawnBomb&&) = default;

		virtual gsl::owner<ActionSpawnBomb*> Clone() const override;
		virtual void Update(WorldState& worldState) override;
		virtual std::string ToString() const override;
	};

	ConcreteFactory(ActionSpawnBomb, Scope);
}
