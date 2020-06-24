#pragma once

#include <memory>

#include "Attributed.h"

namespace UnitTests
{
	class AttributedBar final : public Library::Attributed
	{
		RTTI_DECLARATIONS(AttributedBar, Library::Attributed)

	public:
		explicit AttributedBar(int data = 0);
		AttributedBar(const AttributedBar& rhs);
		AttributedBar(AttributedBar&& rhs) noexcept;
		AttributedBar& operator=(const AttributedBar& rhs);
		AttributedBar& operator=(AttributedBar&& rhs) noexcept;
		~AttributedBar() = default;

		int& Data();
		int Data() const;
		void SetData(int data);

		virtual bool Equals(const RTTI* rhs) const override;
		virtual std::string ToString() const override;

		virtual gsl::owner<Library::Scope*> Clone() const override;
		static const Library::SignatureList& Signatures();

	private:
		int mIntData;
	};
}
