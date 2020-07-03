#include "pch.h"

#include "World.h"
#include "GameTime.h"
#include "EventQueue.h"

using namespace Library;
using namespace std::string_literals;

int main()
{
	TypeManager::Create();

	RegisterType<Entity>();
	RegisterType<World>();

	EntityFactory entityFactory;

	GameTime gameTime;
	EventQueue eventQueue;

	World world("World"s, &gameTime, &eventQueue);

	world.Initialize();
	//world.Run();
	world.Shutdown();

	TypeManager::Destroy();
	return 0;
}
