#pragma once

#include "CppUnitTest.h"

#include "Foo.h"
#include "Bar.h"
#include "SList.h"

using namespace UnitTests;
using namespace Library;
using namespace std::string_literals;


namespace Microsoft::VisualStudio::CppUnitTestFramework
{
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
}