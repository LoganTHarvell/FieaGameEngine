#pragma once
#include "GameObject.h"
#include "Factory.h"
#include "Action.h"

namespace GameDesktop
{
	class MenuDisplay : public GameObject
	{
		RTTI_DECLARATIONS(MenuDisplay, GameObject)
	public:
		static const TypeManager::TypeInfo& TypeInfo();
	private:
	};
	ConcreteFactory(MenuDisplay, Scope)
}

