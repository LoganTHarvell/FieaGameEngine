#pragma once

#include "RenderingManager.h"

namespace Library
{
	inline const RenderingManager::WindowCallback& RenderingManager::GetWindowCallback() const
	{
		return mGetWindow;
	}
}
