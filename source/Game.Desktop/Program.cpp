#include "pch.h"

#include "Game.h"
#include "GameTypes.h"

using namespace GameDesktop;

#if defined(_WIN32)
int WINAPI WinMain(HINSTANCE /*instance*/, HINSTANCE /*previousInstance*/, LPSTR /*commandLine*/, int /*showCommand*/)
#else
int main()
#endif
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	// Checking for memory leak
#endif
	srand(static_cast<unsigned int>(time(NULL)));
	TypeManager::Create();
	GameTypes::Create();

	Game game;
	game.Run();

	GameTypes::Destroy();
	TypeManager::Destroy();
}