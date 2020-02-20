#pragma once

#pragma region Includes
// First Party
#include "RTTI.h"
#include "HashMap.h"
#include "Vector.h"
#include "Datum.h"
#pragma endregion Includes

namespace Library
{
#pragma region Signature Struct
	/// <summary>
	/// Represents signature information for instantiating an attribute for a Scope.
	/// </summary>
	struct Signature
	{
		std::string Name;
		Datum::Types Type;
		std::size_t Offset;
		bool IsInternal;
		std::size_t Size;

		bool operator==(const Signature& rhs) const noexcept;
	};
#pragma endregion Signature Struct

	/// <summary>
	/// Type manager singleton used for registering Attributed type signatures.
	/// </summary>
	class TypeManager final
	{
#pragma region Type Definitions
	public:
		/// <summary>
		/// Type identifiers used as keys for a type registry.
		/// </summary>
		using IdType = RTTI::IdType;

		/// <summary>
		/// List of signatures for each registered types attributes.
		/// </summary>
		using SignatureListType = Vector<Signature>;

		/// <summary>
		/// Data registered for each type.
		/// </summary>
		struct TypeInfo
		{
			SignatureListType signatures;
			const IdType parentTypeId;
		};

		/// <summary>
		/// Registry type for containing TypeInfo.
		/// </summary>
		using RegistryType = HashMap<IdType, TypeInfo>;
#pragma endregion Type Definitions

#pragma region Default Constructor/Destructor
	private:
		TypeManager() = default;
		~TypeManager() = default;
#pragma endregion Default Constructor/Destructor

#pragma region Deleted Copy/Move
	public:
		TypeManager(const TypeManager&) = delete;
		TypeManager(TypeManager&&) = delete;
		TypeManager& operator=(const TypeManager&) = delete;
		TypeManager& operator=(TypeManager&&) = delete;
#pragma endregion Deleted Copy/Move

#pragma region Instance Management
	public:
		/// <summary>
		/// Initializes the TypeManager instance, if necessary.
		/// </summary>
		static void Create();

		/// <summary>
		/// Deletes the TypeManager instance, if necessary.
		/// </summary>
		static void Destroy();

		/// <summary>
		/// Gets the TypeManager instance.
		/// </summary>
		/// <returns>TypeManager instance, if it is created. Otherwise, nullptr.</returns>
		static TypeManager* Instance();
#pragma endregion Instance Management

#pragma region Registry
	public:
		/// <summary>
		/// Gets the Attributed signatures for a given type.
		/// </summary>
		/// <param name="typeId">IdType for the Attributed type whose signatures will be retrieved.</param>
		/// <returns>Signatures for the given Attributed IdType.</returns>
		SignatureListType Signatures(const IdType typeId);

		/// <summary>
		/// Registers an Attributed type and the attribute signatures with the registry.
		/// </summary>
		/// <param name="typeId">IdType associated with the Attributed type.</param>
		/// <param name="signatures">List of attribute signature structs.</param>
		/// <param name="parentTypeId">IdType associated the parent of the Attributed type.</param>
		void Register(const IdType typeId, SignatureListType&& signatures, const IdType parentTypeId);

		/// <summary>
		/// Removes an Attributed type from the registry.
		/// </summary>
		/// <param name="typeId"></param>
		void Deregister(const IdType typeId);

		/// <summary>
		/// Removes all Attributed types from the registry.
		/// </summary>
		void Clear();
#pragma endregion Registry

#pragma region Data Members
	private:
		/// <summary>
		/// Pointer to a TypeManager instance managed by the singleton.
		/// </summary>
		inline static TypeManager* mInstance{ nullptr };
		
		/// <summary>
		/// Registry of IdType keys and TypeInfo values, containing a SignatureListType value and the parent IdType value.
		/// </summary>
		RegistryType mRegistry;
#pragma endregion Data Members
	};

/// <summary>
/// Macro that registers an Attributed type with the registry.
/// </summary>
#define REGISTER_TYPE(Type, ParentType) TypeManager::Register(Type::TypeIdClass(), Type::Signatures(), ParentType::TypeIdClass());
}