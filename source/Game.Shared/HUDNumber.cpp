#include "pch.h"
#include "HUDNumber.h"

namespace GameDesktop
{
	const TypeManager::TypeInfo& HUDNumber::TypeInfo()
	{
		return GameObject::TypeInfo();
	}
	HUDNumber::HUDNumber(const std::string& name) :
		GameObject(TypeIdClass(), name)
	{
	}
	void GameDesktop::HUDNumber::UpdateNumber(unsigned int number)
	{
		GetRenderComponent().SetColumnIndex(number);
	}
}
