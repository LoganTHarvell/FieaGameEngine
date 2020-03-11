#pragma once

#include "Foo.h"

namespace UnitTests
{
	class DerivedFoo : public Foo
	{
		RTTI_DECLARATIONS(DerivedFoo, Foo)
	};
}
