#include "pch.h"
#include "DerivedAttributedFoo.h"

namespace UnitTests
{
	DerivedAttributedFoo::DerivedAttributedFoo(int data) :
		AttributedFoo(data)
	{
	}

	gsl::owner<Library::Scope*> DerivedAttributedFoo::Clone() const
	{
		return new DerivedAttributedFoo(*this);
	}

	const Library::TypeManager::TypeInfo& DerivedAttributedFoo::TypeInfo()
	{
		static const Library::TypeManager::TypeInfo typeInfo = { Library::SignatureListType(), AttributedFoo::TypeIdClass() };
		return typeInfo;
	}
}