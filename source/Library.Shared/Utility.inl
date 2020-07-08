#pragma once

#include "Utility.h"

namespace Library
{
	template <typename T>
	T AlignedSize(T size, T alignment)
	{
		if (alignment > 1)
		{
			const T alignmentBitmask = alignment - 1;
			return ((size + alignmentBitmask) & ~alignmentBitmask);
		}
		return size;
	}

	namespace Exception
	{
		inline AggregateException::AggregateException(const Vector<Entry>& exceptions) :
			Exceptions(exceptions)
		{
		}

		inline AggregateException::AggregateException(Vector<Entry>&& exceptions) :
			Exceptions(std::move(exceptions))
		{
		}
	}
}
