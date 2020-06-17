#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include <ctime>
#include "Game.h"

// Third Party
#include <GL/gl3w.h>

// First Party
#include "JsonParseMaster.h"
#include "JsonScopeParseHelper.h"

#include "Sector.h"
#include "Character.h"
#include "ActionCharacterMovementInput.h"
#include "ActionSpawnBomb.h"
#include "Transform.h"
#include "windows.h"
#include "PlayerInputReaction.h"
#include "Player1InputReaction.h"
#include "Player2InputReaction.h"
#include "Player3InputReaction.h"
#include "Player4InputReaction.h"

#include "Player3InputMessage.h"
#include "Player4InputMessage.h"
#include "Event.h"
#include "GameMessage.h"

#include "DestructibleWall.h"
#include "Explosion.h"

#include "StateManager.h"

#include "AudioManager.h"
#pragma endregion Includes

namespace GameDesktop
{
	void Game::Initialize()
	{
		mWindowManager.InitializeWindow();
		InitializeOpenGL();
		RenderManager::Initialize();
		AudioManager::Create();
		InitializeWorld();
		InitializeWindowOptions();
		InitializeMap();

		mWindowManager.UpdateTitle();
		mWindowManager.UpdateWindowPosition();
		mWindowManager.UpdateWindowSize();
		mWindowManager.UpdateFullscreen();
		mWindowManager.SetCentered(mWindowManager.mIsCentered);
	
		sStateManager.ChangeState(StateManager::GameStates::Menu, sInputManager);

		//mAIManager = AIManager(AIManager::EInitState::FourPlayer);

		AudioManager::Instance()->PlayBackgroundMusic("MenuMusic");
	}

	Game::Game()
	{
		Event<GameMessage>::Subscribe(*this);
		sInputManager.SetWorld(&mWorld);
	}

	Game::~Game()
	{
		Event<GameMessage>::Unsubscribe(*this);
	}

	void Game::Run()
	{
		Initialize();
		const glm::vec4 CornflowerBlue = glm::vec4(0.392f, 0.584f, 0.929f, 1.0f);
		while (!glfwWindowShouldClose(mWindowManager.Window()))
		{
			glfwPollEvents();
			glClearBufferfv(GL_COLOR, 0, &CornflowerBlue[0]);
			glClear(GL_DEPTH_BUFFER_BIT);

			mWorld.Update();
			mAIManager.UpdateAI(mWorld.GetWorldState());
			sInputManager.Update();
			RenderManager::Render(mRenderQueue);

			if (sStateManager.GameState() != StateManager::GameStates::Menu && mGameTime.CurrentTime() > mEndTime)
			{
				GameMessage message(MenuFilename, StateManager::GameStates::Menu);
				const auto event = std::make_shared<Event<GameMessage>>(message);
				mEventQueue.Enqueue(event);
			}
			
			glfwSwapBuffers(mWindowManager.Window());
		}

		Shutdown();
	}

	void Game::Exit()
	{
		glfwSetWindowShouldClose(mWindowManager.Window(), GL_TRUE);
	}

	void Game::Notify(EventPublisher& eventPublisher)
	{
		const auto& message = static_cast<Event<GameMessage>&>(eventPublisher).Message;
		LoadSector(message.Filename);
		mEndTime = mGameTime.CurrentTime() + DefaultGameDuration;
		mEventQueue.Clear();

		if (message.GameState != StateManager::GameStates::Menu)
		{
			mMap.ResetMap();
			mMap.PopulateMap();
		}
		
		sStateManager.ChangeState(static_cast<StateManager::GameStates>(message.GameState), sInputManager);
		//mAIManager = AIManager(static_cast<AIManager::EInitState>(static_cast<int>(message.GameState) + 1));

		AudioManager::Instance()->StopAllSound();
		AudioManager::Instance()->PlayBackgroundMusic("GameMusic");
	}

	WindowManager& Game::GetWindowManager()
	{
		return mWindowManager;
	}

	RenderQueue& Game::GetRenderQueue()
	{
		return mRenderQueue;
	}

	void Game::InitializeWorld()
	{
		mWorld.AppendAuxiliaryAttribute(GameKey) = this;

		JsonScopeParseHelper::SharedData data(mWorld);
		JsonScopeParseHelper helper;
		JsonParseMaster parser(&data);
		parser.AddHelper(helper);
		parser.ParseFromFile(ContentDirectory + WorldFilename);
	}

	void Game::InitializeMap()
	{
		mMap = Map(&mWorld, mWindowManager.Width(), mWindowManager.Height());
		JsonScopeParseHelper::SharedData data(mMap);
		JsonScopeParseHelper helper;
		JsonParseMaster parser(&data);
		parser.AddHelper(helper);
		parser.ParseFromFile(ContentDirectory + MapAttributes);
		mMap.LoadFromFile(ContentDirectory + MapFilename);
	}

	void Game::InitializeWindowOptions()
	{
		auto windowOptions = mWorld.Find(WindowOptionsKey);

		std::string* title = nullptr;
		int* position = nullptr;
		int* size = nullptr;
		int* fullscreen = nullptr;
		int* centered = nullptr;

		if (windowOptions && windowOptions->Size() > 0)
		{
			auto data = windowOptions->Get<Scope*>()->Find(TitleKey);
			if (data && data->Type() == Scope::Types::String && data->Size() > 0) title = &data->Get<std::string>();

			data = windowOptions->Get<Scope*>()->Find(WindowPositionKey);
			if (data && data->Type() == Scope::Types::Integer && data->Size() > 1) position = &data->Get<int>();

			data = windowOptions->Get<Scope*>()->Find(WindowSizeKey);
			if (data && data->Type() == Scope::Types::Integer && data->Size() > 1) size = &data->Get<int>();

			data = windowOptions->Get<Scope*>()->Find(FullscreenKey);
			if (data && data->Type() == Scope::Types::Integer && data->Size() > 0) fullscreen = &data->Get<int>();

			data = windowOptions->Get<Scope*>()->Find(CenterWindowKey);
			if (data && data->Type() == Scope::Types::Integer && data->Size() > 0) centered = &data->Get<int>();
		}

		mWindowManager.SetTitle(title ? *title : DefaultTitle);
		mWindowManager.SetWindowPosition(position ? position[0] : 0, position ? position[1] : 0);
		mWindowManager.SetWindowSize(position ? position[0] : DefaultScreenSize.first,
			position ? position[1] : DefaultScreenSize.second);
		mWindowManager.SetFullscreen(fullscreen ? *fullscreen : StartFullscreen);
		mWindowManager.SetCentered(centered ? *centered : StartCentered);

		glViewport(0, 0, mWindowManager.Width(), mWindowManager.Height());
	}

	void Game::InitializeOpenGL()
	{
		glfwMakeContextCurrent(mWindowManager.Window());

		if (gl3wInit() != 0) throw std::runtime_error("Failed to initialize OpenGL.");

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glfwSetKeyCallback(mWindowManager.Window(), Game::OnKey);
	}

	void Game::Shutdown()
	{
		mWindowManager.ShutdownWindow();
	}

	Map& Game::GetMap()
	{
		return mMap;
	}

	const Map& Game::GetMap() const
	{
		return mMap;
	}

	const std::chrono::high_resolution_clock::time_point& Game::GetEndTime() const
	{
		return mEndTime;
	}

	void Game::LoadSector(const std::string& filename)
	{
		for (const auto& pendingChild : mWorld.PendingChildren())
		{
			delete &pendingChild.Child;
		}

		mWorld.PendingChildren().Clear();
		
		delete mWorld.Orphan(*static_cast<Sector*>(mWorld.Sectors().Get<Scope*>()));
		
		Sector* newSector = new Sector();
		JsonScopeParseHelper::SharedData data(*newSector);
		JsonScopeParseHelper helper;
		JsonParseMaster parser(&data);
		parser.AddHelper(helper);
		parser.ParseFromFile(ContentDirectory + filename);
		mWorld.Adopt(*newSector, World::SectorsKey);
	}

	void Game::OnKey(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		sInputManager.OnKey(window, key, scancode, action, mods);
		window; key; scancode; action; mods;
	}
}