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
		AttributedFoo(AttributedFoo&& rhs) noexcept;
		AttributedFoo& operator=(const AttributedFoo& rhs);
		AttributedFoo& operator=(AttributedFoo&& rhs) noexcept;
		virtual ~AttributedFoo() = default;

		bool operator==(const AttributedFoo& rhs) const noexcept;
		bool operator!=(const AttributedFoo& rhs) const noexcept;

		int& Data();
		int Data() const;
		void SetData(int data);

		virtual bool Equals(const RTTI* rhs) const override;
		virtual std::string ToString() const override;

		virtual gsl::owner<Library::Scope*> Clone() const override;
		static const Library::TypeManager::TypeInfo& TypeInfo();

	private:
		std::unique_ptr<int> mIntData;
		std::unique_ptr<float> mFloatData;
		std::unique_ptr<glm::vec4> mVectorData;
		std::unique_ptr<glm::mat4> mMatrixData;
		std::unique_ptr<std::string> mStringData;
		std::unique_ptr<RTTI> mRttiData;

		Foo foo;
	};
}
