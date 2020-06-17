#include "PlayerInputHandlers.h"

namespace GameDesktop
{
	using namespace Library;
	using namespace std::chrono;

	template<typename T>
	void  PlayerInputHandlers::ControllerInput(Library::World& world, int controllerId, Debouncers& debouncer)
	{
		debouncer;

		if (glfwJoystickIsGamepad(controllerId))
		{
			T message;
			message.SetWorld(&world);

			std::shared_ptr<Event<T>> event;
			EventQueue& eventQueue = *world.GetWorldState().EventQueue;

			GLFWgamepadstate state;

			if (glfwGetGamepadState(controllerId, &state))
			{
				float xAxis = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
				float yAxis = state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];

#pragma region Movement
				if (std::abs(xAxis) > PlayerInputHandlers::AxisClamp || std::abs(yAxis) > PlayerInputHandlers::AxisClamp)
				{
					debouncer.moveDebounce = false;
					message.SetType(T::MessageType::Movement);

					if (std::abs(xAxis) > std::abs(yAxis))
					{
						if (xAxis > 0)
						{
							message["X-Axis"] = std::ceil(xAxis);
						}
						else
						{
							message["X-Axis"] = std::floor(xAxis);
						}
						message["Y-Axis"] = 0.0f;
					}
					else
					{
						if (yAxis > 0)
						{
							message["Y-Axis"] = std::ceil(yAxis);
						}
						else
						{
							message["Y-Axis"] = std::floor(yAxis);
						}
						message["X-Axis"] = 0.0f;
					}
					

					std::string str = "Y-Axis " + std::to_string(yAxis) + "X-Axis " + std::to_string(xAxis) + "\n";
					OutputDebugString(str.c_str());

					event = std::make_shared<Event<T>>(Event<T>(message));
					eventQueue.Enqueue(event);
				}
				else if (!debouncer.moveDebounce)
				{
					debouncer.moveDebounce = true;

					message.SetType(T::MessageType::Movement);
					message["Y-Axis"] = 0.0f;
					message["X-Axis"] = 0.0f;

					std::string str = "Y-Axis " + std::to_string(yAxis) + "X-Axis " + std::to_string(xAxis) + "\n";
					OutputDebugString(str.c_str());

					event = std::make_shared<Event<T>>(Event<T>(message));
					eventQueue.Enqueue(event);
				}
#pragma endregion Movement

#pragma region SpawnBomb
				if (!debouncer.aDebounce && state.buttons[GLFW_GAMEPAD_BUTTON_A] == GLFW_PRESS)
				{
					debouncer.aDebounce = true;
					OutputDebugStringW(L"SpawnBomb\n");

					message.SetType(PlayerInputMessage::MessageType::SpawnBomb);

					event = std::make_shared<Event<T>>(Event<T>(message));
					eventQueue.Enqueue(event);
				}
				else if (debouncer.aDebounce && state.buttons[GLFW_GAMEPAD_BUTTON_A] == GLFW_RELEASE)
				{
					debouncer.aDebounce = false;
				}
#pragma endregion SpawnBomb

#pragma region Punch
				if (!debouncer.bDebounce && state.buttons[GLFW_GAMEPAD_BUTTON_B] == GLFW_PRESS)
				{
					OutputDebugStringW(L"Punch\n");

					debouncer.bDebounce = true;
					message.SetType(PlayerInputMessage::MessageType::Punch);

					event = std::make_shared<Event<T>>(Event<T>(message));
					eventQueue.Enqueue(event);
				}
				else if (debouncer.bDebounce && state.buttons[GLFW_GAMEPAD_BUTTON_B] == GLFW_RELEASE)
				{
					debouncer.bDebounce = false;
				}
#pragma endregion Punch

#pragma region StopKick
				if (!debouncer.xDebounce && state.buttons[GLFW_GAMEPAD_BUTTON_X] == GLFW_PRESS)
				{
					OutputDebugStringW(L"StopKick\n");

					debouncer.xDebounce = true;
					message.SetType(PlayerInputMessage::MessageType::StopKick);

					event = std::make_shared<Event<T>>(Event<T>(message));
					eventQueue.Enqueue(event);
				}
				else if (debouncer.xDebounce && state.buttons[GLFW_GAMEPAD_BUTTON_X] == GLFW_RELEASE)
				{
					debouncer.xDebounce = false;
				}
#pragma endregion StopKick
			}
		}
	}
}