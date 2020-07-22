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
	/**
	 * @brief Abstract base class for accessing runtime type information
	*/
	class RTTI
	{
	public:
		using IdType = std::size_t;	//!< Type identifier

		/**
		 * @brief Gets the type identifier associated with this class type
		 * @returns Type ID associated with RTTI
		*/
		static IdType TypeIdClass() { return 0; }

		/**
		 * @brief Virtual constructor
		 *		  Creates a heap allocated instance of the true type.
		 * 
		 * @returns Pointer to the new instance of the true type as an RTTI
		*/
		virtual gsl::owner<RTTI*> Create() const
		{
			return nullptr;
		}

		/**
		 * @brief Virtual constructor for specifying a return type
		 * @tparam T Related type for the created instance to be returned as
		 * @return Pointer to the created instance as the given type
		*/
		template<typename T>
		gsl::owner<T*> CreateAs()
		{
			RTTI* created = Create();
			return created ? created->As<T>() : nullptr;
		}

		/**
		 * @brief Virtual destructor
		*/
		virtual ~RTTI() = default;

		/**
		 * @brief Gets the type identifier associated with the true class type of an instance
		 * 
		 * @returns Type ID associated with the true class type of an instance
		*/
		virtual IdType TypeIdInstance() const = 0;

		/**
		 * @brief Gets the current instance cast as an RTTI pointer.
		 * 
		 * @returns If the given type ID matches the instance type ID, a pointer to the instance as an RTTI. Otherwise, nullptr.
		*/
		virtual RTTI* QueryInterface(const IdType)
		{
			return nullptr;
		}

		/**
		 * @brief Validates that an instance is related to the type associate with the given IdType value.
		 * 
		 * @returns True, if the given type ID matches with the type ID of this class or any related base/derived class.
		*/
		virtual bool Is(IdType) const
		{
			return false;
		}

		/**
		 * @brief Gets the instance cast as a related class type.
		 *  
		 * @tparam T Type to which this instance is cast.
		 * @returns If the type is related, a pointer to the instance as the given type. Otherwise, nullptr.
		*/
		template <typename T>
		T* As()
		{
			return (Is(T::TypeIdClass()) ? static_cast<T*>(this) : nullptr);
		}

		/**
		 * @brief Gets the instance cast as a related class type.
		 *  
		 * @tparam T Type to which this instance is cast.
		 * @returns If the type is related, a pointer to the instance as the given type. Otherwise, nullptr.
		*/
		template <typename T>
		const T* As() const
		{
			return (Is(T::TypeIdClass()) ? static_cast<const T*>(this) : nullptr);
		}

		/**
		 * @brief Gets the instance cast as a related class type.
		 *  
		 * @tparam T Type to which this instance is cast.
		 * @returns If the type is related, a pointer to the instance as the given type. Otherwise, nullptr.
		*/
		template <typename T>
		T* AssertAs()
		{
			assert(Is(T::TypeIdClass()));
			return static_cast<T*>(this);
		}

		/**
		 * @brief Gets the instance cast as a related class type.
		 *  
		 * @tparam T Type to which this instance is cast.
		 * @returns If the type is related, a pointer to the instance as the given type. Otherwise, nullptr.
		*/
		template <typename T>
		const T* AssertAs() const
		{
			assert(Is(T::TypeIdClass()));
			return static_cast<T*>(this);
		}

		/**
		 * @brief Gets a string representation for the given instance.
		 *  
		 * @returns 
		*/
		virtual std::string ToString() const
		{
			return "RTTI";
		}

		/**
		 * @brief Equality method for comparing two RTTI derived class instances.
		 *  
		 * @param rhs An RTTI derived class to be compared against.
		 * @returns True, if the two RTTI derived class instances are equivalent. Otherwise, false.
		*/
		virtual bool Equals(const RTTI* rhs) const
		{
			return this == rhs;
		}
	};

#pragma region RTTI Declarations Convenience Macro
/**
 *  RTTI overrides macro which simplifies RTTI derived class declarations. Should be called in derived class declarations.
 *  
 * @param Type Type of an RTTI derived class.
 * @param ParentType Type of the direct parent class from which the given Type is derived.
*/
#define RTTI_DECLARATIONS(Type, ParentType)																								\
	public:																																\
		virtual gsl::owner<Library::RTTI*> Create() const override { return new Type(); }												\
		RTTI_DECLARATIONS_ABSTRACT(Type, ParentType)

/**
 *  RTTI overrides macro which simplifies RTTI derived abstract class declarations. Should be called in derived class declarations.
 *  
 * @param Type Type of an RTTI derived class.
 * @param ParentType Type of the direct parent class from which the given Type is derived.
*/
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