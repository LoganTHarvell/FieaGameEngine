#include "pch.h"

#include "ToStringSpecialization.h"
#include "Foo.h"
#include "Bar.h"
#include "Stack.h"


using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;
using namespace Library;

namespace ContainerTests
{
	template<typename T>
	void TestPush(std::initializer_list<T> data)
	{
		Stack<T> stack;
		std::size_t size = 0;

		Assert::IsTrue(stack.IsEmpty());
		Assert::AreEqual(size, stack.Size());

		Assert::ExpectException<std::runtime_error>([&] { stack.Top(); });

		for (const auto& datum : data)
		{
			stack.Push(datum);
			Assert::IsFalse(stack.IsEmpty());
			Assert::AreEqual(++size, stack.Size());
			Assert::AreEqual(datum, stack.Top());
		}
	}

	template<typename T>
	void TestPop(std::initializer_list<T> data)
	{
		Stack<T> stack;
		std::size_t size = data.size();

		for (const auto& datum : data)
		{
			stack.Push(datum);
		}

		for (auto it = std::rbegin(data); it != std::rend(data); ++it)
		{
			stack.Pop();
			Assert::AreEqual((--size), stack.Size());

			if (it + 1 != std::rend(data))
			{
				Assert::AreEqual(*(it + 1), stack.Top());
			}
			else
			{
				Assert::IsTrue(stack.IsEmpty());
			}
		}
	}

	TEST_CLASS(StackTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(Push)
		{
			TestPush<int>({1, 2, 3});
		}

		TEST_METHOD(Pop)
		{
			TestPop<int>({1, 2, 3});
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState StackTest::sStartMemState;
}