#pragma once


#include "RTTI.h"
#include "HashMap.h"
#include "Vector.h"
#include "Datum.h"


namespace Library
{
	struct Signature
	{
		std::string Name;
		Datum::Types Type;
		std::size_t Offset;
		bool IsInternal;
		std::size_t Size;

		bool operator==(const Signature& rhs) const noexcept;
	};

	class TypeManager final
	{
	public:
		using IdType = RTTI::IdType;
		using SignaturesType = Vector<Signature>;

		struct TypeInfo
		{
			SignaturesType signatures;
			const IdType parentTypeId;
		};

		using RegistryType = HashMap<IdType, TypeInfo>;

	private:
		TypeManager() = default;
		~TypeManager() = default;

	public:
		TypeManager(const TypeManager&) = delete;
		TypeManager(TypeManager&&) = delete;
		TypeManager& operator=(const TypeManager&) = delete;
		TypeManager& operator=(TypeManager&&) = delete;

	public:
		static void Create();
		static void Destroy();

		static TypeManager* Instance();

	public:
		SignaturesType Signatures(const IdType typeId);

		void Register(const IdType typeId, SignaturesType signatures, const IdType parentTypeId);
		void Register(const IdType typeId, SignaturesType&& signatures, const IdType parentTypeId);
		void Deregister(const IdType typeId);
		void Clear();

	private:
		inline static TypeManager* mInstance{ nullptr };
		RegistryType mRegistry;
	};

#define REGISTER_TYPE(Type, ParentType) TypeManager::Register(Type::TypeIdClass(), Type::Signatures(), ParentType::TypeIdClass());
}