#include "pch.h"
#include "PlayerInputHandlers.h"

using namespace Library;
using namespace std;
using namespace std::chrono;

namespace GameDesktop
{
	void PlayerInputHandlers::Player1Input(int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods, World& world)
	{
		Player1InputMessage message;
		message.SetWorld(&world);

		std::shared_ptr<Event<Player1InputMessage>> event;
		EventQueue& eventQueue = *world.GetWorldState().EventQueue;

#pragma region Movement
		if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			currentKey = GLFW_KEY_UP;
			OutputDebugStringW(L"P1 Move Up\n");

			message.SetType(Player1InputMessage::MessageType::Movement);
			message["Y-Axis"] = -1.0f;
			message["X-Axis"] = 0.0f;

			event = std::make_shared<Event<Player1InputMessage>>(Event<Player1InputMessage>(message));
			eventQueue.Enqueue(event, high_resolution_clock::now());
		}
		else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			currentKey = GLFW_KEY_DOWN;
			OutputDebugStringW(L"P1 Move Down\n");

			message.SetType(Player1InputMessage::MessageType::Movement);
			message["Y-Axis"] = 1.0f;
			message["X-Axis"] = 0.0f;

			event = std::make_shared<Event<Player1InputMessage>>(Event<Player1InputMessage>(message));
			eventQueue.Enqueue(event, high_resolution_clock::now());
		}
		else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			currentKey = GLFW_KEY_RIGHT;
			OutputDebugStringW(L"P1 Move Right\n");

			message.SetType(Player1InputMessage::MessageType::Movement);
			message["Y-Axis"] = 0.0f;
			message["X-Axis"] = 1.0f;

			event = std::make_shared<Event<Player1InputMessage>>(Event<Player1InputMessage>(message));
			eventQueue.Enqueue(event, high_resolution_clock::now());
		}
		else if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{

			currentKey = GLFW_KEY_LEFT;

			OutputDebugStringW(L"P1 Move Left\n");

			message.SetType(Player1InputMessage::MessageType::Movement);
			message["Y-Axis"] = 0.0f;
			message["X-Axis"] = -1.0f;

			event = std::make_shared<Event<Player1InputMessage>>(Event<Player1InputMessage>(message));
			eventQueue.Enqueue(event, high_resolution_clock::now());
		}
		
		if (key == currentKey && action == GLFW_RELEASE)
		{
			OutputDebugStringW(L"P1 Move Left\n");

			message.SetType(Player1InputMessage::MessageType::Movement);
			message["Y-Axis"] = 0.0f;
			message["X-Axis"] = 0.0f;

			event = std::make_shared<Event<Player1InputMessage>>(Event<Player1InputMessage>(message));
			eventQueue.Enqueue(event, high_resolution_clock::now());
		}
#pragma endregion Movement

#pragma region SpawnBomb
		if (key == GLFW_KEY_C && action == GLFW_PRESS)
		{
			OutputDebugStringW(L"P1 SpawnBomb\n");

			message.SetType(PlayerInputMessage::MessageType::SpawnBomb);

			event = std::make_shared<Event<Player1InputMessage>>(Event<Player1InputMessage>(message));
			eventQueue.Enqueue(event, high_resolution_clock::now());
		}
#pragma endregion SpawnBomb

#pragma region Punch
		if (key == GLFW_KEY_Z && action == GLFW_PRESS)
		{
			OutputDebugStringW(L"P1 Punch\n");

			message.SetType(PlayerInputMessage::MessageType::Punch);

			event = std::make_shared<Event<Player1InputMessage>>(Event<Player1InputMessage>(message));
			eventQueue.Enqueue(event, high_resolution_clock::now());
		}
#pragma endregion Punch

#pragma region StopKick
		if (key == GLFW_KEY_X && action == GLFW_PRESS)
		{
			OutputDebugStringW(L"P1 Stop Kick\n");

			message.SetType(PlayerInputMessage::MessageType::StopKick);

			event = std::make_shared<Event<Player1InputMessage>>(Event<Player1InputMessage>(message));
			eventQueue.Enqueue(event, high_resolution_clock::now());
		}
#pragma endregion StopKick
	}

	void PlayerInputHandlers::Player2Input(Library::World& world)
	{
		ControllerInput<Player2InputMessage>(world, GLFW_JOYSTICK_1, p2Debouncers);
	}

	void PlayerInputHandlers::Player3Input(Library::World& world)
	{
		ControllerInput<Player3InputMessage>(world, GLFW_JOYSTICK_2, p3Debouncers);
	}

	void PlayerInputHandlers::Player4Input(Library::World& world)
	{
		ControllerInput<Player4InputMessage>(world, GLFW_JOYSTICK_3, p4Debouncers);
	}
}