#include "pch.h"


#include "World.h"
#include "GameTime.h"
#include "EventQueue.h"
#include "JsonParseMaster.h"
#include "JsonEntityParseHelper.h"

#include "Model.h"
#include "Actor.h"

#include "RenderingGame.h"
#include "GameException.h"
#include "SolarBody.h"
#include "SolarSystem.h"
#include "UtilityWin32.h"

using namespace std::string_literals;
using namespace Library;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int showCommand)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	TypeManager::Create();
	RegisterType<Entity>();
	RegisterType<Actor>();
	RegisterType<Model>();
	RegisterType<Demo::SolarSystem>();
	RegisterType<Demo::SolarBody>();

	EntityFactory entityFactory;
	ActorFactory actorFactory;
	ModelFactory modelFactory;
	Demo::SolarSystemFactory solarSystemFactory;
	Demo::SolarBodyFactory solarBodyFactory;
	
	ThrowIfFailed(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED), "Error initializing COM.");

	std::filesystem::current_path(UtilityWin32::ExecutableDirectory());

	const std::wstring windowClassName = L"RenderingClass";
	const std::wstring windowTitle = L"Solar System Demo";

	const SIZE RenderTargetSize = { 1024, 768 };
	HWND windowHandle;
	WNDCLASSEX window;

	UtilityWin32::InitializeWindow(window, windowHandle, instance, windowClassName, windowTitle, RenderTargetSize, showCommand);

	auto getRenderTargetSize = [&RenderTargetSize](SIZE& renderTargetSize)
	{
		renderTargetSize = RenderTargetSize;
	};

	auto getWindow = [&]() -> void*
	{
		return reinterpret_cast<void*>(windowHandle);
	};

	Demo::RenderingGame game(getWindow, getRenderTargetSize);
	game.UpdateRenderTargetSize();
	game.Initialize();

	MSG message = { 0 };

	try
	{
		while (message.message != WM_QUIT)
		{
			if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
			else
			{
				game.Run();
			}
		}
	}
	catch (GameException ex)
	{
		MessageBox(windowHandle, ex.whatw().c_str(), windowTitle.c_str(), MB_ABORTRETRYIGNORE);
	}

	game.Shutdown();
	UnregisterClass(windowClassName.c_str(), window.hInstance);
	CoUninitialize();

	TypeManager::Destroy();
	return static_cast<int>(message.wParam);
}