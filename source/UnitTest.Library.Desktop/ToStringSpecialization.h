#pragma once

#include "CppUnitTest.h"

#include "Foo.h"
#include "Bar.h"
#include "SList.h"
#include "Vector.h"
#include "HashMap.h"

using namespace UnitTests;
using namespace Library;
using namespace std::string_literals;


namespace Microsoft::VisualStudio::CppUnitTestFramework
{
#pragma region Foo
	template<>
	inline std::wstring ToString<Foo>(const Foo& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}

	template<>
	inline std::wstring ToString<Foo>(const Foo* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Foo>(Foo* t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion Foo

#pragma region Bar
	template<>
	inline std::wstring ToString<Bar>(const Bar& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}

	template<>
	inline std::wstring ToString<Bar>(const Bar* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Bar>(Bar* t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion Bar

#pragma region SList
	template<>
	inline std::wstring ToString<SList<int>>(const SList<int>& t)
	{
		RETURN_WIDE_STRING(t.Size());
	}

	template<>
	inline std::wstring ToString<SList<int>>(const SList<int>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<SList<int>>(SList<int>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<SList<double>>(const SList<double>& t)
	{
		RETURN_WIDE_STRING(t.Size());
	}

	template<>
	inline std::wstring ToString<SList<double>>(const SList<double>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<SList<double>>(SList<double>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<SList<Foo>>(const SList<Foo>& t)
	{
		RETURN_WIDE_STRING(t.Size());
	}

	template<>
	inline std::wstring ToString<SList<Foo>>(const SList<Foo>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<SList<Foo>>(SList<Foo>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<SList<Bar>>(const SList<Bar>& t)
	{
		RETURN_WIDE_STRING(t.Size());
	}

	template<>
	inline std::wstring ToString<SList<Bar>>(const SList<Bar>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<SList<Bar>>(SList<Bar>* t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion SList

#pragma region SList Iterator
	template<>
	inline std::wstring ToString<SList<int>::Iterator>(const SList<int>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<SList<int>::Iterator>(const SList<int>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<SList<int>::Iterator>(SList<int>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<SList<double>::Iterator>(const SList<double>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<SList<double>::Iterator>(const SList<double>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<SList<double>::Iterator>(SList<double>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<SList<Foo>::Iterator>(const SList<Foo>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<SList<Foo>::Iterator>(const SList<Foo>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<SList<Foo>::Iterator>(SList<Foo>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<SList<Bar>::Iterator>(const SList<Bar>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<SList<Bar>::Iterator>(const SList<Bar>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<SList<Bar>::Iterator>(SList<Bar>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion SList Iterator

#pragma region SList ConstIterator
	template<>
	inline std::wstring ToString<SList<int>::ConstIterator>(const SList<int>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<SList<int>::ConstIterator>(const SList<int>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<SList<int>::ConstIterator>(SList<int>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<SList<double>::ConstIterator>(const SList<double>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<SList<double>::ConstIterator>(const SList<double>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<SList<double>::ConstIterator>(SList<double>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<SList<Foo>::ConstIterator>(const SList<Foo>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<SList<Foo>::ConstIterator>(const SList<Foo>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<SList<Foo>::ConstIterator>(SList<Foo>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}	
	
	template<>
	inline std::wstring ToString<SList<Bar>::ConstIterator>(const SList<Bar>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<SList<Bar>::ConstIterator>(const SList<Bar>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<SList<Bar>::ConstIterator>(SList<Bar>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion SList ConstIterator

#pragma region Vector
	template<>
	inline std::wstring ToString<Vector<int>>(const Vector<int>& t)
	{
		RETURN_WIDE_STRING(t.Size());
	}

	template<>
	inline std::wstring ToString<Vector<int>>(const Vector<int>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Vector<int>>(Vector<int>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Vector<double>>(const Vector<double>& t)
	{
		RETURN_WIDE_STRING(t.Size());
	}

	template<>
	inline std::wstring ToString<Vector<double>>(const Vector<double>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Vector<double>>(Vector<double>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Vector<Foo>>(const Vector<Foo>& t)
	{
		RETURN_WIDE_STRING(t.Size());
	}

	template<>
	inline std::wstring ToString<Vector<Foo>>(const Vector<Foo>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Vector<Foo>>(Vector<Foo>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Vector<Bar>>(const Vector<Bar>& t)
	{
		RETURN_WIDE_STRING(t.Size());
	}

	template<>
	inline std::wstring ToString<Vector<Bar>>(const Vector<Bar>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Vector<Bar>>(Vector<Bar>* t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion Vector

#pragma region Vector Iterator
	template<>
	inline std::wstring ToString<Vector<int>::Iterator>(const Vector<int>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Vector<int>::Iterator>(const Vector<int>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Vector<int>::Iterator>(Vector<int>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Vector<double>::Iterator>(const Vector<double>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Vector<double>::Iterator>(const Vector<double>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Vector<double>::Iterator>(Vector<double>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Vector<Foo>::Iterator>(const Vector<Foo>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Vector<Foo>::Iterator>(const Vector<Foo>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Vector<Foo>::Iterator>(Vector<Foo>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Vector<Bar>::Iterator>(const Vector<Bar>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Vector<Bar>::Iterator>(const Vector<Bar>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Vector<Bar>::Iterator>(Vector<Bar>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion Vector Iterator

#pragma region Vector ConstIterator
	template<>
	inline std::wstring ToString<Vector<int>::ConstIterator>(const Vector<int>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Vector<int>::ConstIterator>(const Vector<int>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Vector<int>::ConstIterator>(Vector<int>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Vector<double>::ConstIterator>(const Vector<double>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Vector<double>::ConstIterator>(const Vector<double>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Vector<double>::ConstIterator>(Vector<double>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Vector<Foo>::ConstIterator>(const Vector<Foo>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Vector<Foo>::ConstIterator>(const Vector<Foo>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Vector<Foo>::ConstIterator>(Vector<Foo>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}	
	
	template<>
	inline std::wstring ToString<Vector<Bar>::ConstIterator>(const Vector<Bar>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Vector<Bar>::ConstIterator>(const Vector<Bar>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Vector<Bar>::ConstIterator>(Vector<Bar>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion Vector ConstIterator

#pragma region HashMap
	template<>
	inline std::wstring ToString<HashMap<double, Foo>>(const HashMap<double, Foo>& t)
	{
		RETURN_WIDE_STRING(t.Size());
	}

	template<>
	inline std::wstring ToString<HashMap<double, Foo>>(const HashMap<double, Foo>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<double, Foo>>(HashMap<double, Foo>* t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion HashMap

#pragma region HashMap PairType
	template<>
	inline std::wstring ToString<HashMap<double, Foo>::PairType>(const HashMap<double, Foo>::PairType& t)
	{
		RETURN_WIDE_STRING(t.first);
	}

	template<>
	inline std::wstring ToString<HashMap<double, Foo>::PairType>(const HashMap<double, Foo>::PairType* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<double, Foo>::PairType>(HashMap<double, Foo>::PairType* t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion HashMap PairType

#pragma region HashMap Iterator
	template<>
	inline std::wstring ToString<HashMap<double, Foo>::Iterator>(const HashMap<double, Foo>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<HashMap<double, Foo>::Iterator>(const HashMap<double, Foo>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<double, Foo>::Iterator>(HashMap<double, Foo>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion HashMap Iterator
}