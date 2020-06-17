#pragma once

#include <chrono>

#include "Action.h"
#include "GameObject.h"
#include "Factory.h"

using namespace Library;

namespace GameDesktop
{
	class ActionExpireEntity final : public Action
	{
		RTTI_DECLARATIONS(ActionExpireEntity, Action)
	public:
		inline static const std::string ExpireTimeKey = "ExpireTime";
		static const TypeManager::TypeInfo& TypeInfo();

		explicit ActionExpireEntity(const std::string& name = std::string());
		explicit ActionExpireEntity(int delay, const std::string& name = std::string());
		~ActionExpireEntity() = default;
		ActionExpireEntity(const ActionExpireEntity&) = default;
		ActionExpireEntity(ActionExpireEntity&&) = default;
		ActionExpireEntity& operator=(const ActionExpireEntity&) = default;
		ActionExpireEntity& operator=(ActionExpireEntity&&) = default;

		virtual gsl::owner<ActionExpireEntity*> Clone() const override;
		virtual void Update(WorldState& worldState) override;
		virtual std::string ToString() const override;
	private:
		int32_t mExpireTime = 1; // miliseconds 
		int32_t mElapsedTime = 0; // miliseconds
	};

	ConcreteFactory(ActionExpireEntity, Scope);
}