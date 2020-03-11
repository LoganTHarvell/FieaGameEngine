#include "pch.h"
#include "AttributedFoo.h"

namespace UnitTests
{
	AttributedFoo::AttributedFoo(int data) : Attributed(TypeIdClass()),
		mInt(data), mFloat(static_cast<float>(data)), 
		mVector(static_cast<float>(data)), mMatrix(static_cast<float>(data)), 
		mString(std::to_string(data)), foo(data), mIntArray{ data, data },
		mFloatArray{ static_cast<float>(data) }, 
		mVectorArray{ { data, data, data, data }, { data, data, data, data } },
		mMatrixArray{	{{ data, 0, 0, 0 }, { 0, data, 0, 0 }, { 0, 0, data, 0 }, { 0, 0, 0, data }},
						{{ data, 0, 0, 0 }, { 0, data, 0, 0 }, { 0, 0, data, 0 }, { 0, 0, 0, data }}	},
		mStringArray{ std::to_string(data), std::to_string(data) },
		foo1(data), foo2(data)
	{
	}

	AttributedFoo::AttributedFoo(const AttributedFoo& rhs) : Attributed(rhs)
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
				{ "Integer", Types::Integer, false, 1, offsetof(AttributedFoo, mInt) },
				{ "Float", Types::Float, false, 1, offsetof(AttributedFoo, mFloat) },
				{ "Vector", Types::Vector, false, 1, offsetof(AttributedFoo, mVector) },
				{ "Matrix", Types::Matrix, false, 1, offsetof(AttributedFoo, mMatrix) },
				{ "String", Types::String, false, 1, offsetof(AttributedFoo, mString) },
				{ "Scope", Types::Scope, true, 1 , 0},
				{ "Rtti", Types::Pointer, false, 1, offsetof(AttributedFoo, mRtti) },
				{ "IntegerArray", Types::Integer, false, 2, offsetof(AttributedFoo, mIntArray) },
				{ "FloatArray", Types::Float, false, 2, offsetof(AttributedFoo, mFloatArray) },
				{ "VectorArray", Types::Vector, false, 2, offsetof(AttributedFoo, mVectorArray) },
				{ "MatrixArray", Types::Matrix, false, 2, offsetof(AttributedFoo, mMatrixArray) },
				{ "StringArray", Types::String, false, 2, offsetof(AttributedFoo, mStringArray) },
				{ "ScopeArray", Types::Scope, true, 2 , 0},
				{ "RttiArray", Types::Pointer, false, 2, offsetof(AttributedFoo, mRttiArray) }
			},

			Attributed::TypeIdClass()
		};

		return typeInfo;
	}
}