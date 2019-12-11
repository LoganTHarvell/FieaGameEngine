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
}