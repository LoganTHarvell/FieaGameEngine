#include "pch.h"

#include "ToStringSpecialization.h"
#include "Foo.h"
#include "Bar.h"
#include "Datum.h"


using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;
using namespace Library;

namespace UnitTests
{
	template<typename T>
	void TestConstructors(const std::initializer_list<T> data)
	{
		Datum emptyDatum;
		Assert::AreEqual(0_z, emptyDatum.Size());
		Assert::AreEqual(0_z, emptyDatum.Capacity());
		Assert::AreEqual(emptyDatum.Type(), Datum::Types::Unknown);

		Datum datum1 = *data.begin();
		Assert::AreEqual(1_z, datum1.Size());
		Assert::AreEqual(1_z, datum1.Capacity());
		Assert::AreEqual(datum1.Type(), Datum::TypeOf<T>());

		datum1 = data;
		Assert::AreEqual(data.size(), datum1.Size());
		Assert::AreEqual(data.size(), datum1.Capacity());
		Assert::AreEqual(datum1.Type(), Datum::TypeOf<T>());

		Datum datum2 = data;
		Assert::AreEqual(data.size(), datum2.Size());
		Assert::AreEqual(data.size(), datum2.Capacity());
		Assert::AreEqual(datum2.Type(), Datum::TypeOf<T>());

		datum2 = T();
		Assert::AreEqual(data.size(), datum2.Size());
		Assert::AreEqual(data.size(), datum2.Capacity());
		Assert::AreEqual(datum2.Type(), Datum::TypeOf<T>());

		if (Datum::TypeOf<T>() != Datum::Types::Integer)
		{
			Assert::ExpectException<std::runtime_error>([&datum1] { datum1 = int(); });
		}
		else
		{
			Assert::ExpectException<std::runtime_error>([&datum1] { datum1 = float(); });
		}

		T tmp[1] = { T() };
		Datum externalStorage;
		externalStorage.SetStorage(gsl::span<T>(tmp));
		Assert::ExpectException<std::runtime_error>([&externalStorage, &data] { externalStorage = data; });

		Datum datumCopy(externalStorage);
		Assert::AreEqual(datumCopy, externalStorage);

		Datum datumAssign;
		datumAssign = externalStorage;
		Assert::AreEqual(datumAssign, externalStorage);
	}

	template<typename T>
	void TestCopy(const std::initializer_list<T> data)
	{
		Datum datum = data;
		
		Datum copy = datum;
		Assert::AreEqual(datum, copy);

		copy = datum;
		Assert::AreEqual(datum, copy);
	}

	template<typename T>
	void TestMove(const std::initializer_list<T> data)
	{
		Datum datum = data;
		Datum copy = datum;

		Datum moved(std::move(datum));
		Assert::AreEqual(moved, copy);

		datum = copy;

		moved = std::move(datum);
		Assert::AreEqual(moved, copy);
	}

	template<typename T>
	void TestEquality(const std::initializer_list<T> data, const T& differentData)
	{
		Datum datum1;
		Datum datum2;
		Assert::AreEqual(datum1, datum2);

		datum1 = data;
		Assert::IsTrue(datum1 != datum2);
		datum2 = data;
		Assert::AreEqual(datum1, datum2);

		datum1 = data;
		datum2 = differentData;
		Assert::IsTrue(datum1 != datum2);

		for (const auto& value : data)
		{
			datum2 = value;
			Assert::IsTrue(datum2 == value);
			Assert::IsFalse(datum2 != value);
		}
	}

	template<typename T>
	void TestTypeSizeCapacity(const std::initializer_list<T> data)
	{
		Datum datum;
		Assert::AreEqual(Datum::Types::Unknown, datum.Type());
		Assert::AreEqual(0_z, datum.Size());
		Assert::IsTrue(datum.IsEmpty());
		Assert::AreEqual(0_z, datum.Capacity());

		datum.SetType(Datum::TypeOf<T>());
		Assert::AreEqual(Datum::TypeOf<T>(), datum.Type());

		if (Datum::TypeOf<T>() != Datum::Types::Integer)
		{
			Assert::ExpectException<std::runtime_error>([&datum] { datum.SetType(Datum::Types::Integer); });
		}
		else
		{
			Assert::ExpectException<std::runtime_error>([&datum] { datum.SetType(Datum::Types::Float); });
		}

		datum = data;
		Assert::AreEqual(Datum::TypeOf<T>(), datum.Type());
		Assert::AreEqual(data.size(), datum.Size());
		Assert::IsFalse(datum.IsEmpty());
		Assert::AreEqual(data.size(), datum.Capacity());

		datum.Reserve(10);
		Assert::AreEqual(data.size(), datum.Size());
		Assert::AreEqual(10_z, datum.Capacity());

		datum.Resize(10);
		datum.SetReserveStrategy(Datum::DefaultReserveFunctor());
		datum.PushBack(*data.begin());
		Assert::AreEqual(15_z, datum.Capacity());
	}

	template<typename T>
	void TestResize(const std::initializer_list<T> data)
	{
		Datum datum = data;

		datum.Resize(10);
		Assert::AreEqual(10_z, datum.Size());
		Assert::AreEqual(10_z, datum.Capacity());

		datum.Resize(3);
		Assert::AreEqual(3_z, datum.Size());
		Assert::AreEqual(10_z, datum.Capacity());
	}

	template<typename T>
	void TestShrinkToFit(const std::initializer_list<T> data)
	{
		{
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum] { datum.ShrinkToFit(); });
		}

		Datum datum = data;

		datum.Resize(10);
		Assert::AreEqual(10_z, datum.Size());
		Assert::AreEqual(10_z, datum.Capacity());

		datum.Resize(5);
		Assert::AreEqual(5_z, datum.Size());
		Assert::AreEqual(10_z, datum.Capacity());
		datum.ShrinkToFit();
		Assert::AreEqual(5_z, datum.Size());
		Assert::AreEqual(5_z, datum.Capacity());

		datum.Resize(0);
		datum.ShrinkToFit();
		Assert::AreEqual(0_z, datum.Size());
		Assert::AreEqual(0_z, datum.Capacity());
	}

	template<typename T>
	void TestElementAccessors(const std::initializer_list<T> data, const T& notFoundData)
	{
		Datum datum;
		
		auto it = data.begin();
		for (std::size_t i = 0; i < data.size(); ++i, ++it)
		{
			datum.PushBack(T());
			Assert::AreEqual(*it, datum.Set(*it, i));
			Assert::AreEqual(*it, datum.Get<T>(i));
		}

		Assert::AreEqual(*data.begin(), datum.Front<T>());
		Assert::AreEqual(*(data.end()-1), datum.Back<T>());

		const Datum datumConst = datum;

		it = data.begin();
		for (std::size_t i = 0; i < data.size(); ++i, ++it)
		{
			Assert::AreEqual(*it, datumConst.Get<T>(i));
		}

		Assert::AreEqual(*data.begin(), datumConst.Front<T>());
		Assert::AreEqual(*(data.end()-1), datumConst.Back<T>());

		Assert::AreEqual(*(data.end() - 1), *datum.Find(*(data.end() - 1)));
		Assert::IsTrue(datum.Find(notFoundData) == nullptr);

		const Datum constDatum = datum;
		Assert::AreEqual(*(data.end() - 1), *constDatum.Find(*(data.end() - 1)));
		Assert::IsTrue(constDatum.Find(notFoundData) == nullptr);
	}

	template<typename T>
	void TestSetStorage(gsl::span<T> storage)
	{
		Datum datum;
		datum.SetStorage(storage);

		Assert::AreEqual(Datum::TypeOf<T>(), datum.Type());
		Assert::AreEqual(static_cast<std::size_t>(storage.size()), datum.Size());
		Assert::AreEqual(static_cast<std::size_t>(storage.size()), datum.Capacity());

		for (std::ptrdiff_t i = 0; i < storage.size(); ++i)
		{
			Assert::AreEqual(storage[i], datum.Get<T>(i));
		}

		if (Datum::TypeOf<T>() != Datum::Types::Integer)
		{
			Assert::ExpectException<std::runtime_error>([&datum] {	int unknown[5] = { 1, 2, 3, 4, 5 };
																	datum.SetStorage(gsl::span<int>(unknown));	});
		}
		else
		{
			Assert::ExpectException<std::runtime_error>([&datum] {	float unknown[5] = { 1, 2, 3, 4, 5 };
																	datum.SetStorage(gsl::span<float>(unknown));	});
		}
	}

	template<typename T>
	void TestPushBack(const std::initializer_list<T> data)
	{
		Datum datum;

		auto it = data.begin();
		for (std::size_t i = 0; i < data.size(); ++i, ++it)
		{
			std::size_t capacity;

			if (datum.Size() < datum.Capacity())
			{
				capacity = datum.Capacity();
			}
			else
			{
				capacity = std::max(static_cast<std::size_t>(datum.Capacity() * 1.5), datum.Capacity() + 1_z);
			}

			datum.PushBack(*it);
			Assert::AreEqual(*it, datum.Get<T>(i));
			Assert::AreEqual(i+1, datum.Size());
			Assert::AreEqual(capacity, datum.Capacity());

			if (Datum::TypeOf<T>() != Datum::Types::Integer)
			{
				Assert::ExpectException<std::runtime_error>([&datum] { datum.PushBack(int()); });
			}
			else
			{
				Assert::ExpectException<std::runtime_error>([&datum] { datum.PushBack(float()); });
			}
		}
	}

	template<typename T>
	void TestPopBack(const std::initializer_list<T> data)
	{
		Datum datum = data;

		for (std::size_t i = 0; i < data.size(); ++i)
		{
			datum.PopBack();
			Assert::AreEqual(data.size() - i - 1, datum.Size());
			
			auto it = data.begin();
			for (std::size_t j = 0; j < datum.Size(); ++j, ++it)
			{
				Assert::AreEqual(*it, datum.Get<T>(j));
			}
		}
	}

	template<typename T>
	void TestRemove(const std::initializer_list<T> data, const T& notFoundData)
	{
		Datum datum = data;

		for (std::size_t i = 0; i < data.size(); ++i)
		{
			datum.Remove(*(data.end()-i-1));
			Assert::AreEqual(data.size() - i - 1, datum.Size());

			auto it = data.begin();
			for (std::size_t j = 0; j < datum.Size(); ++j, ++it)
			{
				Assert::AreEqual(*it, datum.Get<T>(j));
			}
		}

		datum = data;

		for (std::size_t i = 0; i < data.size(); ++i)
		{
			datum.RemoveAt(datum.Size() - 1);
			Assert::AreEqual(data.size() - i - 1, datum.Size());

			auto it = data.begin();
			for (std::size_t j = 0; j < datum.Size(); ++j, ++it)
			{
				Assert::AreEqual(*it, datum.Get<T>(j));
			}
		}

		Assert::IsFalse(datum.Remove(notFoundData));
	}

	template<typename T>
	void TestClear(const std::initializer_list<T> data)
	{
		Datum datum = data;
		
		datum.Clear();
		Assert::AreEqual(0_z, datum.Size());
		Assert::AreEqual(data.size(), datum.Capacity());
	}

	template<typename T>
	void TestStringConversion(const std::initializer_list<T> data)
	{
		Datum datum = data;

		Datum datumToSet;
		datumToSet.SetType(datum.Type());
		datumToSet.Resize(data.size());

		for (std::size_t i = 0; i < data.size(); ++i)
		{
			datumToSet.SetFromString(datum.ToString(i), i);
		}

		if (Datum::TypeOf<T>() != Datum::Types::Pointer)
		{
			Assert::AreEqual(datum, datumToSet);
		}
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DatumTest)
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

		TEST_METHOD(TypeOf)
		{
			Assert::AreEqual(Datum::Types::Unknown, Datum::TypeOf<double>());
			Assert::AreEqual(Datum::Types::Integer, Datum::TypeOf<int>());
			Assert::AreEqual(Datum::Types::Float, Datum::TypeOf<float>());
			Assert::AreEqual(Datum::Types::Vector, Datum::TypeOf<glm::vec4>());
			Assert::AreEqual(Datum::Types::Matrix, Datum::TypeOf<glm::mat4>());
			Assert::AreEqual(Datum::Types::String, Datum::TypeOf<std::string>());
			Assert::AreEqual(Datum::Types::Pointer, Datum::TypeOf<RTTI*>());
		}

 		TEST_METHOD(Constructors)
 		{
			TestConstructors<int>({ 10, 20, 30 });
			TestConstructors<float>({ 10, 20, 30 });
			TestConstructors<glm::vec4>({ glm::vec4(10), glm::vec4(20), glm::vec4(30) });
			TestConstructors<glm::mat4>({ glm::mat4(10), glm::mat4(20), glm::mat4(30) });
			TestConstructors<std::string>({ "10", "20", "30" });

			Foo a(10), b(20), c(30);
			TestConstructors<RTTI*>({ &a, &b, &c });
			TestConstructors<RTTI*>({ nullptr, nullptr, nullptr });
		}

 		TEST_METHOD(Copy)
 		{
 			TestCopy<int>({ 10, 20, 30 });
 			TestCopy<float>({ 10, 20, 30 });
			TestCopy<glm::vec4>({ glm::vec4(10), glm::vec4(20), glm::vec4(30) });
			TestCopy<glm::mat4>({ glm::mat4(10), glm::mat4(20), glm::mat4(30) });
			TestCopy<std::string>({ "10", "20", "30" });

			Foo a(10), b(20), c(30);
			TestCopy<RTTI*>({ &a, &b, &c });
			TestCopy<RTTI*>({ nullptr, nullptr, nullptr });
		}

		TEST_METHOD(Move)
		{
			TestMove<int>({ 10, 20, 30 });
			TestMove<float>({ 10, 20, 30 });
			TestMove<glm::vec4>({ glm::vec4(10), glm::vec4(20), glm::vec4(30) });
			TestMove<glm::mat4>({ glm::mat4(10), glm::mat4(20), glm::mat4(30) });
			TestMove<std::string>({ "10", "20", "30" });

			Foo a(10), b(20), c(30);
			TestMove<RTTI*>({ &a, &b, &c });
			TestMove<RTTI*>({ nullptr, nullptr, nullptr });
		}

		TEST_METHOD(Equality)
		{
			TestEquality<int>({ 10, 20, 30 }, 40);
			TestEquality<float>({ 10, 20, 30 }, 40);
			TestEquality<glm::vec4>({ glm::vec4(10), glm::vec4(20), glm::vec4(30) }, glm::vec4(40));
			TestEquality<glm::mat4>({ glm::mat4(10), glm::mat4(20), glm::mat4(30) }, glm::mat4(40));
			TestEquality<std::string>({ "10", "20", "30" }, "40");
 
 			Foo a(10), b(20), c(30);
 			TestEquality<RTTI*>({ &a, &b, &c }, nullptr);
 			TestEquality<RTTI*>({ nullptr, nullptr, nullptr }, &a);
		}

		TEST_METHOD(TypeSizeCapacity)
		{
			TestTypeSizeCapacity<int>({ 10, 20, 30 });
			TestTypeSizeCapacity<float>({ 10, 20, 30 });
			TestTypeSizeCapacity<glm::vec4>({ glm::vec4(10), glm::vec4(20), glm::vec4(30) });
			TestTypeSizeCapacity<glm::mat4>({ glm::mat4(10), glm::mat4(20), glm::mat4(30) });
			TestTypeSizeCapacity<std::string>({ "10", "20", "30" });

			Foo a(10), b(20), c(30);
			TestTypeSizeCapacity<RTTI*>({ &a, &b, &c });
			TestTypeSizeCapacity<RTTI*>({ nullptr, nullptr, nullptr });
		}

		TEST_METHOD(Resize)
		{
			TestResize<int>({ 10, 20, 30 });
			TestResize<float>({ 10, 20, 30 });
			TestResize<glm::vec4>({ glm::vec4(10), glm::vec4(20), glm::vec4(30) });
			TestResize<glm::mat4>({ glm::mat4(10), glm::mat4(20), glm::mat4(30) });
			TestResize<std::string>({ "10", "20", "30" });

			Foo a(10), b(20), c(30);
			TestResize<RTTI*>({ &a, &b, &c });
			TestResize<RTTI*>({ nullptr, nullptr, nullptr });
		}

		TEST_METHOD(ShrinkToFit)
		{
			TestShrinkToFit<int>({ 10, 20, 30 });
			TestShrinkToFit<float>({ 10, 20, 30 });
			TestShrinkToFit<glm::vec4>({ glm::vec4(10), glm::vec4(20), glm::vec4(30) });
			TestShrinkToFit<glm::mat4>({ glm::mat4(10), glm::mat4(20), glm::mat4(30) });
			TestShrinkToFit<std::string>({ "10", "20", "30" });

			Foo a(10), b(20), c(30);
			TestShrinkToFit<RTTI*>({ &a, &b, &c });
			TestShrinkToFit<RTTI*>({ nullptr, nullptr, nullptr });
		}

		TEST_METHOD(ElementAccessors)
		{
			TestElementAccessors<int>({ 10, 20, 30 }, 40);
			TestElementAccessors<float>({ 10, 20, 30 }, 40);
			TestElementAccessors<glm::vec4>({ glm::vec4(10), glm::vec4(20), glm::vec4(30) }, glm::vec4(40));
			TestElementAccessors<glm::mat4>({ glm::mat4(10), glm::mat4(20), glm::mat4(30) }, glm::mat4(40));
			TestElementAccessors<std::string>({ "10", "20", "30" }, "40");

			Foo a(10), b(20), c(30);
			TestElementAccessors<RTTI*>({ &a, &b, &c }, nullptr);
			TestElementAccessors<RTTI*>({ nullptr, nullptr, nullptr }, &a);
		}

		TEST_METHOD(SetStorage)
		{
			int dataInt[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
			float dataFloat[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
			std::string dataString[10] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };

			Foo a(1), b(2), c(3), d(4), e(5), f(6), g(7), h(8), i(9), j(10);
			RTTI* dataFoo[10] = { &a, &b, &c, &d, &e, &f, &g, &h, &i, &j };
			RTTI* dataNullptr[10] = { nullptr, nullptr, nullptr, nullptr, nullptr };

			TestSetStorage<int>(dataInt);
			TestSetStorage<float>(dataFloat);
			TestSetStorage<std::string>(dataString);
			TestSetStorage<RTTI*>(dataFoo);
			TestSetStorage<RTTI*>(dataNullptr);
		}

		TEST_METHOD(PushBack)
		{
			TestPushBack<int>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
			TestPushBack<float>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
			TestPushBack<glm::vec4>({ glm::vec4(10), glm::vec4(20), glm::vec4(30) });
			TestPushBack<glm::mat4>({ glm::mat4(10), glm::mat4(20), glm::mat4(30) });
			TestPushBack<std::string>({ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});

			Foo a(1), b(2), c(3), d(4), e(5), f(6), g(7), h(8), i(9), j(10);
			TestPushBack<RTTI*>({ &a, &b, &c, &d, &e, &f, &g, &h, &i, &j });
			TestPushBack<RTTI*>({ nullptr, nullptr, nullptr, nullptr, nullptr });
		}

		TEST_METHOD(PopBack)
		{
			TestPopBack<int>({ 10, 20, 30 });
			TestPopBack<float>({ 10, 20, 30 });
			TestPopBack<glm::vec4>({ glm::vec4(10), glm::vec4(20), glm::vec4(30) });
			TestPopBack<glm::mat4>({ glm::mat4(10), glm::mat4(20), glm::mat4(30) });
			TestPopBack<std::string>({ "10", "20", "30" });

			Foo a(10), b(20), c(30);
			TestPopBack<RTTI*>({ &a, &b, &c });
			TestPopBack<RTTI*>({ nullptr, nullptr, nullptr });
		}

		TEST_METHOD(Remove)
		{
			TestRemove<int>({ 10, 20, 30 }, 40);
			TestRemove<float>({ 10, 20, 30 }, 40);
			TestRemove<glm::vec4>({ glm::vec4(10), glm::vec4(20), glm::vec4(30) }, glm::vec4(40));
			TestRemove<glm::mat4>({ glm::mat4(10), glm::mat4(20), glm::mat4(30) }, glm::mat4(40));
			TestRemove<std::string>({ "10", "20", "30" }, "40");

			Foo a(10), b(20), c(30);
			TestRemove<RTTI*>({ &a, &b, &c }, nullptr);
			TestRemove<RTTI*>({ nullptr, nullptr, nullptr }, &a);
		}

		TEST_METHOD(Clear)
		{
			TestClear<int>({ 10, 20, 30 });
			TestClear<float>({ 10, 20, 30 });
			TestClear<glm::vec4>({ glm::vec4(10), glm::vec4(20), glm::vec4(30) });
			TestClear<glm::mat4>({ glm::mat4(10), glm::mat4(20), glm::mat4(30) });
			TestClear<std::string>({ "10", "20", "30" });

			Foo a(10), b(20), c(30);
			TestClear<RTTI*>({ &a, &b, &c });
			TestClear<RTTI*>({ nullptr, nullptr, nullptr });
		}

		TEST_METHOD(StringConversion)
		{
			TestStringConversion<int>({ 10, 20, 30 });
			TestStringConversion<float>({ 10, 20, 30 });
			TestStringConversion<glm::vec4>({ glm::vec4(10), glm::vec4(20), glm::vec4(30) });
			TestStringConversion<glm::mat4>({ glm::mat4(10), glm::mat4(20), glm::mat4(30) });
			TestStringConversion<std::string>({ "10", "20", "30" });

			Foo a(10), b(20), c(30);
			TestStringConversion<RTTI*>({ &a, &b, &c });
			TestStringConversion<RTTI*>({ nullptr, nullptr, nullptr });
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState DatumTest::sStartMemState;
}