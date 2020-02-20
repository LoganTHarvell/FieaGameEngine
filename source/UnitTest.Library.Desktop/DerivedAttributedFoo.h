#pragma once

#include <memory>

#include "AttributedFoo.h"

namespace UnitTests
{
	class DerivedAttributedFoo final : public AttributedFoo
	{
		RTTI_DECLARATIONS(DerivedAttributedFoo, AttributedFoo)

	public:
		explicit DerivedAttributedFoo(int data = 0);
		DerivedAttributedFoo(const DerivedAttributedFoo& rhs) = default;
		DerivedAttributedFoo(DerivedAttributedFoo&& rhs) noexcept = default;
		DerivedAttributedFoo& operator=(const DerivedAttributedFoo& rhs) = default;
		DerivedAttributedFoo& operator=(DerivedAttributedFoo&& rhs) noexcept = default;
		virtual ~DerivedAttributedFoo() = default;

		virtual gsl::owner<Library::Scope*> Clone() const override;
		static Library::TypeManager::SignatureListType Signatures();
	};
}
