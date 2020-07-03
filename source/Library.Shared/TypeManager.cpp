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
#pragma region Signature
	bool Signature::operator==(const Signature& rhs) const noexcept
	{
		return Key == rhs.Key
			&& Type == rhs.Type
			&& IsInternal == rhs.IsInternal
			&& Size == rhs.Size
			&& Offset == rhs.Offset;
	}
	
	bool Signature::operator!=(const Signature& rhs) const noexcept
	{
		return !operator==(rhs);
	}
#pragma endregion Signature
	
#pragma region Instance Management
	void TypeManager::Create(const std::size_t capacity)
	{
		if (!mInstance) mInstance = new TypeManager();
		mInstance->mRegistry.Rehash(capacity);
	}

	void TypeManager::Destroy()
	{
		delete mInstance;
		mInstance = nullptr;
	}

	TypeManager* TypeManager::Instance()
	{
		return mInstance;
	}
#pragma endregion Instance Management

#pragma region Registry
	const TypeInfo* TypeManager::Find(const IdType typeId) const
	{
		const auto it = mRegistry.Find(typeId);
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

	void TypeManager::RegistryRehash(const std::size_t size)
	{
		mRegistry.Rehash(size);
	}

	void TypeManager::Clear()
	{
		mRegistry.Clear();
	}
#pragma endregion Registry
}
