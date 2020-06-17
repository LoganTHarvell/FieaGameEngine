#pragma once

#include <chrono>

#include "Action.h"
#include "GameObject.h"
#include "Factory.h"

using namespace Library;

namespace GameDesktop
{
	class ActionAnimate final : public Action
	{
		/// <summary>
		/// Type definition for a point in time.
		/// </summary>
		using TimePoint = std::chrono::high_resolution_clock::time_point;

		/// <summary>
		/// Type definition for a duration of time.
		/// </summary>
		using Duration = std::chrono::milliseconds;

		RTTI_DECLARATIONS(ActionAnimate, Action)
	public:
		inline static const std::string IsEnabledKey = "IsEnabled";
		inline static const std::string DelayKey = "Delay";
		static const TypeManager::TypeInfo& TypeInfo();

		explicit ActionAnimate(const std::string& name = std::string());
		explicit ActionAnimate(int delay, const std::string& name = std::string());
		~ActionAnimate() = default;
		ActionAnimate(const ActionAnimate&) = default;
		ActionAnimate(ActionAnimate&&) = default;
		ActionAnimate& operator=(const ActionAnimate&) = default;
		ActionAnimate& operator=(ActionAnimate&&) = default;

		virtual gsl::owner<ActionAnimate*> Clone() const override;
		virtual void Update(WorldState& worldState) override;
		virtual std::string ToString() const override;

		void Reset(WorldState& worldState);
		void SetDelay(int newDelay);
	private:
		int32_t mIsEnabled = 1;
		int32_t mDelay = 0;
		TimePoint mTimeLastUpdated{ std::chrono::high_resolution_clock::now() };
	};

	ConcreteFactory(ActionAnimate, Scope);
}