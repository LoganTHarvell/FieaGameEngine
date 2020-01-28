#include "pch.h"

#include "ToStringSpecialization.h"
#include "Foo.h"
#include "Bar.h"
#include "HashMap.h"

#include <vector>

using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;
using namespace Library;

namespace UnitTests
{
	template<typename TKey, typename TData>
	void TestIteratorInitialization(typename HashMap<TKey, TData>::KeyEqualityFunctor keyEqualityFunctor = DefaultEquality<TKey>())
	{
		HashMap hashMap = HashMap<TKey, TData>(20, keyEqualityFunctor);
		HashMap<TKey, TData>::Iterator it1(hashMap.begin());
		HashMap<TKey, TData>::Iterator it2 = hashMap.begin();
		Assert::AreEqual(it1, it2);

		HashMap<TKey, TData>::ConstIterator itConst1 = HashMap<TKey, TData>::ConstIterator(hashMap.begin());
		HashMap<TKey, TData>::ConstIterator itConst2 = hashMap.begin();
		Assert::AreEqual(itConst1, itConst2);
	}

	template<typename TKey, typename TData>
	void TestIteratorDereference(typename HashMap<TKey, TData>::KeyEqualityFunctor keyEqualityFunctor = DefaultEquality<TKey>())
	{
		Assert::ExpectException<std::runtime_error>([] { *HashMap<TKey, TData>::Iterator(); });
		Assert::ExpectException<std::runtime_error>([] { *HashMap<TKey, TData>::ConstIterator(); });

		HashMap hashMap = HashMap<TKey, TData>(20, keyEqualityFunctor);
		HashMap<TKey, TData>::Iterator it = hashMap.begin();
		HashMap<TKey, TData>::ConstIterator itConst = hashMap.cbegin();

		Assert::ExpectException<std::out_of_range>([&it] { *it; });
		Assert::ExpectException<std::out_of_range>([&itConst] { *itConst; });

		it = hashMap.Insert({ TKey(10), TData(10) }).first;
		itConst = hashMap.cbegin();

		Assert::AreEqual(*it, *hashMap.begin());
		Assert::AreEqual(*itConst, *hashMap.cbegin());
	}

	template<typename TKey, typename TData>
	void TestIteratorEquality(typename HashMap<TKey, TData>::KeyEqualityFunctor keyEqualityFunctor = DefaultEquality<TKey>())
	{
		HashMap hashMap = HashMap<TKey, TData>(20, keyEqualityFunctor);
		auto insert1 = hashMap.Insert({ TKey(10), TData(10) });
		auto insert2 = hashMap.Insert({ TKey(20), TData(10) });

		HashMap<TKey, TData>::Iterator it(hashMap.begin());
		HashMap<TKey, TData>::ConstIterator itConstEmpty(hashMap.cbegin());

		Assert::AreEqual(it, hashMap.begin());
		Assert::AreEqual(itConstEmpty, hashMap.cbegin());

		HashMap<TKey, TData>::ConstIterator itConst1 = insert1.first;
		HashMap<TKey, TData>::ConstIterator itConst2 = insert2.first;

		Assert::IsTrue(insert1.first != insert2.first);
		Assert::IsTrue(itConst1 != itConst2);
	}

	template<typename TKey, typename TData>
	void TestIteratorArithmetic(typename HashMap<TKey, TData>::KeyEqualityFunctor keyEqualityFunctor = DefaultEquality<TKey>())
	{
		HashMap<TKey, TData>::Iterator it;
		HashMap<TKey, TData>::ConstIterator itConst;

		Assert::ExpectException<std::runtime_error>([&it] { auto tmp = it++; });
		Assert::ExpectException<std::runtime_error>([&itConst] { auto tmp = itConst++; });

		HashMap hashMap = HashMap<TKey, TData>(20, keyEqualityFunctor);
		auto tmp1 = hashMap.Insert({ TKey(10), TData(10) });
		auto tmp2 = hashMap.Insert({ TKey(20), TData(20) });

		it = hashMap.end();
		itConst = hashMap.cend();

		Assert::ExpectException<std::out_of_range>([&it] { auto tmp = it++; });
		Assert::ExpectException<std::out_of_range>([&itConst] { auto tmp = itConst++; });

		it = hashMap.begin();
		itConst = hashMap.cbegin();

		Assert::IsTrue(++it != hashMap.begin());
		Assert::IsTrue(++itConst != hashMap.begin());

		Assert::IsTrue(it++ != hashMap.end());
		Assert::IsTrue(itConst++ != hashMap.end());

		Assert::AreEqual(it, hashMap.end());
		Assert::AreEqual(itConst, hashMap.cend());
	}

	template<typename TKey, typename TData>
	void TestInitialization(typename HashMap<TKey, TData>::KeyEqualityFunctor keyEqualityFunctor = DefaultEquality<TKey>())
	{
		HashMap hashMap = HashMap<TKey, TData>(20, keyEqualityFunctor);
		Assert::AreEqual(hashMap.begin(), hashMap.end());
		Assert::AreEqual(hashMap.cbegin(), hashMap.cend());
		Assert::AreEqual(hashMap.Size(), 0_z);
		Assert::AreEqual(hashMap.BucketCount(), 20_z);
		Assert::IsTrue(hashMap.IsEmpty());
	}

	template<typename TKey, typename TData>
	void TestCopy(typename HashMap<TKey, TData>::KeyEqualityFunctor keyEqualityFunctor = DefaultEquality<TKey>())
	{
		HashMap hashMap = HashMap<TKey, TData>(20, keyEqualityFunctor);
		auto tmp = hashMap.Insert({ TKey(10), TData(10) }).first;
		HashMap copy(hashMap);
		Assert::AreEqual(copy.Size(), hashMap.Size());
		Assert::AreEqual(*copy.begin(), *hashMap.begin());

		copy = hashMap;
		Assert::AreEqual(copy.Size(), hashMap.Size());
		Assert::AreEqual(*copy.begin(), *hashMap.begin());
	}

	template<typename TKey, typename TData>
	void TestMove(typename HashMap<TKey, TData>::KeyEqualityFunctor keyEqualityFunctor = DefaultEquality<TKey>())
	{
		HashMap hashMap = HashMap<TKey, TData>(20, keyEqualityFunctor);
		auto tmp = hashMap.Insert({ TKey(10), TData(10) }).first;
		HashMap copy = hashMap;
		
		HashMap moved(std::move(hashMap));
		Assert::AreEqual(moved.Size(), copy.Size());
		Assert::AreEqual(moved.BucketCount(), copy.BucketCount());
		Assert::AreEqual(*moved.begin(), *copy.begin());
		Assert::AreEqual(hashMap.BucketCount(), 0_z);
		
		hashMap = copy;
		moved = std::move(hashMap);

		Assert::AreEqual(moved.Size(), copy.Size());
		Assert::AreEqual(moved.BucketCount(), copy.BucketCount());
		Assert::AreEqual(*moved.begin(), *copy.begin());
		Assert::AreEqual(hashMap.BucketCount(), 0_z);
	}

	template<typename TKey, typename TData>
	void TestSizeCapacity(typename HashMap<TKey, TData>::KeyEqualityFunctor keyEqualityFunctor = DefaultEquality<TKey>())
	{
		HashMap hashMap = HashMap<TKey, TData>(20, keyEqualityFunctor);
		
		Assert::AreEqual(hashMap.Size(), 0_z);
		Assert::AreEqual(hashMap.BucketCount(), 20_z);
		Assert::IsTrue(hashMap.IsEmpty());
		Assert::AreEqual(hashMap.LoadFactor(), 0.0f);
		
		auto tmp = hashMap.Insert({ TKey(10), TData(10) }).first;
		tmp = hashMap.Insert({ TKey(20), TData(10) }).first;

		Assert::AreEqual(hashMap.Size(), 2_z);
		Assert::AreEqual(hashMap.BucketCount(), 20_z);
		Assert::IsFalse(hashMap.IsEmpty());
		Assert::AreEqual(hashMap.LoadFactor(), static_cast<float>(hashMap.Size())/hashMap.BucketCount());
	}

	template<typename TKey, typename TData>
	void TestRehash(typename HashMap<TKey, TData>::KeyEqualityFunctor keyEqualityFunctor = DefaultEquality<TKey>())
	{
		HashMap hashMap = HashMap<TKey, TData>(20, keyEqualityFunctor);	
		auto tmp = hashMap.Insert({ TKey(10), TData(10) }).first;
		tmp = hashMap.Insert({ TKey(20), TData(20) }).first;
		tmp = hashMap.Insert({ TKey(30), TData(30) }).first;

		hashMap.Rehash(10);

		Assert::AreEqual(hashMap.Size(), 3_z);
		Assert::AreEqual(hashMap.BucketCount(), 10_z);

		Assert::AreEqual(hashMap[TKey(10)], TData(10));
		Assert::AreEqual(hashMap[TKey(20)], TData(20));
		Assert::AreEqual(hashMap[TKey(30)], TData(30));
	}

	template<typename TKey, typename TData>
	void TestBegin(typename HashMap<TKey, TData>::KeyEqualityFunctor keyEqualityFunctor = DefaultEquality<TKey>())
	{
		HashMap hashMap = HashMap<TKey, TData>(20, keyEqualityFunctor);
		auto tmp = hashMap.Insert({ TKey(10), TData(10) }).first;

		HashMap<TKey, TData>::Iterator it = hashMap.begin();
		HashMap<TKey, TData>::ConstIterator itConst = hashMap.cbegin();

		Assert::AreEqual(it->first, TKey(10));
		Assert::AreEqual(it->second, TData(10));
		Assert::AreEqual(itConst->first, TKey(10));
		Assert::AreEqual(itConst->second, TData(10));

		const HashMap constHashMap = hashMap;
		itConst = constHashMap.begin();
		Assert::AreEqual(itConst, constHashMap.cbegin());
	}

	template<typename TKey, typename TData>
	void TestEnd(typename HashMap<TKey, TData>::KeyEqualityFunctor keyEqualityFunctor = DefaultEquality<TKey>())
	{
		HashMap hashMap = HashMap<TKey, TData>(20, keyEqualityFunctor);
		auto tmp = hashMap.Insert({ TKey(10), TData(10) }).first;

		HashMap<TKey, TData>::Iterator it = hashMap.begin();
		HashMap<TKey, TData>::ConstIterator itConst = hashMap.cbegin();

		Assert::IsTrue(it++ != hashMap.end());
		Assert::AreEqual(it, hashMap.end());
		Assert::IsTrue(itConst++ != hashMap.cend());
		Assert::AreEqual(itConst, hashMap.cend());

		const HashMap constHashMap = hashMap;
		itConst = constHashMap.end();
		Assert::AreEqual(itConst, constHashMap.cend());
	}

	template<typename TKey, typename TData>
	void TestFind(typename HashMap<TKey, TData>::KeyEqualityFunctor keyEqualityFunctor = DefaultEquality<TKey>())
	{
		HashMap hashMap = HashMap<TKey, TData>(20, keyEqualityFunctor);
		auto tmp = hashMap.Insert({ TKey(10), TData(10) }).first;

		HashMap<TKey, TData>::Iterator it = hashMap.Find(TKey(10));
		Assert::AreEqual(it->first, TKey(10));
		Assert::AreEqual(it->second, TData(10));
		
		it = hashMap.Find(TKey(20));
		Assert::AreEqual(it, hashMap.end());

		const HashMap constHashMap = hashMap;
		HashMap<TKey, TData>::ConstIterator itConst = constHashMap.Find(TKey(10));
		Assert::AreEqual(itConst->first, TKey(10));
		Assert::AreEqual(itConst->second, TData(10));

		itConst = hashMap.Find(TKey(20));
		Assert::AreEqual(itConst, hashMap.cend());
	}

	template<typename TKey, typename TData>
	void TestElementAccessors(typename HashMap<TKey, TData>::KeyEqualityFunctor keyEqualityFunctor = DefaultEquality<TKey>())
	{
		HashMap hashMap = HashMap<TKey, TData>(20, keyEqualityFunctor);
		auto tmp1 = hashMap.Insert({ TKey(10), TData(10) }).first;
		auto tmp2 = hashMap.Insert({ TKey(20), TData(20) }).first;

		Assert::AreEqual(hashMap.At(TKey(10)), TData(10));
		Assert::ExpectException<std::out_of_range>([&hashMap] { hashMap.At(TKey(30)); });
		Assert::AreEqual(hashMap[TKey(30)], TData());
		hashMap[TKey(30)] = TData(30);
		Assert::AreEqual(hashMap[TKey(30)], TData(30));

		const HashMap constHashMap = hashMap;
		Assert::AreEqual(constHashMap[TKey(20)], TData(20));
		Assert::ExpectException<std::out_of_range>([&constHashMap] { constHashMap.At(TKey(40)); });
	}

	template<typename TKey, typename TData>
	void TestContainsKey(typename HashMap<TKey, TData>::KeyEqualityFunctor keyEqualityFunctor = DefaultEquality<TKey>())
	{
		HashMap hashMap = HashMap<TKey, TData>(20, keyEqualityFunctor);
		auto tmp = hashMap.Insert({ TKey(10), TData(10) }).first;

		Assert::IsTrue(hashMap.ContainsKey(TKey(10)));
		Assert::IsFalse(hashMap.ContainsKey(TKey(20)));
	}

	template<typename TKey, typename TData>
	void TestInsert(typename HashMap<TKey, TData>::KeyEqualityFunctor keyEqualityFunctor = DefaultEquality<TKey>())
	{
		HashMap hashMap = HashMap<TKey, TData>(20, keyEqualityFunctor);

		auto tmp = hashMap.Insert({ TKey(10), TData(10) });
		Assert::IsTrue(tmp.second);
		
		tmp = hashMap.Insert({ TKey(10), TData(10) });
		Assert::IsFalse(tmp.second);

		HashMap<TKey, TData>::Iterator it = hashMap.begin();
		HashMap<TKey, TData>::ConstIterator itConst = hashMap.cbegin();

		Assert::AreEqual(it->first, TKey(10));
		Assert::AreEqual(it->second, TData(10));
		Assert::AreEqual(itConst->first, TKey(10));
		Assert::AreEqual(itConst->second, TData(10));

		Assert::IsTrue(hashMap.begin() != hashMap.end());
		Assert::AreEqual(1_z, hashMap.Size());
		Assert::IsTrue(!hashMap.IsEmpty());
	}

	template<typename TKey, typename TData>
	void TestRemove(typename HashMap<TKey, TData>::KeyEqualityFunctor keyEqualityFunctor = DefaultEquality<TKey>())
	{
		HashMap hashMap = HashMap<TKey, TData>(5, keyEqualityFunctor);
		
		for (int i = 0; i < 10 * 21; i += 10)
		{
			auto tmp = hashMap.Insert({ TKey(i), TData(i) });
		}

		Assert::IsFalse(hashMap.Remove(HashMap<TKey,TData>::Iterator()));
		Assert::IsTrue(hashMap.Remove(TKey(10)));
		Assert::IsFalse(hashMap.ContainsKey(TKey(10)));

		auto tmp = hashMap.Insert({ TKey(10), TData(10) }).first;
		Assert::IsTrue(hashMap.Remove(tmp));
		Assert::IsFalse(hashMap.ContainsKey(TKey(10)));
	}

	template<typename TKey, typename TData>
	void TestClear(typename HashMap<TKey, TData>::KeyEqualityFunctor keyEqualityFunctor = DefaultEquality<TKey>())
	{
		HashMap hashMap = HashMap<TKey, TData>(20, keyEqualityFunctor);
		auto tmp = hashMap.Insert({ TKey(10), TData(10) }).first;
		tmp = hashMap.Insert({ TKey(20), TData(10) }).first;
		tmp = hashMap.Insert({ TKey(30), TData(10) }).first;

		hashMap.Clear();

		Assert::AreEqual(hashMap.Size(), 0_z);
		Assert::AreEqual(hashMap.BucketCount(), 20_z);
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(HashMapTest)
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

		TEST_METHOD(IteratorInitialization)
		{
			TestIteratorInitialization<int, Foo>();
			TestIteratorInitialization<double, Foo>();
			TestIteratorInitialization<Foo, Foo>();
			TestIteratorInitialization<Bar, Foo>([](const Bar& lhs, const Bar& rhs) { return lhs.Data() == rhs.Data(); });
			TestIteratorInitialization<Bar, Bar>([](const Bar& lhs, const Bar& rhs) { return lhs.Data() == rhs.Data(); });
		}

		TEST_METHOD(IteratorDereference)
		{
			TestIteratorDereference<int, Foo>();
			TestIteratorDereference<double, Foo>();
			TestIteratorDereference<Foo, Foo>();
		}

		TEST_METHOD(IteratorEquality)
		{
			TestIteratorEquality<int, Foo>();
			TestIteratorEquality<double, Foo>();
			TestIteratorEquality<Foo, Foo>();
			TestIteratorEquality<Bar, Foo>([](const Bar& lhs, const Bar& rhs) { return lhs.Data() == rhs.Data(); });
			TestIteratorEquality<Bar, Bar>([](const Bar& lhs, const Bar& rhs) { return lhs.Data() == rhs.Data(); });
		}

		TEST_METHOD(IteratorArithmetic)
		{
			TestIteratorArithmetic<int, Foo>();
			TestIteratorArithmetic<double, Foo>();
			TestIteratorArithmetic<Foo, Foo>();
			TestIteratorArithmetic<Bar, Foo>([](const Bar& lhs, const Bar& rhs) { return lhs.Data() == rhs.Data(); });
			TestIteratorArithmetic<Bar, Bar>([](const Bar& lhs, const Bar& rhs) { return lhs.Data() == rhs.Data(); });
		}

 		TEST_METHOD(Initialization)
 		{
 			TestInitialization<int, Foo>();
 			TestInitialization<double, Foo>();
 			TestInitialization<Foo, Foo>();
 			TestInitialization<Bar, Foo>([](const Bar& lhs, const Bar& rhs) { return lhs.Data() == rhs.Data(); });
 			TestInitialization<Bar, Bar>([](const Bar& lhs, const Bar& rhs) { return lhs.Data() == rhs.Data(); });
 		}

 		TEST_METHOD(Copy)
 		{
 			TestCopy<int, Foo>();
 			TestCopy<double, Foo>();
 			TestCopy<Foo, Foo>();
 		}

		TEST_METHOD(Move)
		{
			TestMove<int, Foo>();
			TestMove<double, Foo>();
			TestMove<Foo, Foo>();
		}

		TEST_METHOD(SizeCapacity)
		{
			TestSizeCapacity<int, Foo>();
			TestSizeCapacity<double, Foo>();
			TestSizeCapacity<Foo, Foo>();
		}

		TEST_METHOD(Rehash)
		{
			TestRehash<int, Foo>();
			TestRehash<double, Foo>();
			TestRehash<Foo, Foo>();
		}

 		TEST_METHOD(Begin)
 		{
 			TestBegin<int, Foo>();
 			TestBegin<double, Foo>();
 			TestBegin<Foo, Foo>();
 		}

 		TEST_METHOD(End)
 		{
 			TestEnd<int, Foo>();
 			TestEnd<double, Foo>();
 			TestEnd<Foo, Foo>();
 			TestEnd<Bar, Foo>([](const Bar& lhs, const Bar& rhs) { return lhs.Data() == rhs.Data(); });
 			TestEnd<Bar, Bar>([](const Bar& lhs, const Bar& rhs) { return lhs.Data() == rhs.Data(); });
 		}

 		TEST_METHOD(Find)
 		{
 			TestFind<int, Foo>();
 			TestFind<double, Foo>();
 			TestFind<Foo, Foo>();
 		}

		TEST_METHOD(ElementAccess)
		{
			TestElementAccessors<int, Foo>();
			TestElementAccessors<double, Foo>();
			TestElementAccessors<Foo, Foo>();
			TestElementAccessors<Bar, Foo>([](const Bar& lhs, const Bar& rhs) { return lhs.Data() == rhs.Data(); });
		}

		TEST_METHOD(ContainsKey)
		{
			TestContainsKey<int, Foo>();
			TestContainsKey<double, Foo>();
			TestContainsKey<Foo, Foo>();
			TestContainsKey<Bar, Foo>([](const Bar& lhs, const Bar& rhs) { return lhs.Data() == rhs.Data(); });
			TestContainsKey<Bar, Bar>([](const Bar& lhs, const Bar& rhs) { return lhs.Data() == rhs.Data(); });
		}

		TEST_METHOD(Insert)
		{
			TestInsert<int, Foo>();
			TestInsert<double, Foo>();
			TestInsert<Foo, Foo>();
		}

		TEST_METHOD(Remove)
		{
			TestRemove<int, Foo>();
			TestRemove<double, Foo>();
			TestRemove<Foo, Foo>();
			TestRemove<Bar, Foo>([](const Bar& lhs, const Bar& rhs) { return lhs.Data() == rhs.Data(); });
			TestRemove<Bar, Bar>([](const Bar& lhs, const Bar& rhs) { return lhs.Data() == rhs.Data(); });
		}

		TEST_METHOD(Clear)
		{
			TestClear<int, Foo>();
			TestClear<double, Foo>();
			TestClear<Foo, Foo>();
			TestClear<Bar, Foo>([](const Bar& lhs, const Bar& rhs) { return lhs.Data() == rhs.Data(); });
			TestClear<Bar, Bar>([](const Bar& lhs, const Bar& rhs) { return lhs.Data() == rhs.Data(); });
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState HashMapTest::sStartMemState;
}