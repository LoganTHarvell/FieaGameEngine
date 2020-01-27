#include "pch.h"

#include "DefaultHash.h"
#include "Foo.h"
#include "Bar.h"


using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;
using namespace Library;


namespace Library
{
	template<>
	inline std::size_t DefaultHash<Foo>::operator()(const Foo& key) const
	{
		int fooData = key.Data();
		const uint8_t* data = reinterpret_cast<const uint8_t*>(&fooData);
		return AdditiveHash(data, sizeof(int));
	}

	template<>
	inline std::size_t DefaultHash<Bar>::operator()(const Bar& key) const
	{
		int fooData = key.Data();
		const uint8_t* data = reinterpret_cast<const uint8_t*>(&fooData);
		return AdditiveHash(data, sizeof(int));
	}

	template<typename T>
	void TestHash()
	{
		DefaultHash<T> hash;

		T a = T(10);
		T b = T(20);
		T c(a);

		Assert::AreEqual(hash(a), hash(a));
		Assert::IsTrue(hash(a) != hash(b));
		Assert::AreEqual(hash(a), hash(c));
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

		TEST_METHOD(Primitives)
		{
			TestHash<int>();
			TestHash<double>();
		}

		TEST_METHOD(FooBarHash)
		{
			TestHash<Foo>();
			TestHash<Bar>();
		}

		TEST_METHOD(StringHash)
		{
			{
				DefaultHash<std::string> hash;
	
				std::string a = "Hello"s;
				std::string b = "Goodbye"s;
				std::string c(a);
	
				Assert::AreEqual(hash(a), hash(a));
				Assert::IsTrue(hash(a) != hash(b));
				Assert::AreEqual(hash(a), hash(c));
			}

			{
				DefaultHash<const std::string> hash;
	
				std::string a = "Hello"s;
				std::string b = "Goodbye"s;
				std::string c(a);
	
				Assert::AreEqual(hash(a), hash(a));
				Assert::IsTrue(hash(a) != hash(b));
				Assert::AreEqual(hash(a), hash(c));
			}
		}

		TEST_METHOD(WideStringHash)
		{
			{
				DefaultHash<std::wstring> hash;
	
				std::wstring a = L"Hello"s;
				std::wstring b = L"Goodbye"s;
				std::wstring c(a);
	
				Assert::AreEqual(hash(a), hash(a));
				Assert::IsTrue(hash(a) != hash(b));
				Assert::AreEqual(hash(a), hash(c));
			}

			{
				DefaultHash<const std::wstring> hash;
	
				std::wstring a = L"Hello"s;
				std::wstring b = L"Goodbye"s;
				std::wstring c(a);
	
				Assert::AreEqual(hash(a), hash(a));
				Assert::IsTrue(hash(a) != hash(b));
				Assert::AreEqual(hash(a), hash(c));
			}
		}

		TEST_METHOD(CharacterStringHash)
		{
			{
				DefaultHash<char *> hash;
	
				char* a = "Hello";
				char* b = "Goodbye";
				char* c(a);
	
				Assert::AreEqual(hash(a), hash(a));
				Assert::IsTrue(hash(a) != hash(b));
				Assert::AreEqual(hash(a), hash(c));
			}

			{
				DefaultHash<const char *> hash;
	
				const char* a = "Hello";
				const char* b = "Goodbye";
				const char* c(a);
	
				Assert::AreEqual(hash(a), hash(a));
				Assert::IsTrue(hash(a) != hash(b));
				Assert::AreEqual(hash(a), hash(c));
			}

			{
				DefaultHash<char * const> hash;
	
				char* const a = "Hello";
				char* const b = "Goodbye";
				char* const c(a);
	
				Assert::AreEqual(hash(a), hash(a));
				Assert::IsTrue(hash(a) != hash(b));
				Assert::AreEqual(hash(a), hash(c));
			}

			{
				DefaultHash<const char * const> hash;
	
				const char* const a = "Hello";
				const char* const b = "Goodbye";
				const char* const c(a);
	
				Assert::AreEqual(hash(a), hash(a));
				Assert::IsTrue(hash(a) != hash(b));
				Assert::AreEqual(hash(a), hash(c));
			}
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState DefaultHashTest::sStartMemState;
}