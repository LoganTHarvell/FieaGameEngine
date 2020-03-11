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
#pragma region Signature Type
	/// <summary>
	/// Represents signature information for instantiating an attribute for a Scope.
	/// </summary>
	struct Signature
	{
		std::string Name;
		Datum::Types Type;
		bool HasInternalStorage;
		std::size_t Size;
		std::size_t Offset;

		bool operator==(const Signature& rhs) const noexcept = default;
	};

	/// <summary>
	/// List of signatures for each registered types attributes.
	/// </summary>
	using SignatureListType = Vector<Signature>;
#pragma endregion Signature Type

	/// <summary>
	/// Manager singleton that maintains a registry for TypeInfo of Attributed subclasses.
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
		/// Data registered for each type.
		/// </summary>
		struct TypeInfo
		{
			const SignatureListType signatures;
			const IdType parentTypeId;
		};

		/// <summary>
		/// Registry type for containing TypeInfo.
		/// </summary>
		using Registry = HashMap<IdType, const TypeInfo&>;
#pragma endregion Type Definitions

#pragma region Special Members
	private:
		TypeManager() = default;
		~TypeManager() = default;

	public:
		TypeManager(const TypeManager&) = delete;
		TypeManager(TypeManager&&) = delete;
		TypeManager& operator=(const TypeManager&) = delete;
		TypeManager& operator=(TypeManager&&) = delete;
#pragma endregion Special Members

#pragma region Instance Management
	public:
		/// <summary>
		/// Initializes the TypeManager instance, if necessary.
		/// </summary>
		/// <param name="capacity">Initial capacity for the registry, not a max size.</param>
		static void Create(const std::size_t capacity=Registry::DefaultBucketCount);

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
		/// Finds the Attributed signatures for a given type.
		/// </summary>
		/// <param name="typeId">IdType for the Attributed derived class whose signatures will be retrieved.</param>
		/// <returns>Pointer to the TypeInfo of the given Attributed IdType.</returns>
		const TypeInfo* Find(const IdType typeId) const;

		/// <summary>
		/// Checks if a given Attributed derived class has been registered.
		/// </summary>
		/// <param name="typeId">IdType for the Attributed derived class whose signatures will be retrieved.</param>
		/// <returns>True, if the Attributed type is registered. Otherwise, false.</returns>
		bool IsRegistered(const IdType typeId) const;

		/// <summary>
		/// Adds TypeInfo for the given Attributed derived class type to the registry.
		/// </summary>
		/// <typeparam name="T">Typename of an Attribute derived class to be registered.</typeparam>
		/// <exception cref="std::runtime_error">Parent type is not registered.</exception>
		/// <exception cref="std::runtime_error">Type registered more than once.</exception>
		template<typename T>
		void Register();

		/// <summary>
		/// Removes an Attributed derived class from the registry.
		/// </summary>
		/// <param name="typeId">IdType for the Attributed derived class whose signatures will be removed.</param>
		/// <remarks>Do not remove parent classes if the child class needs access to the registry.</remarks>
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
		Registry mRegistry;
#pragma endregion Data Members
	};

#pragma region Global Registry Function
	/// <summary>
	/// Adds TypeInfo for the given Attributed derived class type to the TypeManager instance.
	/// </summary>
	/// <typeparam name="T">Typename of an Attribute derived class to be registered.</typeparam>
	/// <exception cref="std::runtime_error">TypeManager instance null.</exception>
	template<typename T>
	void RegisterType();
#pragma endregion Global Registry Function
}

#include "TypeManager.inl"