#pragma once

#include <chrono>

#include "Action.h"
#include "GameObject.h"
#include "Factory.h"

using namespace Library;

namespace GameDesktop
{
	class ActionUpdateAnimationState final : public Action
	{
		/// <summary>
		/// Type definition for a point in time.
		/// </summary>
		using TimePoint = std::chrono::high_resolution_clock::time_point;

		/// <summary>
		/// Type definition for a duration of time.
		/// </summary>
		using Duration = std::chrono::milliseconds;

		RTTI_DECLARATIONS(ActionUpdateAnimationState, Action)
	public:
		inline static const std::string IsEnabledKey = "IsEnabled";
		static const TypeManager::TypeInfo& TypeInfo();

		explicit ActionUpdateAnimationState(const std::string& name = std::string());
		~ActionUpdateAnimationState() = default;
		ActionUpdateAnimationState(const ActionUpdateAnimationState&) = default;
		ActionUpdateAnimationState(ActionUpdateAnimationState&&) = default;
		ActionUpdateAnimationState& operator=(const ActionUpdateAnimationState&) = default;
		ActionUpdateAnimationState& operator=(ActionUpdateAnimationState&&) = default;

		virtual gsl::owner<ActionUpdateAnimationState*> Clone() const override;
		virtual void Update(WorldState& worldState) override;
		virtual std::string ToString() const override;
	private:
		int32_t mIsEnabled = 1;

		int mLastDirection{ 6 };
	};

	ConcreteFactory(ActionUpdateAnimationState, Scope);
}