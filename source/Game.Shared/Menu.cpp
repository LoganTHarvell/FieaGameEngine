#include "pch.h"
#include "Menu.h"

#include "Sector.h"
#include "World.h"
#include "Game.h"
#include "Transform.h"
#include "Event.h"
#include "GameMessage.h"
#include "MenuInputMessage.h"
#include "Map.h"
#include "Character.h"

#include "AudioManager.h"

namespace GameDesktop
{
	const TypeManager::TypeInfo& Menu::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ MenuDisplayKey, Types::Scope, true, 1, 0},
			},

			GameObject::TypeIdClass()
		};

		return typeInfo;
	}
	Menu::Menu(const std::string& name) :
		GameObject(TypeIdClass(), name)
	{
		Event<MenuInputMessage>::Subscribe(*this);
		AudioManager::Instance()->StopAllSound();
		AudioManager::Instance()->PlayBackgroundMusic("MenuMusic");
	}
	Menu::~Menu()
	{
		Event<MenuInputMessage>::Unsubscribe(*this);
	}
	void Menu::Notify(EventPublisher& eventPublisher)
	{
		assert(eventPublisher.Is(Event<MenuInputMessage>::TypeIdClass()));
		Event<MenuInputMessage>& event = static_cast<Event<MenuInputMessage>&>(eventPublisher);
		auto& message = event.Message;

		switch (message.GetType())
		{
		case MenuInputMessage::MessageType::Up:
			DecrementPlayerCount();
			break;
		case MenuInputMessage::MessageType::Down:
			IncrementPlayerCount();
			break;
		case MenuInputMessage::MessageType::Enter:
			StartPressed(event.Message.GetWorld());
			break;
		}
	}
	void Menu::Update(WorldState& worldState)
	{
		if(!mSizeSet)
		{
			RTTI* rtti = worldState.World->operator[](Game::GameKey).Get<RTTI*>();
			assert(rtti->Is(Game::TypeIdClass()));
			Game* game = static_cast<Game*>(rtti);

			GetTransform().SetLocation(glm::vec2(game->GetWindowManager().Width() / 2.0f, game->GetWindowManager().Height() / 2.0f));
			GetTransform().SetScale(glm::vec2(game->GetWindowManager().Width() / Map::scalingFactorX, game->GetWindowManager().Height() / Map::scalingFactorY));

			GetMenuDisplay().GetTransform().SetLocation(glm::vec2(game->GetWindowManager().Width() / 2.0f, game->GetWindowManager().Height() * 0.7f));
			// Not setting the scale for menuDisplay
			mSizeSet = true;
		}
		GameObject::Update(worldState);
		GetMenuDisplay().Update(worldState);
	}
	gsl::owner<Menu*> Menu::Clone() const
	{
		return new Menu(*this);
	}
	void Menu::IncrementPlayerCount()
	{
		if (!mIsInitialized)
		{
			return;
		}
		if (mPlayerCount == 4)
		{
			mPlayerCount = 2;
		}
		else
		{
			++mPlayerCount;
		}
		GetMenuDisplay().GetRenderComponent().SetRowIndex(static_cast<int32_t>(mPlayerCount - 1));
		AudioManager::Instance()->PlaySound("MenuInput");
	}
	void Menu::DecrementPlayerCount()
	{
		if (!mIsInitialized)
		{
			return;
		}
		if (mPlayerCount == 2)
		{
			mPlayerCount = 4;
		}
		else
		{
			--mPlayerCount;
		}
		
		GetMenuDisplay().GetRenderComponent().SetRowIndex(static_cast<int32_t>(mPlayerCount - 1));
		AudioManager::Instance()->PlaySound("MenuInput");
	}
	
	MenuDisplay& Menu::GetMenuDisplay()
	{
		Data& datum = (*this)[MenuDisplayKey];
		assert(datum.Size() > 0);
		Scope& scope = datum[0];
		assert(scope.Is(MenuDisplay::TypeIdClass()));

		return static_cast<MenuDisplay&>(scope);
	}

	void Menu::StartPressed(World* world)
	{
		if (!mIsInitialized)
		{
			GetMenuDisplay().GetRenderComponent().SetRowIndex(1);
			mIsInitialized = true;
			AudioManager::Instance()->PlaySound("MenuSelect");
		}
		else
		{
			Character::sCharacterCount = mPlayerCount;
			const std::string filename = LevelFilenamePrefix + std::to_string(mPlayerCount) + LevelFilenameSuffix;
			GameMessage message(filename, static_cast<StateManager::GameStates>(mPlayerCount-1));
			const auto event = std::make_shared<Event<GameMessage>>(message);
			world->GetWorldState().EventQueue->Enqueue(event);
		}
	}
}
