#pragma once

#pragma region Includes
#include "GLFW/glfw3.h"
#include "RenderManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#pragma endregion Includes
namespace GameDesktop
{
	class WindowManager final
	{
		friend class Game;
		
	public:
		GLFWwindow* Window();
		const GLFWwindow* Window() const;

		const std::string& Title() const;
		void SetTitle(const std::string& title);

		int X() const;
		int Y() const;
		void SetWindowPosition(int x, int y);

		int Width() const;
		int Height() const;
		void SetWindowSize(int width, int height);

		bool IsFullscreen() const;
		void SetFullscreen(const bool fullscreen);
		
		void SetCentered(const bool centered, GLFWmonitor* monitor=nullptr);
		
		float AspectRatio() const;

	public:
		void UpdateTitle();
		void UpdateWindowPosition();
		void UpdateWindowSize();
		void UpdateFullscreen();
		void CenterWindow(GLFWmonitor* monitor);

	private:
		void InitializeWindow();
		void ShutdownWindow();

	private:
		GLFWwindow* mWindow{ nullptr };
		std::string mTitle;
		int mScreenPositionStorage[2]{ 0, 0 };
		int mScreenSizeStorage[2]{ 640, 480 };
		int mIsFullscreen{ 0 };
		int mIsCentered{ 1 };
	};
}

#include "WindowManager.inl"