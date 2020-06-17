#pragma once
#include "IEventSubscriber.h"
#include "GameObject.h"
#include "Factory.h"
#include "Vector.h"
#include <chrono>

using namespace Library;

namespace GameDesktop
{
	class HUDNumber;
	class HUD : public GameObject
	{
		RTTI_DECLARATIONS(HUD, GameObject)
	public:
		inline static const std::string HUDNumbersKey = "HUDNumbers";
		static const TypeManager::TypeInfo& TypeInfo();

	public:
		explicit HUD(const std::string& name = std::string());
		HUD(const HUD&) = default;
		HUD(HUD&&) = default;
		HUD& operator=(const HUD&) = default;
		HUD& operator=(HUD&&) = default;
		virtual ~HUD() = default;

		virtual void Update(WorldState& worldState) override;
		virtual gsl::owner<HUD*> Clone() const override;
		
		HUDNumber& GetHUDNumber(size_t index);
		size_t HUDNumbersSize();
		void Initialize(WorldState& worldState);

	private:
		unsigned int mSecondsLeft;
		bool mInitialized = false;
		void UpdateNumbers();
		const float mGridSize = 16.0;
	};
	ConcreteFactory(HUD, Scope)
}