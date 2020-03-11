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
		/// Finds a factory reference registered with the static class that creates class with the given class name.
		/// </summary>
		/// <param name="className">Name of the class whose factory should be found.</param>
		/// <returns>If found, a pointer to the factory that can create the class with the given class name. Otherwise, false.</returns>
		static const Factory* const Find(const std::string& className);
	
	private:
		/// <summary>
		/// Mapping of class names to factories that can create the class instances.
		/// </summary>
		inline static HashMap<const std::string&, const Factory&> mRegistry;
#pragma endregion Static Members

#pragma region Virtual Interface
	private:
		/// <summary>
		/// Virtual constructor for the type T associated with a Factory derived class.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated instance of the type T assoicated with the derived Factory instance.</returns>
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
#define ConcreteFactory(ConcreteProductType, AbstractProductType)												\
	class ConcreteProductType##Factory final : public Factory<AbstractProductType>								\
	{																											\
	public:																										\
		ConcreteProductType##Factory() { Factory<AbstractProductType>::Register(*this); }							\
		~ConcreteProductType##Factory() { Factory<AbstractProductType>::Deregister(*this); }						\
																												\
		ConcreteProductType##Factory(const ConcreteProductType##Factory&) = delete;								\
		ConcreteProductType##Factory& operator=(const ConcreteProductType##Factory&) = delete;					\
		ConcreteProductType##Factory(ConcreteProductType##Factory&&) noexcept = delete;							\
		ConcreteProductType##Factory& operator=(ConcreteProductType##Factory&&) noexcept = delete;				\
																												\
	private:																									\
		virtual gsl::owner<ConcreteProductType*> Create() const override { return new ConcreteProductType(); }	\
		virtual const std::string& ClassName() const override { return mClassName; }							\
																												\
	private:																									\
		const std::string mClassName{ #ConcreteProductType };													\
	};	
#pragma endregion Concrete Factory Convenience Macro											
}

#include "Factory.inl"