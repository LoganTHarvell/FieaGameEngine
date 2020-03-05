#pragma once

#include <memory>

#include "RTTI.h"

namespace UnitTests
{
	class Foo final : public Library::RTTI
	{
		RTTI_DECLARATIONS(Foo, Library::RTTI)

	public:
		explicit Foo(int data = 0);
		Foo(const Foo& rhs);
		Foo(Foo&& rhs) noexcept;
		Foo& operator=(const Foo& rhs);
		Foo& operator=(Foo&& rhs) noexcept;
		~Foo() = default;


		bool operator==(const Foo& rhs) const noexcept;
		bool operator!=(const Foo& rhs) const noexcept;

		virtual bool Equals(const RTTI* rhs) const override;
		virtual std::string ToString() const override;

		int& Data();
		int Data() const;
		void SetData(int data);

	private:
		std::unique_ptr<int> mData;
	};
}
