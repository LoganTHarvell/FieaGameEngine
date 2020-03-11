#pragma once

#include "Factory.h"

namespace Library
{
	template<typename T>
	inline gsl::owner<T*> Factory<T>::Create(const std::string& className)
	{
		const Factory<T>* const factoryPtr = Find(className);
		return factoryPtr ? factoryPtr->Create() : nullptr;
	}

	template<typename T>
	inline const Factory<T>* const Factory<T>::Find(const std::string& className)
	{
		auto it = mRegistry.Find(className);
		return it != mRegistry.end() ? &it->second : nullptr;
	}

	template<typename T>
	inline void Factory<T>::Add(const Factory& factory)
	{
		auto [it, isNew] = mRegistry.Insert({ factory.ClassName(), factory });
		if (!isNew) throw std::runtime_error("Factory registered more than once.");
	}

	template<typename T>
	inline bool Factory<T>::Remove(const Factory& factory)
	{
		return mRegistry.Remove(factory.ClassName());
	}
}