#pragma once

#pragma region Includes
// Library
#include "RTTI.h"
#include "IEventSubscriber.h"
#include "World.h"
#include "EventQueue.h"
#include "Map.h"

// Game
#include "WindowManager.h"
#include "InputManager.h"
#include "StateManager.h"
#include "RenderQueue.h"
#include "AIManager.h"

#pragma endregion Includes

using namespace std::string_literals;
using namespace std::chrono_literals;
using namespace Library;

namespace GameDesktop
{
	//Forwarded Classes
	class Map;

	class Game final : public RTTI, public IEventSubscriber
	{
		RTTI_DECLARATIONS(Game, RTTI)

#pragma region Static Members
	public:
		inline static const std::string DefaultTitle = "Game"s;
		inline static const std::pair<int, int> DefaultScreenSize = { 1024, 780 };
		inline static const bool StartFullscreen = false;
		inline static const bool StartCentered = true;
		inline static const std::chrono::milliseconds DefaultGameDuration = 2min;

		inline static const std::string ContentDirectory = "Content/"s;
		inline static const std::string WorldFilename = "MenuTest.json"s;
		inline static const std::string MenuFilename = "MainMenu.json"s;
		inline static const std::string WorldName = "World"s;
		inline static const std::string MapAttributes = "TestMap.json"s;
		inline static const std::string MapFilename = "level.txt"s;
		inline static const World::Key GameKey = "Game"s;
		inline static const World::Key WindowOptionsKey = "WindowOptions"s;
		inline static const World::Key TitleKey = "Title"s;
		inline static const World::Key WindowPositionKey = "Position"s;
		inline static const World::Key WindowSizeKey = "Size"s;
		inline static const World::Key FullscreenKey = "SetFullscreen"s;
		inline static const World::Key CenterWindowKey = "SetCentered"s;

	private:
		inline static InputManager sInputManager;
		inline static StateManager sStateManager;
#pragma endregion Static Members

	public:
		Game();
		~Game();

		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;
		Game(Game&&) = delete;
		Game& operator=(Game&&) = delete;

	public:
		void Run();
		void Exit();

		virtual void Notify(EventPublisher& eventPublisher) override;
		
	public:
		WindowManager& GetWindowManager();
		RenderQueue& GetRenderQueue();

		Map& GetMap();
		const Map& GetMap() const;
		
		const std::chrono::high_resolution_clock::time_point& GetEndTime() const;
	private:
		void Initialize();

		void InitializeWorld();

		void InitializeMap();
		void InitializeWindowOptions();
		void InitializeOpenGL();
		void Shutdown();

		void LoadSector(const std::string& filename);

		static void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods);
		
	private:
		WindowManager mWindowManager;

		GameTime mGameTime;
		EventQueue mEventQueue;

		World mWorld{ World(WorldName, &mGameTime, &mEventQueue) };
		Map mMap;
		RenderQueue mRenderQueue;

		std::chrono::high_resolution_clock::time_point mEndTime;
		AIManager mAIManager;
	};
}