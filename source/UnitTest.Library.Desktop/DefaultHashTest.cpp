#include "pch.h"

#include "DefaultHash.h"
#include "Foo.h"


using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;
using namespace Library;


namespace Library
{
	template<>
	inline const std::size_t DefaultHash<Foo>::operator()(const Foo& key) const
	{
		int fooData = key.Data();
		const uint8_t* data = reinterpret_cast<const uint8_t*>(&fooData);
		return AdditiveHash(data, sizeof(int));
	}
}


namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DefaultHashTest)
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

		TEST_METHOD(IntHash)
		{
			DefaultHash<int> hash;

			int a = 10;
			int b = 20;
			int c(a);

			Assert::AreEqual(hash(a), hash(a));
			Assert::IsTrue(hash(a) != hash(b));
			Assert::AreEqual(hash(a), hash(c));
		}

		TEST_METHOD(DoubleHash)
		{
			DefaultHash<double> hash;

			double a = 10;
			double b = 20;
			double c(a);

			Assert::AreEqual(hash(a), hash(a));
			Assert::IsTrue(hash(a) != hash(b));
			Assert::AreEqual(hash(a), hash(c));
		}

		TEST_METHOD(FooHash)
		{
			DefaultHash<Foo> hash;

			Foo a = Foo(10);
			Foo b = Foo(20);
			Foo c(a);

			Assert::AreEqual(hash(a), hash(a));
			Assert::IsTrue(hash(a) != hash(b));
			Assert::AreEqual(hash(a), hash(c));
		}

		TEST_METHOD(StringHash)
		{
			DefaultHash<std::string> hash;

			std::string a = "Hello"s;
			std::string b = "Goodbye"s;
			std::string c(a);

			Assert::AreEqual(hash(a), hash(a));
			Assert::IsTrue(hash(a) != hash(b));
			Assert::AreEqual(hash(a), hash(c));
		}

		TEST_METHOD(WideStringHash)
		{
			DefaultHash<std::wstring> hash;

			std::wstring a = L"Hello"s;
			std::wstring b = L"Goodbye"s;
			std::wstring c(a);

			Assert::AreEqual(hash(a), hash(a));
			Assert::IsTrue(hash(a) != hash(b));
			Assert::AreEqual(hash(a), hash(c));
		}

		TEST_METHOD(CharacterStringHash)
		{
			DefaultHash<char *> hash;

			char* a = "Hello";
			char* b = "Goodbye";
			char* c(a);

			Assert::AreEqual(hash(a), hash(a));
			Assert::IsTrue(hash(a) != hash(b));
			Assert::AreEqual(hash(a), hash(c));
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState DefaultHashTest::sStartMemState;
}