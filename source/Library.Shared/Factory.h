#pragma once

#pragma region Includes
// Standard
#include <string>

// Third Party
#include <gsl/gsl>

// First Party
#include "HashMap.h"
#include "Utility.h"
#pragma endregion Includes

namespace Library
{
	/// <summary>
	/// Represents both a Factory virtual interface and a Factory manager class.
	/// </summary>
	/// <typeparam name="T">Common base class type for classes that can be created by the static class.</typeparam>
	template<typename T>
	class Factory
	{
#pragma region Type Definitions
	public:
		/// <summary>
		/// Typed definition for registries of class names to factories that create the classes.
		/// </summary>
		using Registry = HashMap<const std::string&, const Factory&>;
#pragma endregion Type Definitions

#pragma region Static Members
	public:
		/// <summary>
		/// Creates an instance of a derived class registered with the Factory of a base class type.
		/// </summary>
		/// <param name="className">Name of the class to be instantiated.</param>
		/// <returns>
		/// Owning pointer to a newly heap allocated derived class.
		/// Returned as the base class type T associated with the static Factory class.
		/// </returns>
		static gsl::owner<T*> Create(const std::string& className);

		/// <summary>
		/// Checks whether a class name is registered to a factory instance.
		/// </summary>
		/// <param name="className">Class name to be checked in the registry.</param>
		/// <returns>True if the class name is registered. Otherwise, false.</returns>
		static bool IsRegistered(const std::string& className);

		/// <summary>
		/// Gets the current load factor of the factories registry.
		/// </summary>
		/// <returns>Float representing the load factor of the factories registry.</returns>
		static float RegistryLoadFactor();

		/// <summary>
		/// Rehashes the factories registry with the specified size.
		/// </summary>
		/// <param name="size">Bucket count for the rehashed registry.</param>
		/// <remarks>
		/// Best called before factories are instantiated to a size larger than the number of expected registered factories.
		/// Registry size is not equivalent to the max number of factories that can be registered.
		/// </remarks>
		static void RegistryRehash(const std::size_t size);

	protected:
		/// <summary>
		/// Registers a derived factory to the static Factory class registry.
		/// </summary>
		/// <param name="factory">Derived factory class instance to be added to the static class registry.</param>
		static void Register(const Factory& factory);

		/// <summary>
		/// Removes a derived factory from the static Factory class registry.
		/// </summary>
		/// <param name="factory">Derived factory class instance to be removed from the static class registry.</param>
		static bool Deregister(const Factory& factory);
	
	private:
		/// <summary>
		/// Mapping of class names to factories that can create the class instances.
		/// </summary>
		inline static Registry sRegistry;
#pragma endregion Static Members

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Factory() = default;

		/// <summary>
		/// Default destructor.
		/// </summary>
		virtual ~Factory() = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		Factory(const Factory&) = delete;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		Factory& operator=(const Factory&) = delete;

		/// <summary>
		/// Move constructor.
		/// </summary>
		Factory(Factory&&) = delete;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		Factory& operator=(Factory&&) = delete;
#pragma endregion Special Members

#pragma region Virtual Interface
	private:
		/// <summary>
		/// Virtual constructor for the type T associated with a Factory derived class.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated instance of the type T associated with the derived Factory instance.</returns>
		virtual gsl::owner<T*> Create() const = 0;

		/// <summary>
		/// Name of the class of type T associated with the Factory derived class.
		/// </summary>
		/// <returns>Reference to a constant string representing the class name.</returns>
		virtual const std::string& ClassName() const = 0;
#pragma endregion Virtual Interface
	};

#pragma region Concrete Factory Convenience Macro
/// <summary>
/// Declares and defines a factory class that can be used to create instances of a type given the class name.
/// </summary>
/// <param name="ConcreteProductType">Type which the factory will create.</param>
/// <param name="AbstractProductType">Factory type which the derived Factory instance is registered.</param>
#define ConcreteFactory(ConcreteProductType, AbstractProductType)														\
	class ConcreteProductType##Factory final : public Library::Factory<AbstractProductType>								\
	{																													\
	public:																												\
		ConcreteProductType##Factory() { Library::Factory<AbstractProductType>::Register(*this); }						\
		virtual ~ConcreteProductType##Factory() override { Library::Factory<AbstractProductType>::Deregister(*this); }	\
																														\
		ConcreteProductType##Factory(const ConcreteProductType##Factory&) = delete;										\
		ConcreteProductType##Factory& operator=(const ConcreteProductType##Factory&) = delete;							\
		ConcreteProductType##Factory(ConcreteProductType##Factory&&) noexcept = delete;									\
		ConcreteProductType##Factory& operator=(ConcreteProductType##Factory&&) noexcept = delete;						\
																														\
	private:																											\
		virtual gsl::owner<ConcreteProductType*> Create() const override { return new ConcreteProductType(); }			\
		virtual const std::string& ClassName() const override { return mClassName; }									\
																														\
	private:																											\
		const std::string mClassName{ #ConcreteProductType };															\
	};	
#pragma endregion Concrete Factory Convenience Macro											
}

#include "Factory.inl"