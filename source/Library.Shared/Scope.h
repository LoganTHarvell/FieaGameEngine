#pragma once

#pragma region Includes

// Standard
#include <string>

// First Party
#include "HashMap.h"
#include "Vector.h"
#include "Datum.h"

#pragma endregion Includes
namespace Library
{
	/// <summary>
	/// Represents an ordered table of string name and Datum value pairs.
	/// </summary>
	class Scope final
	{
#pragma region Type Definitions and Constants
	public:
		/// <summary>
		/// TableEntry name type.
		/// </summary>
		using NameType = std::string;

		/// <summary>
		/// TableEntry data type.
		/// </summary>
		using DataType = Datum;

		/// <summary>
		/// Table type defining a mapping names to data values.
		/// </summary>
		using TableType = HashMap<NameType, DataType>;

		/// <summary>
		/// Table entry type defining a name and data value pair.
		/// </summary>
		using TableEntryType = TableType::PairType;
#pragma endregion Type Definitions and Constants

#pragma region Constructors, Destructor, Assignment
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="capacity">Initial capacity of the Scope.</param>
		explicit Scope(const std::size_t capacity=0);

		/// <summary>
		/// Default destructor.
		/// </summary>
		~Scope() = default;

		/// <summary>
		/// Copy constructor.
		/// Takes in a Scope as a parameter, then copies the data values to the constructed Scope.
		/// </summary>
		/// <param name="rhs">Scope to be copied.</param>
		Scope(const Scope& rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// Copies the data values from the right hand side (rhs) value to the left hand side.
		/// </summary>
		/// <param name="rhs">Scope whose values are copied.</param>
		/// <returns>Modified Scope with copied values.</returns>
		Scope& operator=(const Scope& rhs) = default;

		/// <summary>
		/// Move constructor.
		/// Takes a Scope as a parameter and moves the data to the constructed Scope.
		/// </summary>
		/// <param name="rhs">Scope to be moved.</param>
		Scope(Scope&& rhs) = default;

		/// <summary>
		/// Move assignment operator.
		/// Moves the data values from the right hand side (rhs) value to the left hand side.
		/// </summary>
		/// <param name="rhs">Scope whose values are copied.</param>
		/// <returns>Modified Scope with copied values.</returns>
		Scope& operator=(Scope&& rhs) = default;

		/// <summary>
		/// Initializer list constructor.
		/// </summary>
		/// <param name="rhs">List of PairType values for insertion.</param>
		/// <param name="capacity">Capacity to initialize for the Scope.</param>
		Scope(const std::initializer_list<TableEntryType> rhs, const std::size_t capacity=0);

		/// <summary>
		/// Initializer list assignment operator.
		/// </summary>
		/// <param name="rhs">List of values to be in the Scope.</param>
		/// <returns>Reference to the modified Scope containing the new pairs.</returns>
		Scope& operator=(const std::initializer_list<TableEntryType> rhs);
#pragma endregion Constructors, Destructor, Assignment

#pragma region Boolean Operators
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

#pragma region Element Accessors
		/// <summary>
		/// Gets a pointer to the parent Scope, if it exists.
		/// </summary>
		/// <returns>If a child, the pointer to the parent Scope. Otherwise, nullptr.</returns>
		Scope* GetParent();

		/// <summary>
		/// Gets a pointer to the parent Scope, if it exists.
		/// </summary>
		/// <returns>If a child, the pointer to the parent Scope. Otherwise, nullptr.</returns>
		const Scope* GetParent() const;

		/// <summary>
		/// Subscript operator that wraps Append.
		/// </summary>
		/// <param name="name">Name for the TableEntry to be appended.</param>
		/// <returns>Reference to the DataType value of the appended TableEntry.</returns>
		DataType& operator[](const NameType name);

		/// <summary>
		/// Subscript operator that gets the TableEntry DataType value
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		DataType& operator[](const std::size_t index);

		/// <summary>
		/// Finds the DataType value associated with the given NameType value, if it exists.
		/// </summary>
		/// <param name="name">NameType value associated with the DataType value to be found.</param>
		/// <returns>If found, a pointer to the DataType value. Otherwise, nullptr.</returns>
		DataType* Find(const NameType name);

		/// <summary>
		/// Finds the DataType value associated with the given NameType value, if it exists.
		/// </summary>
		/// <param name="name">TableEntry name associated with the data to be found.</param>
		/// <returns>If found, a pointer to the DataType value. Otherwise, nullptr.</returns>
		const DataType* Find(const NameType name) const;

		/// <summary>
		/// Finds the name for the TableEntry at the given index.
		/// </summary>
		/// <param name="index">Index of the TableEntry with the name to be found.</param>
		/// <returns>NameType value of the TableEntry at the given index.</returns>
		const NameType& FindName(const std::size_t index) const;
#pragma endregion Element Accessors

#pragma region Modifiers
		/// <summary>
		/// Appends a TableEntry to the Scope with the given name and a default data value.
		/// </summary>
		/// <param name="name"></param>
		/// <returns>Reference to the DataType value of the appended TableEntry.</returns>
		DataType& Append(const NameType name);
#pragma endregion Modifiers

#pragma region DataType Members
	private:
		/// <summary>
		/// Pointer to the parent Scope instance, if a child.
		/// </summary>
		Scope* mParent{ nullptr };

		/// <summary>
		/// Table containing TableEntryType values.
		/// </summary>
		TableType mTable;

		/// <summary>
		/// Vector of references to the TableEntryType values.
		/// </summary>
		Vector<TableEntryType*> mPairPtrs;
#pragma endregion DataType Members
	};
}

