#pragma once

#include "RTTI.h"

namespace Library
{
	class RenderContext : public RTTI
	{
		RTTI_DECLARATIONS_ABSTRACT(RenderContext, RTTI)

	public:
		RenderContext() = default;
		virtual ~RenderContext() = default;
		RenderContext(const RenderContext & rhs) = default;
		RenderContext& operator=(const RenderContext & rhs) = default;
		RenderContext(RenderContext && rhs) = default;
		RenderContext& operator=(RenderContext && rhs) = default;
	};
}