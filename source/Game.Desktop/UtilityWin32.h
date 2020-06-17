#pragma once

#include <Windows.h>
#include <string>
#include <functional>
#include <vector>
#include <memory>

namespace Library
{
	class UtilityWin32 final
	{
	public:
		using WndProcHandler = std::function<bool(HWND, UINT, WPARAM, LPARAM)>;

		static void InitializeWindow(WNDCLASSEX& window, HWND& windowHandle, HINSTANCE instance, const std::wstring& className, const std::wstring& windowTitle, const SIZE& renderTargetSize, int showCommand);
		static LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
		static POINT CenterWindow(const SIZE& windowSize);

		static std::filesystem::path ExecutableDirectory();

		static const std::vector<std::shared_ptr<WndProcHandler>>& WndProcHandlers();
		static void AddWndProcHandler(std::shared_ptr<WndProcHandler> handler);
		static void RemoveWndProcHandler(std::shared_ptr<WndProcHandler> handler);

		UtilityWin32() = delete;
		UtilityWin32(const UtilityWin32&) = delete;
		UtilityWin32& operator=(const UtilityWin32&) = delete;
		UtilityWin32(UtilityWin32&&) = delete;
		UtilityWin32& operator=(UtilityWin32&&) = delete;
		~UtilityWin32() = default;

	private:
		static std::vector<std::shared_ptr<WndProcHandler>> sWndProcHandlers;
	};
}