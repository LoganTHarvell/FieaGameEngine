#pragma once

#include "CppUnitTest.h"

#include "Foo.h"
#include "SList.h"

using namespace UnitTests;
using namespace Library;


namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Foo>(const Foo& t)
	{
		RETURN_WIDE_STRING("Foo<T>: " + t.Data());
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
	inline std::wstring ToString<SList<int>>(const SList<int>& t)
	{
		RETURN_WIDE_STRING("SList<int>: " + t.Size());
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
		RETURN_WIDE_STRING("SList<double>: " + t.Size());
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
		RETURN_WIDE_STRING("SList<Foo>: " + t.Size());
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
	inline std::wstring ToString<SList<int>::Iterator>(const SList<int>::Iterator& t)
	{
		SList<int>::ConstIterator it = t;
		RETURN_WIDE_STRING("SList<int>::Iterator");
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
		SList<double>::ConstIterator it = t;
		RETURN_WIDE_STRING("SList<double>::Iterator");
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
		SList<Foo>::ConstIterator it = t;
		RETURN_WIDE_STRING("SList<Foo>::Iterator");
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
	inline std::wstring ToString<SList<int>::ConstIterator>(const SList<int>::ConstIterator& t)
	{
		SList<int>::ConstIterator it = t;
		RETURN_WIDE_STRING("SList<int>::ConstIterator");
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
		SList<double>::ConstIterator it = t;
		RETURN_WIDE_STRING("SList<double>::ConstIterator");
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
		SList<Foo>::ConstIterator it = t;
		RETURN_WIDE_STRING("SList<Foo>::ConstIterator");
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
}