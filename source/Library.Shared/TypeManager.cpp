#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "TypeManager.h"
#pragma endregion Includes

namespace Library
{
#pragma region Signature Struct
	bool Signature::operator==(const Signature& rhs) const noexcept
	{
		bool isEqual = true;

		if (isEqual) isEqual = Name == rhs.Name;
		if (isEqual) isEqual = Type == rhs.Type;
		if (isEqual) isEqual = Offset == rhs.Offset;
		if (isEqual) isEqual = IsInternal == rhs.IsInternal;
		if (isEqual) isEqual = Size == rhs.Size;

		return false;
	}
#pragma endregion Signature Struct

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

		while (id != 0)
		{
			auto it = mRegistry.Find(id);
			assert(it != mRegistry.end());
			
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

	void TypeManager::Register(const IdType typeId, SignatureListType&& signatures, const IdType parentTypeId)
	{
		if (!mRegistry.ContainsKey(parentTypeId))
		{
			throw std::runtime_error("Parent type is not registered.");
		}

		auto [it, containsKey] = mRegistry.Insert({ typeId, { std::move(signatures), parentTypeId } });

		if (containsKey)
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
