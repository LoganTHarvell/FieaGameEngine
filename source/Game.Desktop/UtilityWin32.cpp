#include "pch.h"
#include "UtilityWin32.h"
#include <windows.h>
#include <string>
#include <Shlwapi.h>
#include <algorithm>

using namespace std;
using namespace std::filesystem;

namespace Library
{
	vector<shared_ptr<UtilityWin32::WndProcHandler>> UtilityWin32::sWndProcHandlers;

	void UtilityWin32::InitializeWindow(WNDCLASSEX& window, HWND& windowHandle, HINSTANCE instance, const wstring& className, const wstring& windowTitle, const SIZE& renderTargetSize, int showCommand)
	{
		ZeroMemory(&window, sizeof(window));
		window.cbSize = sizeof(WNDCLASSEX);
		window.style = CS_CLASSDC;
		window.lpfnWndProc = WndProc;
		window.hInstance = instance;
		window.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		window.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
		window.hCursor = LoadCursor(nullptr, IDC_ARROW);
		window.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
		window.lpszClassName = className.c_str();

		RECT windowRectangle = { 0, 0, renderTargetSize.cx, renderTargetSize.cy };
		AdjustWindowRect(&windowRectangle, WS_OVERLAPPEDWINDOW, FALSE);

		RegisterClassEx(&window);
		POINT center = CenterWindow(renderTargetSize);
		windowHandle = CreateWindow(className.c_str(), windowTitle.c_str(), WS_OVERLAPPEDWINDOW, center.x, center.y, windowRectangle.right - windowRectangle.left, windowRectangle.bottom - windowRectangle.top, nullptr, nullptr, instance, nullptr);

		ShowWindow(windowHandle, showCommand);
		UpdateWindow(windowHandle);
	}

	POINT UtilityWin32::CenterWindow(const SIZE& windowSize)
	{
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		POINT center;
		center.x = (screenWidth - windowSize.cx) / 2;
		center.y = (screenHeight - windowSize.cy) / 2;

		return center;
	}

	path UtilityWin32::ExecutableDirectory()
	{
		int argCount;
		LPWSTR* args = CommandLineToArgvW(GetCommandLine(), &argCount);
		assert(args != nullptr);
		return path(args[0]).parent_path();
	}

	LRESULT WINAPI UtilityWin32::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		for (auto& wndProcHandler : sWndProcHandlers)
		{
			(*wndProcHandler)(windowHandle, message, wParam, lParam);
		}

		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_ACTIVATEAPP:
			DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
			DirectX::Mouse::ProcessMessage(message, wParam, lParam);
			break;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
			break;

		case WM_INPUT:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEHOVER:
			DirectX::Mouse::ProcessMessage(message, wParam, lParam);
			break;
		}

		return DefWindowProc(windowHandle, message, wParam, lParam);
	}

	const vector<shared_ptr<UtilityWin32::WndProcHandler>>& UtilityWin32::WndProcHandlers()
	{
		return sWndProcHandlers;
	}

	void UtilityWin32::AddWndProcHandler(shared_ptr<WndProcHandler> handler)
	{
		sWndProcHandlers.push_back(handler);
	}

	void UtilityWin32::RemoveWndProcHandler(shared_ptr<WndProcHandler> handler)
	{
		sWndProcHandlers.erase(find(sWndProcHandlers.cbegin(), sWndProcHandlers.cend(), handler));
	}
}