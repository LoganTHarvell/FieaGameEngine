#pragma once

// Header
#include "Factory.h"

namespace Library
{
#pragma region Static Methods
	template<typename T>
	inline gsl::owner<T*> Factory<T>::Create(const std::string& className)
	{
		auto factoryIt = sRegistry.Find(className);
		return factoryIt != sRegistry.end() ? factoryIt->second.Create() : nullptr;
	}

	template<typename T>
	inline bool Factory<T>::IsRegistered(const std::string& className)
	{
		return sRegistry.Find(className) != sRegistry.end();
	}

	template<typename T>
	inline float Factory<T>::RegistryLoadFactor()
	{
		return sRegistry.LoadFactor();
	}

	template<typename T>
	inline void Factory<T>::RegistryRehash(const std::size_t size)
	{
		sRegistry.Rehash(size);
	}

	template<typename T>
	inline void Factory<T>::Register(const Factory& factory)
	{
		auto [it, isNew] = sRegistry.Insert({ factory.ClassName(), factory });
		if (!isNew) throw std::runtime_error("Factory registered more than once.");
	}

	template<typename T>
	inline bool Factory<T>::Deregister(const Factory& factory)
	{
		return sRegistry.Remove(factory.ClassName());
	}
#pragma endregion Static Methods
}