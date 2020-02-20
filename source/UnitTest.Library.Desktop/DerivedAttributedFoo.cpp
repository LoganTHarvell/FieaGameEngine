#include "pch.h"
#include "DerivedAttributedFoo.h"

namespace UnitTests
{
	RTTI_DEFINITIONS(DerivedAttributedFoo)

		DerivedAttributedFoo::DerivedAttributedFoo(int data) :
		AttributedFoo(data)
	{
	}

	gsl::owner<Library::Scope*> DerivedAttributedFoo::Clone() const
	{
		return new DerivedAttributedFoo(*this);
	}

	Library::TypeManager::SignatureListType DerivedAttributedFoo::Signatures()
	{
		return Library::TypeManager::SignatureListType();
	}
}