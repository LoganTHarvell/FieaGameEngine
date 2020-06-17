#pragma once

#include <chrono>

#include "Action.h"
#include "GameObject.h"
#include "Factory.h"

using namespace Library;

namespace GameDesktop
{
	class ActionPlaySound final : public Action
	{
		/// <summary>
		/// Type definition for a point in time.
		/// </summary>
		using TimePoint = std::chrono::high_resolution_clock::time_point;

		/// <summary>
		/// Type definition for a duration of time.
		/// </summary>
		using Duration = std::chrono::milliseconds;

		RTTI_DECLARATIONS(ActionPlaySound, Action)
	public:
		inline static const std::string IsEnabledKey = "IsEnabled";
		inline static const std::string DelayKey = "Delay";
		inline static const std::string SoundKey = "Sound";
		static const TypeManager::TypeInfo& TypeInfo();

		explicit ActionPlaySound(const std::string& name = std::string());
		explicit ActionPlaySound(int delay, const std::string& name = std::string());
		~ActionPlaySound() = default;
		ActionPlaySound(const ActionPlaySound&) = default;
		ActionPlaySound(ActionPlaySound&&) = default;
		ActionPlaySound& operator=(const ActionPlaySound&) = default;
		ActionPlaySound& operator=(ActionPlaySound&&) = default;

		virtual gsl::owner<ActionPlaySound*> Clone() const override;
		virtual void Update(WorldState& worldState) override;
		virtual std::string ToString() const override;
	private:
		int32_t mIsEnabled = 1;
		int32_t mDelay = 0;
		TimePoint mTimeLastUpdated{ std::chrono::high_resolution_clock::now() };
		std::string mSoundName;
	};

	ConcreteFactory(ActionPlaySound, Scope)
}