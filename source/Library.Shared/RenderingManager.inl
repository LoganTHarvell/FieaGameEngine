#pragma once

#include "RenderingManager.h"

namespace Library
{
	inline RenderingManager::API RenderingManager::GetAPI() const
	{
		return mAPI;
	}

	inline const RenderingManager::WindowCallback& RenderingManager::GetWindowCallback() const
	{
		return mGetWindow;
	}
}
