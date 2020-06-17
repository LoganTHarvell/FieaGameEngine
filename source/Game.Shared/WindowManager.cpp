#include "pch.h"

#include "WindowManager.h"

namespace GameDesktop
{
	void WindowManager::InitializeWindow()
	{
		if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW.");

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		// Setup GLFW window properties
		// OpenGL version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		// Core profile = No backwards compatibility
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		// Allow forward compatibility
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		mWindow = glfwCreateWindow(mScreenSizeStorage[0], mScreenSizeStorage[1], 
								   mTitle.c_str(), nullptr, nullptr);

		if (!mWindow) throw std::runtime_error("Failed to initialize window.");
	}

	void WindowManager::ShutdownWindow()
	{
		glfwDestroyWindow(mWindow);
		glfwTerminate();
	}

	void WindowManager::SetCentered(const bool centered, GLFWmonitor* monitor)
	{
		mIsCentered = static_cast<int>(centered);
		if (mWindow && mIsCentered) CenterWindow(monitor ? monitor : glfwGetPrimaryMonitor());
	}

	void WindowManager::CenterWindow(GLFWmonitor* monitor)
	{
		assert(monitor != nullptr);

		const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
		if (!videoMode) throw std::runtime_error("Could not retrieve video mode.");

		glfwGetMonitorPos(monitor, &mScreenPositionStorage[0], &mScreenPositionStorage[1]);

		mScreenPositionStorage[0] += (videoMode->width - mScreenSizeStorage[0]) / 2;
		mScreenPositionStorage[1] += (videoMode->height - mScreenSizeStorage[1]) / 2;

		UpdateWindowPosition();
		mIsCentered = false;
	}
}
