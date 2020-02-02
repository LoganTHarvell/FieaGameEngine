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
		Assert::AreEqual(emptyDatum.Type(), Datum::DatumTypes::Unknown);

		Datum datum1 = *data.begin();
		Assert::AreEqual(1_z, datum1.Size());
		Assert::AreEqual(1_z, datum1.Capacity());
		Assert::AreEqual(datum1.Type(), Datum::TypeOf<T>());

		datum1 = data;
		Assert::AreEqual(3_z, datum1.Size());
		Assert::AreEqual(3_z, datum1.Capacity());
		Assert::AreEqual(datum1.Type(), Datum::TypeOf<T>());

		Datum datum2 = data;
		Assert::AreEqual(3_z, datum2.Size());
		Assert::AreEqual(3_z, datum2.Capacity());
		Assert::AreEqual(datum2.Type(), Datum::TypeOf<T>());

		datum2 = *data.begin();
		Assert::AreEqual(1_z, datum2.Size());
		Assert::AreEqual(1_z, datum2.Capacity());
		Assert::AreEqual(datum2.Type(), Datum::TypeOf<T>());
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
	void TestEquality(const std::initializer_list<T> data)
	{
		Datum datum1;
		Datum datum2;
		Assert::AreEqual(datum1, datum2);

		datum1 = data;
		datum2 = data;
		Assert::AreEqual(datum1, datum2);

		datum1 = data;
		datum2 = *data.begin();
		Assert::IsTrue(datum1 != datum2);

		Assert::IsTrue(datum2 == *data.begin());
		Assert::IsFalse(datum2 != *data.begin());
	}

	template<typename T>
	void TestTypeSizeCapacity(const std::initializer_list<T> data)
	{
		Datum datum;
		Assert::AreEqual(Datum::DatumTypes::Unknown, datum.Type());
		Assert::AreEqual(0_z, datum.Size());
		Assert::IsTrue(datum.IsEmpty());
		Assert::AreEqual(0_z, datum.Capacity());

		datum.SetType(Datum::TypeOf<T>());
		Assert::AreEqual(Datum::TypeOf<T>(), datum.Type());
		Assert::ExpectException<std::runtime_error>([&datum] { datum.SetType(Datum::TypeOf<T>()); });

		datum = data;
		Assert::AreEqual(Datum::TypeOf<T>(), datum.Type());
		Assert::AreEqual(data.size(), datum.Size());
		Assert::IsFalse(datum.IsEmpty());
		Assert::AreEqual(data.size(), datum.Capacity());

		datum.Reserve(10);
		Assert::AreEqual(data.size(), datum.Size());
		Assert::AreEqual(10_z, datum.Capacity());
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
			Assert::ExpectException<std::runtime_error>([&datum] { datum.ShrinkToFit() });
		}

		Datum datum = data;

		datum.Resize(10);
		Assert::AreEqual(10_z, datum.Size());
		Assert::AreEqual(10_z, datum.Capacity());

		datum.Resize(5);
		datum.ShrinkToFit();
		Assert::AreEqual(5_z, datum.Size());
		Assert::AreEqual(5_z, datum.Capacity());

		datum.Resize(0);
		datum.ShrinkToFit();
		Assert::AreEqual(0_z, datum.Size());
		Assert::AreEqual(0_z, datum.Capacity());
	}

	template<typename T>
	void TestElementAccessors(const std::initializer_list<T> data)
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
		}
	}

	template<typename T>
	void TestPopBack()
	{

	}

	template<typename T>
	void TestRemove()
	{

	}

	template<typename T>
	void TestClear(const std::initializer_list<T> data)
	{
		Datum datum = data;
		
		datum.Clear();
		Assert::AreEqual(0_z, datum.Size());
		Assert::AreEqual(data.size(), datum.Capacity());
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

 		TEST_METHOD(Constructors)
 		{
			TestConstructors<int>({ 10, 20, 30 });
			TestConstructors<float>({ 10, 20, 30 });
 		}

 		TEST_METHOD(Copy)
 		{
 			TestCopy<int>({ 10, 20, 30 });
 			TestCopy<float>({ 10, 20, 30 });
 		}

		TEST_METHOD(Move)
		{
			TestMove<int>({ 10, 20, 30 });
			TestMove<float>({ 10, 20, 30 });
		}

		TEST_METHOD(Equality)
		{
			TestEquality<int>({ 10, 20, 30 });
			TestEquality<float>({ 10, 20, 30 });
		}

		TEST_METHOD(TypeSizeCapacity)
		{
			TestTypeSizeCapacity<int>({ 10, 20, 30 });
			TestTypeSizeCapacity<float>({ 10, 20, 30 });
		}

		TEST_METHOD(Resize)
		{
			TestResize<int>({ 10, 20, 30 });
			TestResize<float>({ 10, 20, 30 });
		}

		TEST_METHOD(ElementAccessors)
		{
			TestElementAccessors<int>({ 10, 20, 30 });
			TestElementAccessors<float>({ 10, 20, 30 });
		}

		TEST_METHOD(PushBack)
		{
			TestPushBack<int>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
			TestPushBack<float>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
		}

		TEST_METHOD(Clear)
		{
			TestClear<int>({ 10, 20, 30 });
			TestClear<float>({ 10, 20, 30 });
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState DatumTest::sStartMemState;
}