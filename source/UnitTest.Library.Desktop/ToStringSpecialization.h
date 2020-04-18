#pragma once

#include "CppUnitTest.h"

#include <chrono>

#pragma warning(disable : 4201)
#include <glm/gtx/string_cast.hpp>
#pragma warning(default : 4201)

#include "Foo.h"
#include "Bar.h"
#include "AttributedFoo.h"
#include "AttributedBar.h"
#include "SList.h"
#include "Vector.h"
#include "HashMap.h"
#include "Datum.h"
#include "Scope.h"
#include "TypeManager.h"
#include "JsonParseMaster.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "Action.h"
#include "GameTime.h"
#include "EventQueue.h"

using namespace UnitTests;
using namespace Library;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
#pragma region Standard Library
	template<>
	inline std::wstring ToString<std::chrono::milliseconds>(const std::chrono::milliseconds& t)
	{
		RETURN_WIDE_STRING(t.count());
	}

	template<>
	inline std::wstring ToString<std::chrono::milliseconds>(const std::chrono::milliseconds* t)
	{
		RETURN_WIDE_STRING(t->count());
	}

	template<>
	inline std::wstring ToString<std::chrono::milliseconds>(std::chrono::milliseconds* t)
	{
		RETURN_WIDE_STRING(t->count());
	}
#pragma endregion Standard Library

#pragma region ThirdParty
	template<>
	inline std::wstring ToString<glm::vec4>(const glm::vec4& t)
	{
		RETURN_WIDE_STRING(glm::to_string(t).c_str());
	}

	template<>
	inline std::wstring ToString<glm::vec4>(const glm::vec4* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<glm::vec4>(glm::vec4* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<glm::mat4>(const glm::mat4& t)
	{
		RETURN_WIDE_STRING(glm::to_string(t).c_str());
	}

	template<>
	inline std::wstring ToString<glm::mat4>(const glm::mat4* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<glm::mat4>(glm::mat4* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<gsl::span<int>>(const gsl::span<int>& t)
	{
		RETURN_WIDE_STRING(t.size());
	}

	template<>
	inline std::wstring ToString<gsl::span<int>>(const gsl::span<int>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<gsl::span<int>>(gsl::span<int>* t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion ThirdParty

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

#pragma region Attributed
	template<>
	inline std::wstring ToString<Attributed>(const Attributed& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	inline std::wstring ToString<Attributed>(const Attributed* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Attributed>(Attributed* t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion Attributed

#pragma region AttributedFoo
	template<>
	inline std::wstring ToString<AttributedFoo>(const AttributedFoo& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	inline std::wstring ToString<AttributedFoo>(const AttributedFoo* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<AttributedFoo>(AttributedFoo* t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion AttributedFoo

#pragma region AttributedBar
	template<>
	inline std::wstring ToString<AttributedBar>(const AttributedBar& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}

	template<>
	inline std::wstring ToString<AttributedBar>(const AttributedBar* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<AttributedBar>(AttributedBar* t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion AttributedBar

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

#pragma region HashMap Pair
	template<>
	inline std::wstring ToString<HashMap<int, Foo>::Pair>(const HashMap<int, Foo>::Pair& t)
	{
		RETURN_WIDE_STRING(t.first);
	}

	template<>
	inline std::wstring ToString<HashMap<int, Foo>::Pair>(const HashMap<int, Foo>::Pair* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<int, Foo>::Pair>(HashMap<int, Foo>::Pair* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<double, Foo>::Pair>(const HashMap<double, Foo>::Pair& t)
	{
		RETURN_WIDE_STRING(t.first);
	}

	template<>
	inline std::wstring ToString<HashMap<double, Foo>::Pair>(const HashMap<double, Foo>::Pair* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<double, Foo>::Pair>(HashMap<double, Foo>::Pair* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<Foo, Foo>::Pair>(const HashMap<Foo, Foo>::Pair& t)
	{
		RETURN_WIDE_STRING(t.first.Data());
	}

	template<>
	inline std::wstring ToString<HashMap<Foo, Foo>::Pair>(const HashMap<Foo, Foo>::Pair* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<Foo, Foo>::Pair>(HashMap<Foo, Foo>::Pair* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<Bar, Foo>::Pair>(const HashMap<Bar, Foo>::Pair& t)
	{
		RETURN_WIDE_STRING(t.first.Data());
	}

	template<>
	inline std::wstring ToString<HashMap<Bar, Foo>::Pair>(const HashMap<Bar, Foo>::Pair* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<Bar, Foo>::Pair>(HashMap<Bar, Foo>::Pair* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<Bar, Bar>::Pair>(const HashMap<Bar, Bar>::Pair& t)
	{
		RETURN_WIDE_STRING(t.first.Data());
	}

	template<>
	inline std::wstring ToString<HashMap<Bar, Bar>::Pair>(const HashMap<Bar, Bar>::Pair* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<Bar, Bar>::Pair>(HashMap<Bar, Bar>::Pair* t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion HashMap Pair

#pragma region HashMap Iterator
	template<>
	inline std::wstring ToString<std::pair<HashMap<int, Foo>::Iterator, bool>>(const std::pair<HashMap<int, Foo>::Iterator, bool>& t)
	{
		try
		{
			return ToString(*(t.first));
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<std::pair<HashMap<int, Foo>::Iterator, bool>>(const std::pair<HashMap<int, Foo>::Iterator, bool>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<std::pair<HashMap<int, Foo>::Iterator, bool>>(std::pair<HashMap<int, Foo>::Iterator, bool>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<std::pair<HashMap<double, Foo>::Iterator, bool>>(const std::pair<HashMap<double, Foo>::Iterator, bool>& t)
	{
		try
		{
			return ToString(*(t.first));
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<std::pair<HashMap<double, Foo>::Iterator, bool>>(const std::pair<HashMap<double, Foo>::Iterator, bool>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<std::pair<HashMap<double, Foo>::Iterator, bool>>(std::pair<HashMap<double, Foo>::Iterator, bool>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<std::pair<HashMap<Foo, Foo>::Iterator, bool>>(const std::pair<HashMap<Foo, Foo>::Iterator, bool>& t)
	{
		try
		{
			return ToString(*(t.first));
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<std::pair<HashMap<Foo, Foo>::Iterator, bool>>(const std::pair<HashMap<Foo, Foo>::Iterator, bool>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<std::pair<HashMap<Foo, Foo>::Iterator, bool>>(std::pair<HashMap<Foo, Foo>::Iterator, bool>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<std::pair<HashMap<Bar, Foo>::Iterator, bool>>(const std::pair<HashMap<Bar, Foo>::Iterator, bool>& t)
	{
		try
		{
			return ToString(*(t.first));
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<std::pair<HashMap<Bar, Foo>::Iterator, bool>>(const std::pair<HashMap<Bar, Foo>::Iterator, bool>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<std::pair<HashMap<Bar, Foo>::Iterator, bool>>(std::pair<HashMap<Bar, Foo>::Iterator, bool>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<std::pair<HashMap<Bar, Bar>::Iterator, bool>>(const std::pair<HashMap<Bar, Bar>::Iterator, bool>& t)
	{
		try
		{
			return ToString(*(t.first));
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<std::pair<HashMap<Bar, Bar>::Iterator, bool>>(const std::pair<HashMap<Bar, Bar>::Iterator, bool>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<std::pair<HashMap<Bar, Bar>::Iterator, bool>>(std::pair<HashMap<Bar, Bar>::Iterator, bool>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<int, Foo>::Iterator>(const HashMap<int, Foo>::Iterator& t)
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
	inline std::wstring ToString<HashMap<int, Foo>::Iterator>(const HashMap<int, Foo>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<int, Foo>::Iterator>(HashMap<int, Foo>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

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

	template<>
	inline std::wstring ToString<HashMap<Foo, Foo>::Iterator>(const HashMap<Foo, Foo>::Iterator& t)
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
	inline std::wstring ToString<HashMap<Foo, Foo>::Iterator>(const HashMap<Foo, Foo>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<Foo, Foo>::Iterator>(HashMap<Foo, Foo>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<Bar, Foo>::Iterator>(const HashMap<Bar, Foo>::Iterator& t)
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
	inline std::wstring ToString<HashMap<Bar, Foo>::Iterator>(const HashMap<Bar, Foo>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<Bar, Foo>::Iterator>(HashMap<Bar, Foo>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<Bar, Bar>::Iterator>(const HashMap<Bar, Bar>::Iterator& t)
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
	inline std::wstring ToString<HashMap<Bar, Bar>::Iterator>(const HashMap<Bar, Bar>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<Bar, Bar>::Iterator>(HashMap<Bar, Bar>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<int, Foo>::ConstIterator>(const HashMap<int, Foo>::ConstIterator& t)
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
	inline std::wstring ToString<HashMap<int, Foo>::ConstIterator>(const HashMap<int, Foo>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<int, Foo>::ConstIterator>(HashMap<int, Foo>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}
	template<>
	inline std::wstring ToString<HashMap<double, Foo>::ConstIterator>(const HashMap<double, Foo>::ConstIterator& t)
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
	inline std::wstring ToString<HashMap<double, Foo>::ConstIterator>(const HashMap<double, Foo>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<double, Foo>::ConstIterator>(HashMap<double, Foo>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<Foo, Foo>::ConstIterator>(const HashMap<Foo, Foo>::ConstIterator& t)
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
	inline std::wstring ToString<HashMap<Foo, Foo>::ConstIterator>(const HashMap<Foo, Foo>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<Foo, Foo>::ConstIterator>(HashMap<Foo, Foo>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<Bar, Foo>::ConstIterator>(const HashMap<Bar, Foo>::ConstIterator& t)
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
	inline std::wstring ToString<HashMap<Bar, Foo>::ConstIterator>(const HashMap<Bar, Foo>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<Bar, Foo>::ConstIterator>(HashMap<Bar, Foo>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<Bar, Bar>::ConstIterator>(const HashMap<Bar, Bar>::ConstIterator& t)
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
	inline std::wstring ToString<HashMap<Bar, Bar>::ConstIterator>(const HashMap<Bar, Bar>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<HashMap<Bar, Bar>::ConstIterator>(HashMap<Bar, Bar>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion HashMap Iterator

#pragma region Datum
	template<>
	inline std::wstring ToString<RTTI>(const RTTI& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<RTTI>(const RTTI* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<RTTI>(RTTI* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Datum::Types>(const Datum::Types& t)
	{
		RETURN_WIDE_STRING(static_cast<int>(t));
	}

	template<>
	inline std::wstring ToString<Datum::Types>(const Datum::Types* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Datum::Types>(Datum::Types* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Datum>(const Datum& t)
	{
		try
		{
			RETURN_WIDE_STRING(t.ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"Data Unknown or empty."s;
		}
	}

	template<>
	inline std::wstring ToString<Datum>(const Datum* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Datum>(Datum* t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion Datum

#pragma region Scope
	template<>
	inline std::wstring ToString<Scope>(const Scope& t)
	{
		RETURN_WIDE_STRING(t.GetParent());
	}

	template<>
	inline std::wstring ToString<Scope>(const Scope* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Scope>(Scope* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<std::pair<Scope::Data*, std::size_t>>(const std::pair<Scope::Data*, std::size_t>& t)
	{
		try 
		{
			std::string str = (t.first ? t.first->ToString() : "nullptr") + ", index: "s + std::to_string(t.second);
			return std::wstring(str.begin(), str.end());
		}
		catch (const std::exception&)
		{
			return L"Data Unknown"s;
		}
	}

	template<>
	inline std::wstring ToString<std::pair<Scope::Data*, std::size_t>>(const std::pair<Scope::Data*, std::size_t>* t)
	{
		try
		{
			std::string str = t->first->ToString() + ", index: "s + std::to_string(t->second);
			return std::wstring(str.begin(), str.end());
		}
		catch (const std::exception&)
		{
			return L"Data Unknown"s;
		}
	}

	template<>
	inline std::wstring ToString<std::pair<Scope::Data*, std::size_t>>(std::pair<Scope::Data*, std::size_t>* t)
	{
		try
		{
			std::string str = t->first->ToString() + ", index: "s + std::to_string(t->second);
			return std::wstring(str.begin(), str.end());
		}
		catch (const std::exception&)
		{
			return L"Data Unknown"s;
		}
	}

	template<>
	inline std::wstring ToString<std::pair<const Scope::Data*, std::size_t>>(const std::pair<const Scope::Data*, std::size_t>& t)
	{
		try 
		{
			std::string str = (t.first ? t.first->ToString() : "nullptr") + ", index: "s + std::to_string(t.second);
			return std::wstring(str.begin(), str.end());
		}
		catch (const std::exception&)
		{
			return L"Data Unknown"s;
		}
	}

	template<>
	inline std::wstring ToString<std::pair<const Scope::Data*, std::size_t>>(const std::pair<const Scope::Data*, std::size_t>* t)
	{
		try
		{
			std::string str = t->first->ToString() + ", index: "s + std::to_string(t->second);
			return std::wstring(str.begin(), str.end());
		}
		catch (const std::exception&)
		{
			return L"Data Unknown"s;
		}
	}

	template<>
	inline std::wstring ToString<std::pair<const Scope::Data*, std::size_t>>(std::pair<const Scope::Data*, std::size_t>* t)
	{
		try
		{
			std::string str = t->first->ToString() + ", index: "s + std::to_string(t->second);
			return std::wstring(str.begin(), str.end());
		}
		catch (const std::exception&)
		{
			return L"Data Unknown"s;
		}
	}
#pragma endregion Scope

#pragma region TypeManager
	template<>
	inline std::wstring ToString<SignatureListType>(const SignatureListType& t)
	{
		RETURN_WIDE_STRING(t.Size());
	}

	template<>
	inline std::wstring ToString<SignatureListType>(const SignatureListType* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<SignatureListType>(SignatureListType* t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion TypeManager

#pragma region JSON Parser
	template<>
	inline std::wstring ToString<JsonParseMaster::SharedData>(const JsonParseMaster::SharedData& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<JsonParseMaster::SharedData>(const JsonParseMaster::SharedData* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonParseMaster::SharedData>(JsonParseMaster::SharedData* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonParseMaster>(const JsonParseMaster& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<JsonParseMaster>(const JsonParseMaster* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonParseMaster>(JsonParseMaster* t)
	{
		RETURN_WIDE_STRING(t);
	}
#pragma endregion JSON Parser

#pragma region Action
	template<>
	inline std::wstring ToString<Action>(const Action& t)
	{
		RETURN_WIDE_STRING(t.Name().c_str());
	}

	template<>
	inline std::wstring ToString<Action>(const Action* t)
	{
		try 
		{
			RETURN_WIDE_STRING(t->Name().c_str());
		}
		catch (const std::exception&)
		{
			RETURN_WIDE_STRING(t);
		}
	}

	template<>
	inline std::wstring ToString<Action>(Action* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->Name().c_str());
		}
		catch (const std::exception&)
		{
			RETURN_WIDE_STRING(t);
		}
	}
#pragma endregion Action

#pragma region Entity
	template<>
	inline std::wstring ToString<Entity>(const Entity& t)
	{
		RETURN_WIDE_STRING(t.Name().c_str());
	}

	template<>
	inline std::wstring ToString<Entity>(const Entity* t)
	{
		try 
		{
			RETURN_WIDE_STRING(t->Name().c_str());
		}
		catch (const std::exception&)
		{
			RETURN_WIDE_STRING(t);
		}
	}

	template<>
	inline std::wstring ToString<Entity>(Entity* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->Name().c_str());
		}
		catch (const std::exception&)
		{
			RETURN_WIDE_STRING(t);
		}
	}
#pragma endregion Entity

#pragma region Sector
	template<>
	inline std::wstring ToString<Sector>(const Sector& t)
	{
		RETURN_WIDE_STRING(t.Name().c_str());
	}

	template<>
	inline std::wstring ToString<Sector>(const Sector* t)
	{
		try 
		{
			RETURN_WIDE_STRING(t->Name().c_str());
		}
		catch (const std::exception&)
		{
			RETURN_WIDE_STRING(t);
		}
	}

	template<>
	inline std::wstring ToString<Sector>(Sector* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->Name().c_str());
		}
		catch (const std::exception&)
		{
			RETURN_WIDE_STRING(t);
		}
	}
#pragma endregion Sector

#pragma region World
	template<>
	inline std::wstring ToString<World>(const World& t)
	{
		RETURN_WIDE_STRING(t.Name().c_str());
	}

	template<>
	inline std::wstring ToString<World>(const World* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->Name().c_str());
		}
		catch (const std::exception&)
		{
			RETURN_WIDE_STRING(t);
		}
	}

	template<>
	inline std::wstring ToString<World>(World* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->Name().c_str());
		}
		catch (const std::exception&)
		{
			RETURN_WIDE_STRING(t);
		}
	}
#pragma endregion World

#pragma region GameTime
	template<>
	inline std::wstring ToString<GameTime>(const GameTime& t)
	{
		RETURN_WIDE_STRING(t.ElapsedGameTime().count());
	}

	template<>
	inline std::wstring ToString<GameTime>(const GameTime* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->ElapsedGameTime().count());
		}
		catch (const std::exception&)
		{
			RETURN_WIDE_STRING(t);
		}
	}

	template<>
	inline std::wstring ToString<GameTime>(GameTime* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->ElapsedGameTime().count());
		}
		catch (const std::exception&)
		{
			RETURN_WIDE_STRING(t);
		}
	}
#pragma endregion GameTime

#pragma region EventQueue
	template<>
	inline std::wstring ToString<EventQueue>(const EventQueue& t)
	{
		RETURN_WIDE_STRING(t.Size());
	}

	template<>
	inline std::wstring ToString<EventQueue>(const EventQueue* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->Size());
		}
		catch (const std::exception&)
		{
			RETURN_WIDE_STRING(t);
		}
	}

	template<>
	inline std::wstring ToString<EventQueue>(EventQueue* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->Size());
		}
		catch (const std::exception&)
		{
			RETURN_WIDE_STRING(t);
		}
	}
#pragma endregion EventQueue
}