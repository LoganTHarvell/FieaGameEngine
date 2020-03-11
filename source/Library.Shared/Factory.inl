#pragma once

// Header File
#include "Factory.h"

namespace Library
{
#pragma region Static Methods
	template<typename T>
	inline gsl::owner<T*> Factory<T>::Create(const std::string& className)
	{
		auto factoryIt = mRegistry.Find(className);
		return factoryIt != mRegistry.end() ? factoryIt->second.Create() : nullptr;
	}

	template<typename T>
	inline float Factory<T>::RegistryLoadFactor()
	{
		return mRegistry.LoadFactor();
	}

	template<typename T>
	inline void Factory<T>::RegistryRehash(const std::size_t size)
	{
		mRegistry.Rehash(size);
	}

	template<typename T>
	inline void Factory<T>::Register(const Factory& factory)
	{
		auto [it, isNew] = mRegistry.Insert({ factory.ClassName(), factory });
		if (!isNew) throw std::runtime_error("Factory registered more than once.");
	}

	template<typename T>
	inline bool Factory<T>::Deregister(const Factory& factory)
	{
		return mRegistry.Remove(factory.ClassName());
	}
#pragma endregion Static Methods
}