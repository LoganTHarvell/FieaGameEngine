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

		const HashMap constHashMap = hashMap;
		HashMap<TKey, TData>::ConstIterator itConst = constHashMap.Find(TKey(10));
		Assert::AreEqual(itConst->first, TKey(10));
		Assert::AreEqual(itConst->second, TData(10));
	}

	template<typename TKey, typename TData>
	void TestInsert(typename HashMap<TKey, TData>::KeyEqualityFunctor keyEqualityFunctor = DefaultEquality<TKey>())
	{
		HashMap hashMap = HashMap<TKey, TData>(20, keyEqualityFunctor);
		auto tmp = hashMap.Insert({ TKey(10), TData(10) }).first;

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
		}

		TEST_METHOD(IteratorArithmetic)
		{
			TestIteratorArithmetic<int, Foo>();
			TestIteratorArithmetic<double, Foo>();
			TestIteratorArithmetic<Foo, Foo>();
		}

		TEST_METHOD(Initialization)
		{
			TestInitialization<int, Foo>();
			TestInitialization<double, Foo>();
			TestInitialization<Foo, Foo>();
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
		}

		TEST_METHOD(Find)
		{
			TestFind<int, Foo>();
			TestFind<double, Foo>();
			TestFind<Foo, Foo>();
		}

		TEST_METHOD(Insert)
		{
			TestInsert<int, Foo>();
			TestInsert<double, Foo>();
			TestInsert<Foo, Foo>();
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState HashMapTest::sStartMemState;
}