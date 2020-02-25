#pragma once

#include <memory>

#include "Attributed.h"
#include "Foo.h"

namespace UnitTests
{
	class AttributedFoo : public Library::Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, Library::Attributed);

	public:
		explicit AttributedFoo(int data = 0);
		AttributedFoo(const AttributedFoo& rhs);
		AttributedFoo(AttributedFoo&& rhs) noexcept = default;
		AttributedFoo& operator=(const AttributedFoo& rhs);
		AttributedFoo& operator=(AttributedFoo&& rhs) noexcept = default;
		virtual ~AttributedFoo() = default;

		virtual std::string ToString() const override;

		virtual gsl::owner<Library::Scope*> Clone() const override;
		static const Library::TypeManager::TypeInfo& TypeInfo();

	private:
		int mIntData{ 0 };
		float mFloatData{ 0.0f };
		glm::vec4 mVectorData{ 0.0f };
		glm::mat4 mMatrixData{ 0.0f };
		std::string mStringData{ "0" };
		
		Foo foo{ 0 };
		RTTI* mRttiData;

	};
}
