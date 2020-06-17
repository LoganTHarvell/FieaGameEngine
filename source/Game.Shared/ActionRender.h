#pragma once
#include "Action.h"
#include "GameObject.h"
#include "Factory.h"
#include "Game.h"

using namespace Library;

namespace GameDesktop
{
	class ActionRender final : public Action
	{
		RTTI_DECLARATIONS(ActionRender, Action)
	public:
		inline static const std::string IsEnabledKey = "IsEnabled";
		inline static const std::string PriorityKey = "Priority";
		static const TypeManager::TypeInfo& TypeInfo();

		explicit ActionRender(const std::string& name = std::string(), int32_t priority = 0);
		~ActionRender() = default;
		ActionRender(const ActionRender&) = default;
		ActionRender(ActionRender&&) = default;
		ActionRender& operator=(const ActionRender&) = default;
		ActionRender& operator=(ActionRender&&) = default;

		virtual gsl::owner<ActionRender*> Clone() const override;
		virtual void Update(WorldState& worldState) override;
		virtual std::string ToString() const override;

		void SetEnabled(int32_t input) { mIsEnabled = input; }
		void SetPriority(int32_t input) { mPriority = input; }

	private:
		int32_t mIsEnabled = 1;
		int32_t mPriority = 0;
	};

	ConcreteFactory(ActionRender, Scope)
}
