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
	void TypeManager::Create()
	{
		if (!mInstance) mInstance = new TypeManager();
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
	TypeManager::SignatureListType TypeManager::Signatures(const IdType typeId)
	{
		SList<SignatureListType*> signaturesLists;

		IdType id = typeId;
		std::size_t size = 0;

		while (id != Attributed::TypeIdClass())
		{
			auto it = mRegistry.Find(id);

			if (it == mRegistry.end()) return SignatureListType();
			
			signaturesLists.PushFront(&it->second.signatures);
			id = it->second.parentTypeId;
			size += it->second.signatures.Size();	
		}

		SignatureListType combinedSignatures(size);

		for (const auto& signatures : signaturesLists)
		{
			for (const auto& signature : *signatures)
			{
				combinedSignatures.PushBack(signature);
			}
		}
		
		return combinedSignatures;
	}

	bool TypeManager::IsRegistered(const IdType typeId) const
	{
		return mRegistry.Find(typeId) != mRegistry.end();
	}

	void TypeManager::Register(const IdType typeId, SignatureListType&& signatures, const IdType parentTypeId)
	{
		if (parentTypeId != Attributed::TypeIdClass() && !mRegistry.ContainsKey(parentTypeId))
		{
			throw std::runtime_error("Parent type is not registered.");
		}

		auto [it, isNew] = mRegistry.Insert({ typeId, { std::move(signatures), parentTypeId } });

		if (!isNew)
		{
			throw std::runtime_error("Type registered more than once.");
		}
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
