#pragma once
#include <functional>
#include <GLFW/glfw3.h>
#include "EventQueue.h"
#include "Vector.h"
#include "World.h"

namespace GameDesktop
{
	class InputManager final
	{
	public:
		typedef std::function<void(int, int, int, int, Library::World&)> KeyboardHandler;
		typedef std::function<void(Library::World&)> GamepadHandler;

		InputManager(Library::World* world = nullptr);
		InputManager(const InputManager& rhs) = default;
		InputManager(InputManager&& rhs) = default;
		InputManager& operator=(const InputManager& rhs) = default;
		InputManager& operator=(InputManager&& rhs) = default;
		~InputManager() = default;

		void AddKeyboardHandler(KeyboardHandler handler);

		void AddGamepadHandler(GamepadHandler handler);

		const Library::World* GetWorld() const;
		void SetWorld(Library::World* world);

		void ClearInputHandlers();

		void Update();

		void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods);

	private:
		Library::Vector<KeyboardHandler> mKeyboardInputHandlers{ Library::Vector(Library::Vector<KeyboardHandler>::EqualityFunctor()) };
		Library::Vector<std::shared_ptr<GamepadHandler>> mGamepadInputHandlers{ Library::Vector(Library::Vector<std::shared_ptr<GamepadHandler>>::EqualityFunctor()) };
		Library::World* mWorld;
	};
}

