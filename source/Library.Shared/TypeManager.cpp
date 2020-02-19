#include "pch.h"

#include "TypeManager.h"


namespace Library
{
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

	void TypeManager::Create()
	{
		if (!mInstance) mInstance = new TypeManager();
	}

	TypeManager* TypeManager::Instance()
	{
		return mInstance;
	}

	void TypeManager::Destroy()
	{
		if (mInstance) delete mInstance;
		mInstance = nullptr;
	}

	TypeManager::SignaturesType TypeManager::Signatures(const IdType typeId)
	{
		if (typeId == 0) return SignaturesType();

		const TypeInfo& typeInfo = mRegistry.At(typeId);
		SignaturesType combinedSignatures = Signatures(typeInfo.parentTypeId);	

		for (const auto& signature : typeInfo.signatures)
		{
			combinedSignatures.PushBack(signature);	
		}
		
		return combinedSignatures;
	}

	void TypeManager::Register(const IdType typeId, SignaturesType signatures, const IdType parentTypeId)
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

	void TypeManager::Register(const IdType typeId, SignaturesType&& signatures, const IdType parentTypeId)
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
}
