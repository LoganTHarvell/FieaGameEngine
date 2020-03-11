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
	/// <typeparam name="T">Common type for classes that can be created by the static class.</typeparam>
	template<typename T>
	class Factory
	{
#pragma region Static Members
	public:
		static gsl::owner<T*> Create(const std::string& className);

	protected:
		static void Add(const Factory& factory);
		static bool Remove(const Factory& factory);

	private:
		static const Factory* const Find(const std::string& className);
	
	private:
		inline static HashMap<const std::string&, const Factory&> mRegistry;
#pragma endregion Static Members

#pragma region Virtual Interface
	private:
		virtual gsl::owner<T*> Create() const = 0;
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
		ConcreteProductType##Factory() { Factory<AbstractProductType>::Add(*this); }							\
		~ConcreteProductType##Factory() { Factory<AbstractProductType>::Remove(*this); }						\
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