#pragma once
#include "GameObject.h"
#include "IEventSubscriber.h"
#include "MenuDisplay.h"
#include "Factory.h"

namespace GameDesktop
{
	class Menu final : public GameObject, public IEventSubscriber
	{
		RTTI_DECLARATIONS(Menu, GameObject)
	public:
		inline static const std::string MenuDisplayKey = "MenuDisplay";
		inline static const std::string LevelFilenamePrefix = "Bomberman";
		inline static const std::string LevelFilenameSuffix = ".json";
		static const TypeManager::TypeInfo& TypeInfo();
	public:
		explicit Menu(const std::string& name = std::string());
		Menu(const Menu&) = default;
		Menu(Menu&&) = default;
		Menu& operator=(const Menu&) = default;
		Menu& operator=(Menu&&) = default;
		virtual ~Menu();

		virtual void Notify(EventPublisher& eventPublisher) override;
		virtual void Update(WorldState& worldState) override;
		virtual gsl::owner<Menu*> Clone() const override;
		MenuDisplay& GetMenuDisplay();


	private:
		bool mSizeSet = false;
		bool mIsInitialized = false;
		int mPlayerCount = 2;

		void StartPressed(class World* world);
		void IncrementPlayerCount();
		void DecrementPlayerCount();
	};

	ConcreteFactory(Menu, Scope)
}

