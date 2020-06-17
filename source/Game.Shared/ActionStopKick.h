#pragma once
#include "Action.h"
#include "Factory.h"

namespace GameDesktop
{
	class ActionStopKick final : public Library::Action
	{
		RTTI_DECLARATIONS(ActionStopKick, Library::Action);

	public:
		explicit ActionStopKick(const std::string& name = std::string());
		~ActionStopKick() = default;
		ActionStopKick(const ActionStopKick&) = default;
		ActionStopKick(ActionStopKick&&) = default;
		ActionStopKick& operator=(const ActionStopKick&) = default;
		ActionStopKick& operator=(ActionStopKick&&) = default;

		virtual gsl::owner<ActionStopKick*> Clone() const override;
		virtual void Update(Library::WorldState& worldState) override;
		virtual std::string ToString() const override;
	};

	ConcreteFactory(ActionStopKick, Library::Scope);
}

