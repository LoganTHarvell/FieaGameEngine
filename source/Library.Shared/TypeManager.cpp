#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "TypeManager.h"

// First Party
#include "Attributed.h"
#pragma endregion Includes

namespace Library
{
#pragma region Instance Management
	void TypeManager::Create(const std::size_t capacity)
	{
		if (!mInstance) mInstance = new TypeManager();
		mInstance->mRegistry.Rehash(capacity);
	}

	void TypeManager::Destroy()
	{
		if (mInstance) delete mInstance;
		mInstance = nullptr;
	}

	TypeManager* TypeManager::Instance()
	{
		return mInstance;
	}
#pragma endregion Instance Management

#pragma region Registry
	const TypeManager::TypeInfo* TypeManager::Find(const IdType typeId) const
	{
		auto it = mRegistry.Find(typeId);
		return it != mRegistry.end() ? &it->second : nullptr;
	}

	bool TypeManager::IsRegistered(const IdType typeId) const
	{
		return mRegistry.Find(typeId) != mRegistry.end();
	}
	
	void TypeManager::Deregister(const IdType typeId)
	{
		mRegistry.Remove(typeId);
	}

	void TypeManager::Clear()
	{
		mRegistry.Clear();
	}
#pragma endregion Registry
}
