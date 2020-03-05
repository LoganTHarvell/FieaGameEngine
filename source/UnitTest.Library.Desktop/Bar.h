#pragma once

#include <memory>

#include "RTTI.h"

namespace UnitTests
{
	class Bar final : public Library::RTTI
	{
		RTTI_DECLARATIONS(Bar, Library::RTTI)

	public:
		explicit Bar(int data = 0);
		Bar(const Bar& rhs);
		Bar(Bar&& rhs) noexcept;
		Bar& operator=(const Bar& rhs);
		Bar& operator=(Bar&& rhs) noexcept;
		~Bar() = default;

		virtual bool Equals(const RTTI* rhs) const override;
		virtual std::string ToString() const override;

		int& Data();
		int Data() const;
		void SetData(int data);

	private:
		std::unique_ptr<int> mData;
	};
}
