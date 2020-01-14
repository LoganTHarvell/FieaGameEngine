#pragma once

#include <memory>

namespace UnitTests
{
	class Bar
	{
	public:
		explicit Bar(int data = 0);
		Bar(const Bar& rhs);
		Bar(Bar&& rhs) noexcept;
		Bar& operator=(const Bar& rhs);
		Bar& operator=(Bar&& rhs) noexcept;
		~Bar() = default;

		int& Data();
		int Data() const;
		void SetData(int data);

	private:
		std::unique_ptr<int> mData;
	};
}
