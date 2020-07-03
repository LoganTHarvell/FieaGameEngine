#include <catch.hpp>

#include "SList.h"

using namespace Library;

TEMPLATE_TEST_CASE("SList Construction", "[SList]", int, float, std::string)
{
	GIVEN("Default Constructor")
	{
		SList<TestType> list;

		THEN("SList is empty, begin and end iterators are equal")
		{
			REQUIRE(list.Size() == 0);
			REQUIRE(list.IsEmpty());
			REQUIRE(list.begin() == list.end());
		}
	}
}