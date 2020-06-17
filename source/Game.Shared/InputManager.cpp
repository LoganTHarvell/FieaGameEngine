#include "pch.h"
#include "InputManager.h"

using namespace Library;

namespace GameDesktop
{
	InputManager::InputManager(Library::World* world) : mWorld(world) {}

	void InputManager::AddKeyboardHandler(KeyboardHandler handler)
	{
		if (!handler) throw std::runtime_error("Can't add standard functor to input handler");

		mKeyboardInputHandlers.EmplaceBack(handler);
	}

	void InputManager::AddGamepadHandler(GamepadHandler handler)
	{
		mGamepadInputHandlers.EmplaceBack(std::make_shared<GamepadHandler>(handler));
	}

	const World* InputManager::GetWorld() const
	{
		return mWorld;
	}

	void InputManager::SetWorld(Library::World* world)
	{
		mWorld = world;
	}

	void InputManager::ClearInputHandlers()
	{
		mKeyboardInputHandlers.Clear();
		mGamepadInputHandlers.Clear();
	}

	void InputManager::Update()
	{
		assert(mWorld != nullptr);

		for (const auto& handler : mGamepadInputHandlers)
		{
			handler->operator()(*mWorld);
		}
	}

	void InputManager::OnKey([[maybe_unused]] GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		assert(mWorld != nullptr);

		for (auto& handler : mKeyboardInputHandlers)
		{
			handler(key, scancode, action, mods, *mWorld);
		}
	}
}