#include "pch.h"
#include "DerivedAttributedFoo.h"

namespace UnitTests
{
	DerivedAttributedFoo::DerivedAttributedFoo(int data) : AttributedFoo(TypeIdClass(), data)
	{
	}

	gsl::owner<Library::Scope*> DerivedAttributedFoo::Clone() const
	{
		return new DerivedAttributedFoo(*this);
	}

	const Library::SignatureList& DerivedAttributedFoo::Signatures()
	{
		static const Library::SignatureList signatures = { Library::SignatureList() };
		return signatures;
	}
}