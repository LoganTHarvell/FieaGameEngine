#include "WindowManager.h"

namespace GameDesktop
{
	inline GLFWwindow* WindowManager::Window()
	{
		return mWindow;
	}

	inline const GLFWwindow* WindowManager::Window() const
	{
		return mWindow;
	}

	inline const std::string& WindowManager::Title() const
	{
		return mTitle;
	}

	inline void WindowManager::SetTitle(const std::string& title)
	{
		mTitle = title;
		UpdateTitle();
	}
	
	inline int WindowManager::X() const
	{
		return mScreenPositionStorage[0];
	}
	
	inline int WindowManager::Y() const
	{
		return mScreenPositionStorage[1];
	}

	inline void WindowManager::SetWindowPosition(int x, int y)
	{
		mScreenPositionStorage[0] = x;
		mScreenPositionStorage[1] = y;
		UpdateWindowPosition();
	}
	
	inline int WindowManager::Width() const
	{
		return mScreenSizeStorage[0];
	}
	
	inline int WindowManager::Height() const
	{
		return mScreenSizeStorage[1];
	}


	inline void WindowManager::SetWindowSize(int width, int height)
	{
		mScreenSizeStorage[0] = width;
		mScreenSizeStorage[1] = height;
		RenderManager::SetProjectionMatrix(glm::ortho(0.0f, float(width), float(height), 0.0f, -100.0f, 100.0f));
		UpdateWindowSize();
	}
	
	inline bool WindowManager::IsFullscreen() const
	{
		return mWindow ? glfwGetWindowMonitor(mWindow) != nullptr : false;
	}
	
	inline void WindowManager::SetFullscreen(const bool fullscreen)
	{
		mIsFullscreen = static_cast<int>(fullscreen);
		UpdateFullscreen();
	}
	
	inline float WindowManager::AspectRatio() const
	{
		return static_cast<float>(Width()) / static_cast<float>(Height());
	}


	inline void WindowManager::UpdateTitle()
	{
		if (mWindow) glfwSetWindowTitle(mWindow, mTitle.c_str());
	}

	inline void WindowManager::UpdateWindowPosition()
	{
		if (mWindow)
		{
			glfwSetWindowPos(mWindow, mScreenPositionStorage[0], mScreenPositionStorage[1]);
		}
	}

	inline void WindowManager::UpdateWindowSize()
	{
		if (mWindow)
		{
			glfwSetWindowSize(mWindow, mScreenSizeStorage[0], mScreenSizeStorage[1]);
		}
	}

	inline void WindowManager::UpdateFullscreen()
	{
		if (mWindow)
		{
			glfwGetWindowPos(mWindow, &mScreenPositionStorage[0], &mScreenPositionStorage[1]);
			glfwGetWindowSize(mWindow, &mScreenSizeStorage[0], &mScreenSizeStorage[1]);

			glfwSetWindowMonitor(mWindow, mIsFullscreen ? glfwGetPrimaryMonitor() : nullptr,
								 mScreenPositionStorage[0], mScreenPositionStorage[1],
								 mScreenSizeStorage[0], mScreenSizeStorage[1],
								 GLFW_DONT_CARE);
		}
	}
}
