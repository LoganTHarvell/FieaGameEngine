#pragma once

#pragma region Includes
// Standard
#include <string>
#include <cstddef>

// Third Party
#include <gsl/gsl>
#pragma endregion Includes

namespace Library
{
	/// <summary>
	/// Abstract base class for accessing runtime type information.
	/// </summary>
	class RTTI
	{
	public:
		/// <summary>
		/// Type identifier.
		/// </summary>
		using IdType = std::size_t;

		/// <summary>
		/// Gets the type identifier associated with this class type.
		/// </summary>
		/// <returns>Type ID associated with RTTI.</returns>
		static IdType TypeIdClass() { return 0; }

		/// <summary>
		/// Virtual constructor.
		/// Creates a heap allocated instance of the true type.
		/// </summary>
		/// <returns>Pointer to the new instance of the true type as an RTTI.</returns>
		virtual gsl::owner<RTTI*> Create() const
		{
			return nullptr;
		}

		/// <summary>
		/// Virtual constructor for specifying a return type.
		/// </summary>
		template<typename T>
		gsl::owner<T*> CreateAs()
		{
			RTTI* created = Create();
			return created ? created->As<T>() : nullptr;
		}

		/// <summary>
		/// Virtual destructor.
		/// </summary>
		virtual ~RTTI() = default;

		/// <summary>
		/// Gets the type identifier associated with the true class type of an instance.
		/// </summary>
		/// <returns>Type ID associated with the true class type of an instance.</returns>
		virtual RTTI::IdType TypeIdInstance() const = 0;

		/// <summary>
		/// Gets the current instance cast as an RTTI pointer.
		/// </summary>
		/// <returns>If the given type ID matches the instance type ID, a pointer to the instance as an RTTI. Otherwise, nullptr.</returns>
		virtual RTTI* QueryInterface(const IdType)
		{
			return nullptr;
		}

		/// <summary>
		/// Validates that an instance is related to the type associate with the given IdType value.
		/// </summary>
		/// <returns>True, if the given type ID matches with the type ID of this class or any related base/derived class.</returns>
		virtual bool Is(IdType) const
		{
			return false;
		}

		/// <summary>
		/// Gets the instance cast as a related class type.
		/// </summary>
		/// <typeparam name="T">Type to which this instance is cast.</param>
		/// <returns>If the type is related, a pointer to the instance as the given type. Otherwise, nullptr.</returns>
		template <typename T>
		T* As()
		{
			return (Is(T::TypeIdClass()) ? reinterpret_cast<T*>(const_cast<RTTI*>(this)) : nullptr);
		}

		/// <summary>
		/// Gets the instance cast as a related class type.
		/// </summary>
		/// <typeparam name="T">Type to which this instance is cast.</param>
		/// <returns>If the type is related, a pointer to the instance as the given type. Otherwise, nullptr.</returns>
		template <typename T>
		const T* As() const
		{
			return (Is(T::TypeIdClass()) ? reinterpret_cast<const T*>(this) : nullptr);
		}

		/// <summary>
		/// Gets a string representation for the given instance.
		/// </summary>
		/// <returns></returns>
		virtual std::string ToString() const
		{
			return "RTTI";
		}

		/// <summary>
		/// Equality method for comparing two RTTI derived class instances.
		/// </summary>
		/// <param name="rhs">An RTTI derived class to be compared against.</param>
		/// <returns>True, if the two RTTI derived class instances are equivalent. Otherwise, false.</returns>
		virtual bool Equals(const RTTI* rhs) const
		{
			return this == rhs;
		}
	};

#pragma region RTTI Declarations Convenience Macro
/// <summary>
/// RTTI overrides macro which simplifies RTTI derived class declarations. Should be called in derived class declarations.
/// </summary>
/// <param name="Type">Type of an RTTI derived class.</param>
/// <param name="ParentType">Type of the direct parent class from which the given Type is derived.</param>
#define RTTI_DECLARATIONS(Type, ParentType)																								\
	public:																																\
		virtual gsl::owner<Library::RTTI*> Create() const override { return new Type(); }												\
		RTTI_DECLARATIONS_ABSTRACT(Type, ParentType)

/// <summary>
/// RTTI overrides macro which simplifies RTTI derived abstract class declarations. Should be called in derived class declarations.
/// </summary>
/// <param name="Type">Type of an RTTI derived class.</param>
/// <param name="ParentType">Type of the direct parent class from which the given Type is derived.</param>
#define RTTI_DECLARATIONS_ABSTRACT(Type, ParentType)																					\
	public:																																\
		using Base = ParentType;																										\
																																		\
		static Library::RTTI::IdType TypeIdClass() { return sRunTimeTypeId; }															\
																																		\
		virtual Library::RTTI::IdType TypeIdInstance() const override { return TypeIdClass(); }											\
																																		\
		virtual Library::RTTI* QueryInterface(const Library::RTTI::IdType id) override													\
        {																																\
			return (id == sRunTimeTypeId ? reinterpret_cast<Library::RTTI*>(this) : ParentType::QueryInterface(id));					\
        }																																\
																																		\
		virtual bool Is(Library::RTTI::IdType id) const override																		\
		{																																\
			return (id == sRunTimeTypeId ? true : ParentType::Is(id));																	\
		}																																\
																																		\
		private:																														\
			inline static const Library::RTTI::IdType sRunTimeTypeId = reinterpret_cast<Library::RTTI::IdType>(&Type::TypeIdClass);
#pragma endregion RTTI Declarations Convenience Macro
}