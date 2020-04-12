#pragma once

#pragma region Includes
// First Party
#include "RTTI.h"
#include "TypeManager.h"
#include "Scope.h"
#pragma endregion Includes

namespace Library
{
	/// <summary>
	/// Abstract base for derived classes with mirrored attributes for each of the members in its data structure. This requires the registration with
	/// </summary>
	/// <remarks>
	/// In order to populate, any derived class must be registered with a TypeManager. To do so, each derived 
	/// class must implement a TypeInfo method that returns a const TypeInfo reference to data that will not
	/// change after registration.
	/// </remarks>
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS_ABSTRACT(Attributed, Scope)

#pragma region Constructors, Destructor, Assignment
	public:
		/// <summary>
		/// Main constructor.
		/// Populates the scope with attributes associated with the passed RTTI::IdType.
		/// </summary>
		/// <param name="typeId">RTTI::IdType to use during Populate.</param>
		explicit Attributed(const RTTI::IdType typeId);
		
		/// <summary>
		/// Default constructor.
		/// </summary>
		Attributed() = delete;

		/// <summary>
		/// Virtual default destructor.
		/// </summary>
		virtual ~Attributed() override = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">Right hand side Attributed class to be copied.</param>
		Attributed(const Attributed& rhs);

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">Right hand side Attributed class to be copied.</param>
		/// <returns>Reference to the newly modified instance.</returns>
		Attributed& operator=(const Attributed& rhs);

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">Right hand side Attributed class to be moved.</param>
		Attributed(Attributed&& rhs) noexcept;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">Right hand side Attributed class to be moved.</param>
		/// <returns>Reference to the newly modified Attributed instance.</returns>
		/// <remarks>The right hand side Attributed instance is now completely empty.</remarks>
		Attributed& operator=(Attributed&& rhs) noexcept;
#pragma endregion Constructors, Destructor, Assignment
		
#pragma region Pure Virtual Constructor
	public:
		/// <summary>
		/// Pure virtual method signature for the Clone method inherited from Scope.
		/// Creates an instance copy of a Scope derived class.
		/// </summary>
		/// <returns>Pointer to a newly heap allocated copy of the instance.</returns>
		virtual gsl::owner<Scope*> Clone() const override = 0;
#pragma endregion Pure Virtual Constructor

#pragma region Boolean Operators
	public:
		/// <summary>
		/// Equals operator. 
		/// Checks if the size of the Scope and the data values are equal to the right hand side Scope.
		/// </summary>
		/// <param name="rhs">Scope on the right hand side to be compared to the left.</param>
		/// <returns>True when Scope values are equivalent, otherwise false.</returns>
		bool operator==(const Attributed& rhs) const noexcept;

		/// <summary>
		/// Not equal operator. 
		/// Checks if the size of the Scope and the data values are not equal to the right hand side Scope.
		/// </summary>
		/// <param name="rhs">Scope on the right hand side to be compared to the left.</param>
		/// <returns>True when Scope values are not equivalent, otherwise false.</returns>
		bool operator!=(const Attributed& rhs) const noexcept;
#pragma endregion Boolean Operators

#pragma region Accessors
	public:
		/// <summary>
		/// Checks if an Attribute is associated with the given name.
		/// </summary>
		/// <param name="key">KeyType value to search for in the instance.</param>
		/// <returns>True if associated with an Attribute. Otherwise, false.</returns>
		bool IsAttribute(const KeyType& key);

		/// <summary>
		/// Checks if a prescribed Attribute is associated with the given name.
		/// </summary>
		/// <param name="key">KeyType value to search for in the instance.</param>
		/// <returns>True if associated with an Attribute. Otherwise, false.</returns>
		bool IsPrescribedAttribute(const KeyType& key);

		/// <summary>
		/// Checks if an auxiliary Attribute is associated with the given name.
		/// </summary>
		/// <param name="key">KeyType value to search for in the instance.</param>
		/// <returns>True if associated with an Attribute. Otherwise, false.</returns>
		bool IsAuxiliaryAttribute(const KeyType& key);

		/// <summary>
		/// Gets the list of prescribed Attribute values.
		/// </summary>
		/// <returns>List of pointers to all prescribed Attribute values.</returns>
		void ForEachPrescribed(std::function<void(Attribute&)> functor);

		/// <summary>
		/// Gets the list of prescribed Attribute values.
		/// </summary>
		/// <returns>List of pointers to all prescribed Attribute values.</returns>
		void ForEachPrescribed(std::function<void(const Attribute&)> functor) const;

		/// <summary>
		/// Gets the list of auxiliary Attribute values.
		/// </summary>
		/// <returns>List of pointers to all auxiliary Attribute values.</returns>
		void ForEachAuxiliary(std::function<void(Attribute&)> functor);

		/// <summary>
		/// Gets the list of prescribed Attribute values.
		/// </summary>
		/// <returns>List of pointers to all auxiliary Attribute values.</returns>
		void ForEachAuxiliary(std::function<void(const Attribute&)> functor) const;
#pragma endregion Accessors
		
#pragma region Modifiers
		/// <summary>
		/// Appends an empty DataType value to the instance as an auxiliary Attribute.
		/// </summary>
		/// <param name="key">KeyType value for the new Attribute to append.</param>
		/// <returns>Reference to the DataType value associated with the name.</returns>
		DataType& AppendAuxiliaryAttribute(const KeyType& key);
#pragma endregion Modifiers

#pragma region RTTI Overrides
	public:
		/// <summary>
		/// RTTI override that returns a string representation of the Attributed class.
		/// </summary>
		/// <returns>String representation of the Attributed class.</returns>
		virtual std::string ToString() const override;

		/// <summary>
		/// RTTI override that determines if the Attributed instance is equal to a given RTTI derived class instance.
		/// </summary>
		/// <param name="rhs">RTTI derived class instance to be compared against.</param>
		/// <returns>True if the Attributed instance is equivalent to the given RTTI derived class instance.</returns>
		bool Equals(const RTTI* rhs) const;
#pragma endregion RTTI Overrides

#pragma region Helper Methods
	private:
		/// <summary>
		/// Helper method for populating the Scope attributes during construction.
		/// Recursively called to append any parent attributes registered.
		/// </summary>
		/// <param name="typeInfo">TypeInfo associated with the Attributed class.</param>
		void Populate(const TypeManager::TypeInfo* typeInfo);

		/// <summary>
		/// Updates storage for all DataType values in the Scope with external storage.
		/// Recursively called to update any parent attributes registered.
		/// </summary>
		/// <param name="typeInfo">TypeInfo associated with the Attributed class.</param>
		/// <returns>Next prescribed Attribute index to be updated, used by recursive calls.</returns>
		std::size_t UpdateExternalStorage(const TypeManager::TypeInfo* typeInfo);
#pragma endregion Helper Methods

#pragma region Data Members
	private:
		/// <summary>
		/// Number of prescribed Attributes.
		/// </summary>
		inline static std::size_t mNumPrescribed;
#pragma endregion Data Members
	};
}