#pragma once

#include "Attributed.h"
#include "Foo.h"

namespace UnitTests
{
	class AttributedFoo : public Library::Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, Library::Attributed)

	public:
		explicit AttributedFoo(const int data = 0);
		AttributedFoo(const AttributedFoo& rhs);
		AttributedFoo(AttributedFoo&& rhs) noexcept;
		AttributedFoo& operator=(const AttributedFoo& rhs);
		AttributedFoo& operator=(AttributedFoo&& rhs) noexcept;
		virtual ~AttributedFoo() = default;

		virtual std::string ToString() const override;

		virtual gsl::owner<Library::Scope*> Clone() const override;
		static const Library::SignatureList& Signatures();

	protected:
		explicit AttributedFoo(const IdType typeId, const int data = 0);

	private:
		int mInt;
		float mFloat;
		glm::vec4 mVector;
		glm::mat4 mMatrix;
		std::string mString;

		Foo foo;
		RTTI* mRtti{ &foo };

		int mIntArray[2];
		float mFloatArray[2];
		glm::vec4 mVectorArray[2];
		glm::mat4 mMatrixArray[2];
		std::string mStringArray[2];

		Foo foo1;
		Foo foo2;
		RTTI* mRttiArray[2]{ &foo1, &foo2 };

		int* mInvalidInt{ nullptr };
	};
}
