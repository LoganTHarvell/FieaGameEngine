#include "Stack.h"

namespace Library
{
#pragma region Element Accessors
	template<typename T, typename Container>
	inline T& Stack<T, Container>::Top()
	{
		return mContainer.Back();
	}

	template<typename T, typename Container>
	inline const T& Stack<T, Container>::Top() const
	{
		return mContainer.Back();
	}
#pragma endregion Element Accessors

#pragma region Size
	template<typename T, typename Container>
	inline std::size_t Stack<T, Container>::Size() const
	{
		return mContainer.Size();
	}
	
	template<typename T, typename Container>
	inline bool Stack<T, Container>::IsEmpty() const
	{
		return mContainer.IsEmpty();
	}
#pragma endregion Size

#pragma region Modifiers
	template<typename T, typename Container>
	inline void Stack<T, Container>::Push(const T& value)
	{
		mContainer.PushBack(value);
	}

	template<typename T, typename Container>
	inline void Stack<T, Container>::Push(T&& value)
	{
		mContainer.PushBack(std::move(value));
	}

	template<typename T, typename Container>
	inline void Stack<T, Container>::Pop()
	{
		mContainer.PopBack();
	}

	template<typename T, typename Container>
	inline void Stack<T, Container>::Clear()
	{
		mContainer.Clear();
	}
#pragma endregion Modifiers
}