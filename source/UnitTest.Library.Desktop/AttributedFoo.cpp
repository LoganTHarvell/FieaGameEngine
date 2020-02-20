#include "pch.h"
#include "AttributedFoo.h"

namespace UnitTests
{
	RTTI_DEFINITIONS(AttributedFoo);

	AttributedFoo::AttributedFoo(int data) : 
		Attributed(AttributedFoo::TypeIdClass()), mIntData(std::make_unique<int>(data)),
		mFloatData(std::make_unique<float>(static_cast<float>(data))), mVectorData(std::make_unique<glm::vec4>(static_cast<float>(data))),
		mMatrixData(std::make_unique<glm::mat4>(static_cast<float>(data))), mRttiData(std::make_unique<Foo>(data))
	{
	}

	AttributedFoo::AttributedFoo(const AttributedFoo& rhs) :
		Attributed(rhs), mIntData(std::make_unique<int>(*rhs.mIntData))
	{
	}

	AttributedFoo::AttributedFoo(AttributedFoo&& rhs) noexcept :
		Attributed(std::move(rhs)), mIntData(std::move(rhs.mIntData))
	{
		rhs.mIntData = nullptr;
	}

	AttributedFoo& AttributedFoo::operator=(const AttributedFoo& rhs)
	{
		Attributed::operator=(rhs);

		if (this != &rhs)
		{
			*mIntData = *rhs.mIntData;
		}

		return *this;
	}

	AttributedFoo& AttributedFoo::operator=(AttributedFoo&& rhs) noexcept
	{
		Attributed::operator=(std::move(rhs));

		if (this != &rhs)
		{
			mIntData = std::move(rhs.mIntData);
			rhs.mIntData = nullptr;
		}

		return *this;
	}

	bool AttributedFoo::operator==(const AttributedFoo& rhs) const noexcept
	{
		return *mIntData == *rhs.mIntData;
	}

	bool AttributedFoo::operator!=(const AttributedFoo& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	int& AttributedFoo::Data()
	{
		return *mIntData;
	}

	int AttributedFoo::Data() const
	{
		return *mIntData;
	}
	
	void AttributedFoo::SetData(int data)
	{
		*mIntData = data;
	}

	bool AttributedFoo::Equals(const RTTI* rhs) const
	{
		if (!rhs) return false;

		const AttributedFoo* rhsAttributedFoo = rhs->As<AttributedFoo>();
		return rhsAttributedFoo ? *mIntData == rhsAttributedFoo->Data() : false;
	}

	std::string AttributedFoo::ToString() const
	{
		return "AttributedFoo: " + std::to_string(*mIntData);
	}

	gsl::owner<Library::Scope*> AttributedFoo::Clone() const
	{
		return new AttributedFoo(*this);
	}

	Library::TypeManager::SignatureListType AttributedFoo::Signatures()
	{
		return Library::TypeManager::SignatureListType
		{
			{ "integer", DataType::Types::Integer, offsetof(AttributedFoo, mIntData), false, 1 },
			{ "float", DataType::Types::Integer, offsetof(AttributedFoo, mFloatData), false, 1 },
			{ "vector", DataType::Types::Integer, offsetof(AttributedFoo, mVectorData), false, 1 },
			{ "matrix", DataType::Types::Integer, offsetof(AttributedFoo, mMatrixData), false, 1 },
			{ "string", DataType::Types::Integer, offsetof(AttributedFoo, mStringData), false, 1 },
			{ "scope", DataType::Types::Integer, 0, true, 1 },
			{ "rttiPtr", DataType::Types::Integer, offsetof(AttributedFoo, mRttiData), false, 1 }
		};
	}
}