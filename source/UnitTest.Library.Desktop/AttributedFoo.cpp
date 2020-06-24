#include "pch.h"
#include "AttributedFoo.h"

namespace UnitTests
{
	AttributedFoo::AttributedFoo(const int data) : Attributed(TypeIdClass()),
		mInt(data), mFloat(static_cast<float>(data)), 
		mVector(static_cast<float>(data)), mMatrix(static_cast<float>(data)), 
		mString(std::to_string(data)), foo(data), mIntArray{ data, data },
		mFloatArray{ static_cast<float>(data), static_cast<float>(data) }, 
		mVectorArray{ { data, data, data, data }, { data, data, data, data } },
		mMatrixArray{	{{ data, 0, 0, 0 }, { 0, data, 0, 0 }, { 0, 0, data, 0 }, { 0, 0, 0, data }},
						{{ data, 0, 0, 0 }, { 0, data, 0, 0 }, { 0, 0, data, 0 }, { 0, 0, 0, data }}	},
		mStringArray{ std::to_string(data), std::to_string(data) },
		foo1(data), foo2(data)
	{
	}

	AttributedFoo::AttributedFoo(const AttributedFoo& rhs) : Attributed(rhs),
		mInt(rhs.mInt), mFloat(rhs.mFloat), 
		mVector(rhs.mVector), mMatrix(rhs.mMatrix), 
		mString(rhs.mString), foo(rhs.foo), mIntArray{ rhs.mIntArray[0], rhs.mIntArray[1] },
		mFloatArray{ rhs.mFloatArray[0], rhs.mFloatArray[1] }, 
		mVectorArray{ rhs.mVectorArray[0], rhs.mVectorArray[1] },
		mMatrixArray{	rhs.mMatrixArray[0], rhs.mMatrixArray[1] },
		mStringArray{ rhs.mStringArray[0], rhs.mStringArray[1] },
		foo1(rhs.foo1), foo2(rhs.foo2)
	{
	}

	AttributedFoo& AttributedFoo::operator=(const AttributedFoo& rhs)
	{
		Attributed::operator=(rhs);

		mInt = rhs.mInt;
		mFloat = rhs.mFloat;
		mVector = rhs.mVector;
		mMatrix = rhs.mMatrix;
		mString = rhs.mString;

		foo = rhs.foo;

		for (std::size_t i = 0; i < 2; ++i)
		{
			mIntArray[i] = rhs.mIntArray[i];
			mFloatArray[i] = rhs.mFloatArray[i];
			mVectorArray[i] = rhs.mVectorArray[i];
			mMatrixArray[i] = rhs.mMatrixArray[i];
			mStringArray[i] = rhs.mStringArray[i];
		}

		foo1 = rhs.foo1;
		foo2 = rhs.foo2;

		return *this;
	}


	AttributedFoo::AttributedFoo(AttributedFoo&& rhs) noexcept : Attributed(std::move(rhs)),
		mInt(rhs.mInt), mFloat(rhs.mFloat), 
		mVector(rhs.mVector), mMatrix(rhs.mMatrix), 
		mString(rhs.mString), foo(rhs.foo), mIntArray{ rhs.mIntArray[0], rhs.mIntArray[1] },
		mFloatArray{ rhs.mFloatArray[0], rhs.mFloatArray[1] }, 
		mVectorArray{ rhs.mVectorArray[0], rhs.mVectorArray[1] },
		mMatrixArray{	rhs.mMatrixArray[0], rhs.mMatrixArray[1] },
		mStringArray{ std::move(rhs.mStringArray[0]), std::move(rhs.mStringArray[1]) },
		foo1(rhs.foo1), foo2(rhs.foo2)
	{
		rhs.mInt = 0;
		rhs.mFloat = 0;
		rhs.mVector = glm::vec4(0);
		rhs.mMatrix = glm::mat4(0);

		rhs.foo.SetData(0);

		for (std::size_t i = 0; i < 2; ++i)
		{
			rhs.mIntArray[i] = 0;
			rhs.mFloatArray[i] = 0;
			rhs.mVectorArray[i] = glm::vec4(0);
			rhs.mMatrixArray[i] = glm::mat4(0);
		}

		rhs.foo1.SetData(0);
		rhs.foo2.SetData(0);
	}

	AttributedFoo& AttributedFoo::operator=(AttributedFoo&& rhs) noexcept
	{
		Attributed::operator=(std::move(rhs));

		mInt = rhs.mInt;
		mFloat = rhs.mFloat;
		mVector = rhs.mVector;
		mMatrix = rhs.mMatrix;
		mString = std::move(rhs.mString);

		foo = rhs.foo;

		for (std::size_t i = 0; i < 2; ++i)
		{
			mIntArray[i] = rhs.mIntArray[i];
			mFloatArray[i] = rhs.mFloatArray[i];
			mVectorArray[i] = rhs.mVectorArray[i];
			mMatrixArray[i] = rhs.mMatrixArray[i];
			mStringArray[i] = std::move(rhs.mStringArray[i]);
		}
		
		foo1 = rhs.foo1;
		foo2 = rhs.foo2;

		rhs.mInt = 0;
		rhs.mFloat = 0;
		rhs.mVector = glm::vec4(0);
		rhs.mMatrix = glm::mat4(0);

		rhs.foo.SetData(0);

		for (std::size_t i = 0; i < 2; ++i)
		{
			rhs.mIntArray[i] = 0;
			rhs.mFloatArray[i] = 0;
			rhs.mVectorArray[i] = glm::vec4(0);
			rhs.mMatrixArray[i] = glm::mat4(0);
		}

		rhs.foo1.SetData(0);
		rhs.foo2.SetData(0);

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

	const Library::SignatureList& AttributedFoo::Signatures()
	{
		static const Library::SignatureList signatures =
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
		};

		return signatures;
	}

	AttributedFoo::AttributedFoo(const IdType typeId, const int data) : Attributed(typeId),
		mInt(data), mFloat(static_cast<float>(data)),
		mVector(static_cast<float>(data)), mMatrix(static_cast<float>(data)),
		mString(std::to_string(data)), foo(data), mIntArray{ data, data },
		mFloatArray{ static_cast<float>(data), static_cast<float>(data) },
		mVectorArray{ { data, data, data, data }, { data, data, data, data } },
		mMatrixArray{	{{ data, 0, 0, 0 }, { 0, data, 0, 0 }, { 0, 0, data, 0 }, { 0, 0, 0, data }},
						{{ data, 0, 0, 0 }, { 0, data, 0, 0 }, { 0, 0, data, 0 }, { 0, 0, 0, data }} },
		mStringArray{ std::to_string(data), std::to_string(data) },
		foo1(data), foo2(data)
	{
	}
}