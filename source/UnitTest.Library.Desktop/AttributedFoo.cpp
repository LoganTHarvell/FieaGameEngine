#include "pch.h"
#include "AttributedFoo.h"

namespace UnitTests
{
	RTTI_DEFINITIONS(AttributedFoo);

	AttributedFoo::AttributedFoo(int data) : Attributed(TypeIdClass()),
		mIntData(data), mFloatData(static_cast<float>(data)), 
		mVectorData(static_cast<float>(data)), mMatrixData(static_cast<float>(data)), 
		mStringData(std::to_string(data)), foo(data)
	{
	}

	AttributedFoo::AttributedFoo(const AttributedFoo& rhs) : mRttiData(&foo), Attributed(rhs)
	{
	}

	AttributedFoo& AttributedFoo::operator=(const AttributedFoo& rhs)
	{
		Attributed::operator=(rhs);

		return *this;
	}

	std::string AttributedFoo::ToString() const
	{
		std::ostringstream oss;
		oss << "Foo " << Attributed::ToString();
		return oss.str();
	}

	gsl::owner<Library::Scope*> AttributedFoo::Clone() const
	{
		return new AttributedFoo(*this);
	}

	const Library::TypeManager::TypeInfo& AttributedFoo::TypeInfo()
	{
		static const Library::TypeManager::TypeInfo typeInfo
		{
			{
				{ "integer", DataType::Types::Integer, false, 1, offsetof(AttributedFoo, mIntData) },
				{ "float", DataType::Types::Float, false, 1, offsetof(AttributedFoo, mFloatData) },
				{ "vector", DataType::Types::Vector, false, 1, offsetof(AttributedFoo, mVectorData) },
				{ "matrix", DataType::Types::Matrix, false, 1, offsetof(AttributedFoo, mMatrixData) },
				{ "string", DataType::Types::String, false, 1, offsetof(AttributedFoo, mStringData) },
				{ "scope", DataType::Types::Scope, true, 1 , 0},
				{ "rttiPtr", DataType::Types::Pointer, false, 1, offsetof(AttributedFoo, mRttiData) }
			},

			Attributed::TypeIdClass()
		};

		return typeInfo;
	}
}