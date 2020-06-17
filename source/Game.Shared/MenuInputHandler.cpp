#include "pch.h"
#include "MenuInputHandler.h"
#include "MenuInputMessage.h"
#include "Event.h"
#include "EventQueue.h"

using namespace Library;
using namespace std;
using namespace std::chrono;

namespace GameDesktop
{
	void MenuInput(int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods, Library::World& world)
	{
		MenuInputMessage message;
		message.SetWorld(&world);

		std::shared_ptr<Event<MenuInputMessage>> event;
		EventQueue& eventQueue = *world.GetWorldState().EventQueue;

		if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		{
			OutputDebugStringW(L"Menu Up\n");
			message.SetType(MenuInputMessage::MessageType::Up);

			event = std::make_shared<Event<MenuInputMessage>>(Event<MenuInputMessage>(message));
			eventQueue.Enqueue(event, high_resolution_clock::now());
		}
		else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		{
			OutputDebugStringW(L"Menu Down\n");

			message.SetType(MenuInputMessage::MessageType::Down);

			event = std::make_shared<Event<MenuInputMessage>>(Event<MenuInputMessage>(message));
			eventQueue.Enqueue(event, high_resolution_clock::now());
		}
		else if (key == GLFW_KEY_C || key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
			OutputDebugStringW(L"Menu Enter\n");

			message.SetType(MenuInputMessage::MessageType::Enter);

			event = std::make_shared<Event<MenuInputMessage>>(Event<MenuInputMessage>(message));
			eventQueue.Enqueue(event, high_resolution_clock::now());
		}
	}
}