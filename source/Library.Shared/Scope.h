#pragma once

#pragma region Includes
// Standard
#include <string>

// Third Party
#include <gsl/gsl>

// First Party
#include "RTTI.h"
#include "HashMap.h"
#include "Datum.h"
#include "Vector.h"
#include "SList.h"
#pragma endregion Includes

namespace Library
{
	/// <summary>
	/// Represents an ordered table of key and data value pairs.
	/// </summary>
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI)

#pragma region Type Definitions and Constants
	public:
		/// <summary>
		/// Attribute key type.
		/// </summary>
		using Key = std::string;

		/// <summary>
		/// Attribute data type.
		/// </summary>
		using Data = Datum;

		/// <summary>
		/// Types supported by Data values.
		/// </summary>
		using Types = Data::Types;

		/// <summary>
		/// Table type defining a mapping names to data values.
		/// </summary>
		using Table = HashMap<Key, Data>;

		/// <summary>
		/// Attribute type defining a key and data value pair.
		/// </summary>
		using Attribute = Table::Pair;
#pragma endregion Type Definitions and Constants

#pragma region Constructors, Destructor, Assignment
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param key="capacity">Initial capacity of the Scope.</param>
		explicit Scope(const std::size_t capacity=0);

		/// <summary>
		/// Default destructor.
		/// </summary>
		virtual ~Scope() override;

		/// <summary>
		/// Copy constructor.
		/// Takes in a Scope as a parameter, then copies the data values to the constructed Scope.
		/// </summary>
		/// <param name="rhs">Scope to be copied.</param>
		Scope(const Scope& rhs);

		/// <summary>
		/// Copy assignment operator.
		/// Copies the data values from the right hand side (rhs) value to the left hand side.
		/// </summary>
		/// <param name="rhs">Scope whose values are copied.</param>
		/// <returns>Modified Scope with copied values.</returns>
		Scope& operator=(const Scope& rhs);

		/// <summary>
		/// Move constructor.
		/// Takes a Scope as a parameter and moves the data to the constructed Scope.
		/// </summary>
		/// <param name="rhs">Scope to be moved.</param>
		/// <remarks>Cannot call on a stack allocated Scope when moving from a nested Scope.</remarks>
		Scope(Scope&& rhs) noexcept;

		/// <summary>
		/// Move assignment operator.
		/// Moves the data values from the right hand side (rhs) value to the left hand side.
		/// </summary>
		/// <param name="rhs">Scope whose values are copied.</param>
		/// <returns>Modified Scope with copied values.</returns>
		/// <remarks>Cannot call on a stack allocated Scope when moving from a nested Scope.</remarks>
		Scope& operator=(Scope&& rhs) noexcept;

		/// <summary>
		/// Initializer list constructor.
		/// </summary>
		/// <param name="rhs">List of Pair values for insertion.</param>
		/// <param name="capacity">Capacity to initialize for the Scope.</param>
		/// <exception cref="std::runtime_error">Duplicate names found in the initializer list.</exception>
		/// <remarks>Data values are copied.</remarks>
		Scope(std::initializer_list<Attribute> rhs, const std::size_t capacity=0);

		/// <summary>
		/// Initializer list assignment operator.
		/// </summary>
		/// <param name="rhs">List of values to be in the Scope.</param>
		/// <returns>Reference to the modified Scope containing the new pairs.</returns>
		/// <exception cref="std::runtime_error">Duplicate names found in the initializer list.</exception>
		/// <remarks>Data values are copied.</remarks>
		Scope& operator=(std::initializer_list<Attribute> rhs);

		/// <summary>
		/// Creates a heap allocated copy of the current Scope.
		/// </summary>
		/// <returns>Pointer to a newly heap allocated Scope copy.</returns>
		/// <remarks>Override in derived classes to support copy construction and assignment as a nested Scope.</remarks>
		virtual gsl::owner<Scope*> Clone() const;
#pragma endregion Constructors, Destructor, Assignment

#pragma region Boolean Operators
	public:
		/// <summary>
		/// Equals operator. 
		/// Checks if the size of the Scope and the data values are equal to the right hand side Scope.
		/// </summary>
		/// <param name="rhs">Scope on the right hand side to be compared to the left.</param>
		/// <returns>True when Scope values are equivalent, otherwise false.</returns>
		bool operator==(const Scope& rhs) const noexcept;

		/// <summary>
		/// Not equal operator. 
		/// Checks if the size of the Scope and the data values are not equal to the right hand side Scope.
		/// </summary>
		/// <param name="rhs">Scope on the right hand side to be compared to the left.</param>
		/// <returns>True when Scope values are not equivalent, otherwise false.</returns>
		bool operator!=(const Scope& rhs) const noexcept;
#pragma endregion Boolean Operators

#pragma region Size and Capacity
		/// <summary>
		/// Getter method for the number of Attribute values in the Scope.
		/// </summary>
		/// <returns>Number of Table Entry values in the Scope.</returns>
		std::size_t Size() const;

		/// <summary>
		/// Checks if the size of the Scope is greater than zero, indicating it is non-empty.
		/// </summary>
		/// <returns>True if the Scope contains no elements, otherwise false.</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Gets the number of elements for which memory is already allocated. 
		/// </summary>
		/// <returns>Number of elements for which memory is already allocated.</returns>
		/// <remarks>This is not equivalent to the max size. It is the max size before new memory will be allocated.</remarks>
		std::size_t Capacity() const;

		/// <summary>
		/// Reserves memory for a given number of elements.
		/// </summary>
		/// <param name="capacity">New capacity for the Scope.</param>
		void Reserve(const std::size_t capacity);

		/// <summary>
		/// Reduces the capacity of the Scope to fit the content size.
		/// </summary>
		/// <remarks>
		/// For efficiency this should only be called once size will no longer increase. 
		/// Otherwise, a Reserve call should be made to specify the new desired capacity.
		/// </remarks>
		void ShrinkToFit();
#pragma endregion Size and Capacity

#pragma region Accessors
	public:
		/// <summary>
		/// Gets a pointer to the parent Scope, if it exists.
		/// </summary>
		/// <returns>If a child, the pointer to the parent Scope. Otherwise, nullptr.</returns>
		virtual Scope* GetParent() const;

		/// <summary>
		/// Checks if the Scope instance is a parent of the given Scope or any of its parents.
		/// </summary>
		/// <param name="scope">Child scope to be checked against.</param>
		/// <returns>True, if the Scope instance is an ancestor of the given Scope. Otherwise, false.</returns>
		bool IsAncestorOf(const Scope& scope) const;

		/// <summary>
		/// Checks if the given Scope is the parent of the Scope instance or any of its parents.
		/// </summary>
		/// <param name="scope">Parent scope to be checked against.</param>
		/// <returns>True, if the given Scope is an ancestor of the Scope instance. Otherwise, false.</returns>
		bool IsDescendentOf(const Scope& scope) const;

		/// <summary>
		/// Subscript operator.
		/// Gets a reference to the Data value of the given Key value.
		/// Appends a new default Data value if one does not already exist.
		/// </summary>
		/// <param name="key">Key value for the Attribute to be accessed or appended.</param>
		/// <returns>Reference to the Data value of the Attribute with the given Key value.</returns>
		Data& operator[](const Key& key);

		/// <summary>
		/// Subscript operator.
		/// Gets a reference to a constant Data value of the given Key value.
		/// </summary>
		/// <param name="key">Key value for the Attribute to be accessed.</param>
		/// <returns>Reference to the constant Data value of the Attribute with the given Key value.</returns>
		/// <exception cref="std::runtime_error">Key value not found.</exception>
		const Data& operator[](const Key& key) const;

		/// <summary>
		/// Subscript operator.
		/// Gets a reference to the Data value at the given index.
		/// </summary>
		/// <param name="index">Index for the Attribute to be accessed.</param>
		/// <returns>Reference to the Data value of the Attribute at the given index.</returns>
		Data& operator[](const std::size_t index);

		/// <summary>
		/// Subscript operator.
		/// Gets a reference to the constant Data value at the given index.
		/// </summary>
		/// <param name="index">Index for the Attribute to be accessed.</param>
		/// <returns>Reference to the constant Data value of the Attribute at the given index.</returns>
		const Data& operator[](const std::size_t index) const;

		/// <summary>
		/// Finds the Data value associated with the given Key value, if it exists.
		/// </summary>
		/// <param name="key">Key value associated with the Data value to be found.</param>
		/// <returns>If found, a pointer to the Data value. Otherwise, nullptr.</returns>
		virtual Data* Find(const Key& key);

		/// <summary>
		/// Finds the Data value associated with the given Key value, if it exists.
		/// </summary>
		/// <param name="key">Attribute key associated with the data to be found.</param>
		/// <returns>If found, a pointer to the Data value. Otherwise, nullptr.</returns>
		const Data* Find(const Key& key) const;

		/// <summary>
		/// Gets a pointer to the constant Key value for the Attribute at the given index, if it exists.
		/// </summary>
		/// <param name="index">Index of the Attribute with the key to be found.</param>
		/// <returns> If found, pointer to the constant Key& value. Otherwise, nullptr.</returns>
		const Key* FindName(const std::size_t index) const;

		/// <summary>
		/// Gets the Data value and index of a given Scope.
		/// </summary>
		/// <param name="scope">Scope to be found.</param>
		/// <returns>If found, a pair of a pointer to the Data value and the index within. Otherwise, nullptr and 0.</returns>
		std::pair<Data*, std::size_t> FindScope(const Scope& scope);

		/// <summary>
		/// Gets the Data value and index of a given Scope.
		/// </summary>
		/// <param name="scope">Scope to be found.</param>
		/// <returns>If found, a pair of a pointer to the Data value and the index within. Otherwise, nullptr and 0.</returns>
		std::pair<const Data*, std::size_t> FindScope(const Scope& scope) const;

		/// <summary>
		/// Performs a breadth-first search on the scope and its ancestors for a Attribute with a matching Key value.
		/// </summary>
		/// <param name="key">Key value of the Attribute to be found.</param>
		/// <param name="scopePtrOut">Output parameter that points to the Scope which owns the found Attribute.</param>
		/// <returns>If found, a pointer to the Data value of the Attribute. Otherwise, nullptr.</returns>
		Data* Search(const Key& key, Scope** scopePtrOut=nullptr);

		/// <summary>
		/// Performs a breadth-first search on the scope and its ancestors for a Attribute with a matching Key value.
		/// </summary>
		/// <param name="key">Key value of the Attribute to be found.</param>
		/// <param name="scopePtrOut">Output parameter that points to the Scope which owns the found Attribute.</param>
		/// <returns>If found, a pointer to the Data value of the Attribute. Otherwise, nullptr.</returns>
		const Data* Search(const Key& key, const Scope** scopePtrOut=nullptr) const;

		/// <summary>
		/// Performs a breadth-first search on the scope and its children for a Attribute with a matching Key value.
		/// </summary>
		/// <param name="key">Key value of the Attribute to be found.</param>
		/// <param name="scopePtrOut">Output parameter that points to the Scope which owns the found Attribute.</param>
		/// <returns>If found, a pointer to the Data value of the Attribute. Otherwise, nullptr.</returns>
		Data* SearchChildren(const Key& key, Scope** scopePtrOut=nullptr);

		/// <summary>
		/// Performs a breadth-first search on the scope and its children for a Attribute with a matching Key value.
		/// </summary>
		/// <param name="key">Key value of the Attribute to be found.</param>
		/// <param name="scopePtrOut">Output parameter that points to the Scope which owns the found Attribute.</param>
		/// <returns>If found, a pointer to the Data value of the Attribute. Otherwise, nullptr.</returns>
		const Data* SearchChildren(const Key& key, const Scope** scopePtrOut=nullptr) const;

		/// <summary>
		/// Performs an action iteratively on each Attribute in the Scope.
		/// </summary>
		/// <param name="functor">Function object to be called on each Attribute.</param>
		void ForEachAttribute(const std::function<void(Attribute&)>& functor);

		/// <summary>
		/// Performs an action iteratively on each Attribute in the Scope.
		/// </summary>
		/// <param name="functor">Function object to be called on each Attribute.</param>
		void ForEachAttribute(const std::function<void(const Attribute&)>& functor) const;
#pragma endregion Accessors

#pragma region Modifiers
	public:
		/// <summary>
		/// Appends a Attribute to the Scope with the given key and a default data value.
		/// </summary>
		/// <param name="key">Key value for the Attribute to be accessed or appended.</param>
		/// <returns>Reference to the Data value of the appended Attribute.</returns>
		/// <exception cref="std::runtime_error">Key value cannot be empty.</exception>
		Data& Append(const Key& key);

		/// <summary>
		/// Appends a Attribute to the Scope with the given key and a default Scope value, as a child.
		/// </summary>
		/// <param name="key">Key value for the Attribute to be accessed or appended with a child Scope.</param>
		/// <param name="capacity">New Scope capacity.</param>
		/// <returns>Reference to the Data value of the appended Attribute.</returns>
		/// <exception cref="std::runtime_error">Name cannot be empty.</exception>
		/// <exception cref="std::runtime_error">Table entry already exists with a non-Scope type.</exception>
		Scope& AppendScope(const Key& key, const std::size_t capacity=0);

		/// <summary>
		/// Removes a child Scope, given its address.
		/// </summary>
		/// <param name="child">Scope to remove from the Table.</param>
		/// <returns>True, if the child was successfully orphaned. Otherwise, false.</returns>
		gsl::owner<Scope*> Orphan(Scope& child);

		/// <summary>
		/// Appends an existing Scope, calling Orphan as needed on the previous parent.
		/// </summary>
		/// <param name="child">Scope to copy and append.</param>
		/// <param name="key">Key value of the Attribute that the child will be appended onto.</param>
		/// <returns>Reference to the newly adopted Scope.</returns>
		Scope& Adopt(Scope& child, const Key& key);

		/// <summary>
		/// Clears all members of the scope.
		/// </summary>
		void Clear();
#pragma endregion Modifiers

#pragma region Helper Methods
	private:
		/// <summary>
		/// SearchChildren helper method. 
		/// Performs a breadth-first search on the Scope and its children for a Attribute with a matching Key value.
		/// </summary>
		/// <param name="queue">Queue of Scope values to be searched.</param>
		/// <param name="key">Key value of the Attribute to be found.</param>
		/// <param name="scopePtrOut">Output parameter that points to the Scope which owns the found Attribute.</param>
		/// <returns>If found, a pointer to the Data value of the Attribute. Otherwise, nullptr.</returns>
		static Data* SearchChildrenHelper(const Vector<Scope*>& queue, const Key& key, Scope** scopePtrOut=nullptr);
#pragma endregion Helper Methods

#pragma region RTTI Overrides
	public:
		/// <summary>
		/// RTTI override that returns a string representation of the Scope.
		/// </summary>
		/// <returns>String representation of the Scope.</returns>
		virtual std::string ToString() const override;

		/// <summary>
		/// RTTI override that determines if the Scope is equal to a given RTTI derived class instance.
		/// </summary>
		/// <param name="rhs">RTTI derived class instance to be compared against.</param>
		/// <returns>True if the Scope is equivalent to the given RTTI derived class instance.</returns>
		virtual bool Equals(const RTTI* rhs) const override;
#pragma endregion RTTI Overrides

#pragma region Data Members
	protected:
		/// <summary>
		/// Vector of references to the TableEntryType values.
		/// </summary>
		Vector<Attribute*> mPairPtrs;

	private:
		/// <summary>
		/// Pointer to the parent Scope instance, if a child.
		/// </summary>
		Scope* mParent{ nullptr };

		/// <summary>
		/// Table containing TableEntryType values.
		/// </summary>
		Table mTable;

		/// <summary>
		/// SList containing child Scopes.
		/// </summary>
		Vector<Scope*> mChildren;
#pragma endregion Data Members
	};
}

#include "Scope.inl"