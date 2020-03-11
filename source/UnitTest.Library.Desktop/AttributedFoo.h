#pragma once

#include <memory>

#include "Attributed.h"
#include "Foo.h"

namespace UnitTests
{
	class AttributedFoo : public Library::Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, Library::Attributed)

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
		int mInt{ 0 };
		float mFloat{ 0.0f };
		glm::vec4 mVector{ 0.0f };
		glm::mat4 mMatrix{ 0.0f };
		std::string mString{ "0" };

		Foo foo{ 0 };
		RTTI* mRtti{ &foo };

		int mIntArray[2]{ 0 };
		float mFloatArray[2]{ 0.0f };
		glm::vec4 mVectorArray[2]{	{ 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }	};
		glm::mat4 mMatrixArray[2]{	{{ 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }},
									{{ 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }}	};
		std::string mStringArray[2]{ "0", "0" };
		
		Foo foo1{ 0 };
		Foo foo2{ 0 };
		RTTI* mRttiArray[2]{ &foo1, &foo2 };
	};
}
