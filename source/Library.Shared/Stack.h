#pragma once

namespace Library
{
	/// <summary>
	/// Forward declaration for Vector class used as the default Container type.
	/// </summary>
	template<typename T> class Vector;

	/// <summary>
	/// Represents a Stack container.
	/// </summary>
	/// <typeparam name="T">Type of the elements on the Stack.</typeparam>
	/// <typeparam name="T">Type of the container used by the Stack.</typeparam>
	template<typename T, typename Container=Vector<T>>
	class Stack
	{
#pragma region Element Accessors
	public:
		/// <summary>
		/// Gets a reference to the top element on the Stack.
		/// </summary>
		/// <returns>Reference to the top element on the Stack.</returns>
		T& Top();

		/// <summary>
		/// Gets a const reference to the top element on the Stack.
		/// </summary>
		/// <returns>Constant reference to the top element on the Stack.</returns>
		const T& Top() const;
#pragma endregion Element Accessors

#pragma region Size
	public:
		/// <summary>
		/// Getter method for the number of values in the Stack.
		/// </summary>
		/// <returns>Number of Table Entry values in the Scope.</returns>
		std::size_t Size() const;

		/// <summary>
		/// Checks if the size of the Stack is greater than zero, indicating it is non-empty.
		/// </summary>
		/// <returns>True if the Stack contains no elements, otherwise false.</returns>
		bool IsEmpty() const;
#pragma endregion Size

#pragma region Modifiers
	public:
		/// <summary>
		/// Constructs and adds an element to the back of the Stack given an argument list.
		/// </summary>
		/// <param name="args">Argument list used to construct the element.</param>
		/// <typeparam name="Args">Variadic list for constructor arguments.</typeparam>
		template<typename... Args>
		T& Emplace(Args&&... args);
		
		/// <summary>
		/// Pushes an element onto the Stack.
		/// </summary>
		/// <param name="value">Reference to the element to be pushed onto the Stack.</param>
		void Push(const T& value);

		/// <summary>
		/// Pushes an element onto the Stack.
		/// </summary>
		/// <param name="value">Reference to the element to be pushed onto the Stack.</param>
		void Push(T&& value);

		/// <summary>
		/// Pops an element from the top of the Stack.
		/// </summary>
		void Pop();

		/// <summary>
		/// Clears all elements from the Stack.
		/// </summary>
		void Clear();
#pragma endregion Modifiers

#pragma region Data Members
	private:
		/// <summary>
		/// Instance of the container used to store elements on the Stack.
		/// </summary>
		Container mContainer{ Container(Container::EqualityFunctor()) };
#pragma endregion Data Members
	};
}

#include "Stack.inl"

