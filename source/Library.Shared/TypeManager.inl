#pragma once

// Header
#include "TypeManager.h"

namespace Library
{
#pragma region Registry
	template<typename T>
	inline void TypeManager::Register()
	{
		if (T::Base::TypeIdClass() != Attributed::TypeIdClass() && !mRegistry.ContainsKey(T::Base::TypeIdClass()))
		{
			throw std::runtime_error("Parent type is not registered.");
		}

		const TypeInfo typeInfo = { T::Signatures(), T::Base::TypeIdClass() };
		
		const bool isNew = mRegistry.TryEmplace(T::TypeIdClass(), typeInfo).second;

		if (!isNew)
		{
			throw std::runtime_error("Type registered more than once.");
		}
	}

	inline float TypeManager::RegistryLoadFactor() const
	{
		return mRegistry.LoadFactor();
	}
#pragma endregion Registry

#pragma region Global Registry Function
	template<typename T>
	inline void RegisterType()
	{
		if (!TypeManager::Instance()) throw std::runtime_error("TypeManager instance null.");

		TypeManager::Instance()->Register<T>();
	}
#pragma endregion Global Registry Function
}