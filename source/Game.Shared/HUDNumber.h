#pragma once
#include "GameObject.h"
#include "Factory.h"

namespace GameDesktop
{
	class HUDNumber : public GameObject
	{
		RTTI_DECLARATIONS(HUDNumber, GameObject)
	public:
		static const TypeManager::TypeInfo& TypeInfo();

	public:
		explicit HUDNumber(const std::string& name = std::string());
		HUDNumber(const HUDNumber&) = default;
		HUDNumber(HUDNumber&&) = default;
		HUDNumber& operator=(const HUDNumber&) = default;
		HUDNumber& operator=(HUDNumber&&) = default;
		virtual ~HUDNumber() = default;

		void UpdateNumber(unsigned int number);
	private:
		unsigned int mNumber = 0;
	};
	ConcreteFactory(HUDNumber, Scope)
}

