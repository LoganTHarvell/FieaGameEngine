#include "pch.h"

#include "ToStringSpecialization.h"
#include "Foo.h"
#include "Bar.h"
#include "SList.h"


using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;
using namespace Library;


namespace ContainerTests
{
	TEST_CLASS(SListTest)
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
			SList<int> intList;
			SList<double> doubleList;
			SList<Foo> fooList;

			SList<int>::Iterator intIterator1 = SList<int>::Iterator(intList.begin());
			SList<double>::Iterator doubleIterator1 = SList<double>::Iterator(doubleList.begin());
			SList<Foo>::Iterator fooIterator1 = SList<Foo>::Iterator(fooList.begin());

			SList<int>::Iterator intIterator2 = intList.begin();
			SList<double>::Iterator doubleIterator2 = doubleList.begin();
			SList<Foo>::Iterator fooIterator2 = fooList.begin();

			Assert::AreEqual(intIterator1, intIterator2);
			Assert::AreEqual(doubleIterator1, doubleIterator2);
			Assert::AreEqual(fooIterator1, fooIterator2);

			SList<int>::ConstIterator constIntIterator1 = SList<int>::ConstIterator(intList.begin());
			SList<double>::ConstIterator constDoubleIterator1 = SList<double>::ConstIterator(doubleList.begin());
			SList<Foo>::ConstIterator constFooIterator1 = SList<Foo>::ConstIterator(fooList.begin());

			SList<int>::ConstIterator constIntIterator2 = intList.begin();
			SList<double>::ConstIterator constDoubleIterator2 = doubleList.begin();
			SList<Foo>::ConstIterator constFooIterator2 = fooList.begin();

			Assert::AreEqual(constIntIterator1, constIntIterator2);
			Assert::AreEqual(constDoubleIterator1, constDoubleIterator2);
			Assert::AreEqual(constFooIterator1, constFooIterator2);
		}

		TEST_METHOD(IteratorDereference)
		{
			Assert::ExpectException<std::runtime_error>([] { *SList<int>::Iterator(); });
			Assert::ExpectException<std::runtime_error>([] { *SList<double>::Iterator(); });
			Assert::ExpectException<std::runtime_error>([] { *SList<Foo>::Iterator(); });

			Assert::ExpectException<std::runtime_error>([] { *SList<int>::ConstIterator(); });
			Assert::ExpectException<std::runtime_error>([] { *SList<double>::ConstIterator(); });
			Assert::ExpectException<std::runtime_error>([] { *SList<Foo>::ConstIterator(); });

			SList<int> intList;
			SList<double> doubleList;
			SList<Foo> fooList;

			Assert::ExpectException<std::out_of_range>([&intList] { *(intList.begin()); });
			Assert::ExpectException<std::out_of_range>([&doubleList] { *(doubleList.begin()); });
			Assert::ExpectException<std::out_of_range>([&fooList] { *(fooList.begin()); });
			Assert::ExpectException<std::out_of_range>([&fooList] { fooList.begin()->Data(); });

			Assert::ExpectException<std::out_of_range>([&intList] { *(intList.cbegin()); });
			Assert::ExpectException<std::out_of_range>([&doubleList] { *(doubleList.cbegin()); });
			Assert::ExpectException<std::out_of_range>([&fooList] { *(fooList.cbegin()); });
			Assert::ExpectException<std::out_of_range>([&fooList] { fooList.cbegin()->Data(); });

			intList.PushBack(10);
			doubleList.PushBack(10);
			fooList.PushBack(Foo(10));

			Assert::AreEqual(*intList.begin(), 10);
			Assert::AreEqual(*doubleList.begin(), 10.0);
			Assert::AreEqual(*fooList.begin(), Foo(10));
			Assert::AreEqual(fooList.begin()->Data(), 10);

			Assert::AreEqual(*intList.cbegin(), 10);
			Assert::AreEqual(*doubleList.cbegin(), 10.0);
			Assert::AreEqual(*fooList.cbegin(), Foo(10));
			Assert::AreEqual(fooList.cbegin()->Data(), 10);
		}

		TEST_METHOD(IteratorEquality)
		{
			SList<int> intList = { 10, 10 };
			SList<double> doubleList = { 10, 10 };
			SList<Foo> fooList = { Foo(10), Foo(10) };

			SList<int>::Iterator intIterator = intList.begin();
			SList<double>::Iterator doubleIterator = doubleList.begin();
			SList<Foo>::Iterator fooIterator = fooList.begin();

			SList<int>::ConstIterator constIntIterator = intList.cbegin();
			SList<double>::ConstIterator constDoubleIterator = doubleList.cbegin();
			SList<Foo>::ConstIterator constFooIterator = fooList.cbegin();

			Assert::AreEqual(intIterator, intList.begin());
			Assert::AreEqual(doubleIterator, doubleList.begin());
			Assert::AreEqual(fooIterator, fooList.begin());

			Assert::AreEqual(constIntIterator, intList.cbegin());
			Assert::AreEqual(constDoubleIterator, doubleList.cbegin());
			Assert::AreEqual(constFooIterator, fooList.cbegin());

			SList<int>::Iterator prevIntIterator = intIterator++;
			SList<double>::Iterator prevDoubleIterator = doubleIterator++;
			SList<Foo>::Iterator prevFooIterator = fooIterator++;

			SList<int>::ConstIterator prevConstIntIterator = constIntIterator++;
			SList<double>::ConstIterator prevConstDoubleIterator = constDoubleIterator++;
			SList<Foo>::ConstIterator prevConstFooIterator = constFooIterator++;

			Assert::IsTrue(intIterator != prevIntIterator);
			Assert::IsTrue(doubleIterator != prevDoubleIterator);
			Assert::IsTrue(fooIterator != prevFooIterator);

			Assert::IsTrue(constIntIterator != prevConstIntIterator);
			Assert::IsTrue(constDoubleIterator != prevConstDoubleIterator);
			Assert::IsTrue(constFooIterator != prevConstFooIterator);
		}

		TEST_METHOD(IteratorIncrementOperators)
		{
			Assert::ExpectException<std::runtime_error>([] { ++SList<int>::Iterator(); });
			Assert::ExpectException<std::runtime_error>([] { ++SList<double>::Iterator(); });
			Assert::ExpectException<std::runtime_error>([] { ++SList<Foo>::Iterator(); });

			Assert::ExpectException<std::runtime_error>([] { ++SList<int>::ConstIterator(); });
			Assert::ExpectException<std::runtime_error>([] { ++SList<double>::ConstIterator(); });
			Assert::ExpectException<std::runtime_error>([] { ++SList<Foo>::ConstIterator(); });

			SList<int> intList = { 10, 20, 30 };
			SList<double> doubleList = { 10, 20, 30 };
			SList<Foo> fooList = { Foo(10), Foo(20), Foo(30) };

			SList<int>::Iterator intIterator = intList.begin();
			SList<double>::Iterator doubleIterator = doubleList.begin();
			SList<Foo>::Iterator fooIterator = fooList.begin();

			SList<int>::ConstIterator constIntIterator = intList.cbegin();
			SList<double>::ConstIterator constDoubleIterator = doubleList.cbegin();
			SList<Foo>::ConstIterator constFooIterator = fooList.cbegin();

			Assert::AreEqual(*intIterator, 10);
			Assert::AreEqual(*doubleIterator, 10.0);
			Assert::AreEqual(*fooIterator, Foo(10));

			Assert::AreEqual(*constIntIterator, 10);
			Assert::AreEqual(*constDoubleIterator, 10.0);
			Assert::AreEqual(*constFooIterator, Foo(10));

			SList<int>::Iterator prevIntIterator = intIterator++;
			SList<double>::Iterator prevDoubleIterator = doubleIterator++;
			SList<Foo>::Iterator prevFooIterator = fooIterator++;

			Assert::AreEqual(*prevIntIterator, 10);
			Assert::AreEqual(*prevDoubleIterator, 10.0);
			Assert::AreEqual(*prevFooIterator, Foo(10));

			Assert::AreEqual(*intIterator, 20);
			Assert::AreEqual(*doubleIterator, 20.0);
			Assert::AreEqual(*fooIterator, Foo(20));

			SList<int>::ConstIterator prevConstIntIterator = constIntIterator++;
			SList<double>::ConstIterator prevConstDoubleIterator = constDoubleIterator++;
			SList<Foo>::ConstIterator prevConstFooIterator = constFooIterator++;

			Assert::AreEqual(*prevConstIntIterator, 10);
			Assert::AreEqual(*prevConstDoubleIterator, 10.0);
			Assert::AreEqual(*prevConstFooIterator, Foo(10));

			Assert::AreEqual(*constIntIterator, 20);
			Assert::AreEqual(*constDoubleIterator, 20.0);
			Assert::AreEqual(*constFooIterator, Foo(20));

			++intIterator;
			++doubleIterator;
			++fooIterator;

			++constIntIterator;
			++constDoubleIterator;
			++constFooIterator;

			Assert::AreEqual(*intIterator, 30);
			Assert::AreEqual(*doubleIterator, 30.0);
			Assert::AreEqual(*fooIterator, Foo(30));

			Assert::AreEqual(*constIntIterator, 30);
			Assert::AreEqual(*constDoubleIterator, 30.0);
			Assert::AreEqual(*constFooIterator, Foo(30));

			++intIterator;
			++doubleIterator;
			++fooIterator;

			++constIntIterator;
			++constDoubleIterator;
			++constFooIterator;

			Assert::ExpectException<std::out_of_range>([&intIterator] { ++intIterator; });
			Assert::ExpectException<std::out_of_range>([&doubleIterator] { ++doubleIterator; });
			Assert::ExpectException<std::out_of_range>([&fooIterator] { ++fooIterator; });
										 
			Assert::ExpectException<std::out_of_range>([&constIntIterator] { ++constIntIterator; });
			Assert::ExpectException<std::out_of_range>([&constDoubleIterator] { ++constDoubleIterator; });
			Assert::ExpectException<std::out_of_range>([&constFooIterator] { ++constFooIterator; });
		}

		TEST_METHOD(SListInitialization)
		{
			SList<int> intListEmpty;
			SList<double> doubleListEmpty;
			SList<Foo> fooListEmpty;

			Assert::AreEqual(intListEmpty.begin(), intListEmpty.end());
			Assert::AreEqual(doubleListEmpty.begin(), doubleListEmpty.end());
			Assert::AreEqual(fooListEmpty.begin(), fooListEmpty.end());

			Assert::AreEqual(intListEmpty.cbegin(), intListEmpty.cend());
			Assert::AreEqual(doubleListEmpty.cbegin(), doubleListEmpty.cend());
			Assert::AreEqual(fooListEmpty.cbegin(), fooListEmpty.cend());

			Assert::AreEqual(intListEmpty.Size(), 0_z);
			Assert::AreEqual(doubleListEmpty.Size(), 0_z);
			Assert::AreEqual(fooListEmpty.Size(), 0_z);

			SList<int> intListFilled = { 10, 20, 30 };
			SList<double> doubleListFilled = { 10, 20, 30 };
			SList<Foo> fooListFilled = { Foo(10), Foo(20), Foo(30) };

			Assert::AreEqual(intListFilled.Size(), 3_z);
			Assert::AreEqual(doubleListFilled.Size(), 3_z);
			Assert::AreEqual(fooListFilled.Size(), 3_z);

			intListFilled = { 0 };
			doubleListFilled = { 0 };
			fooListFilled = { Foo(0) };

			Assert::AreEqual(intListFilled.Front(), 0);
			Assert::AreEqual(doubleListFilled.Front(), 0.0);
			Assert::AreEqual(fooListFilled.Front(), Foo(0));

			Assert::AreEqual(intListFilled.Size(), 1_z);
			Assert::AreEqual(doubleListFilled.Size(), 1_z);
			Assert::AreEqual(fooListFilled.Size(), 1_z);
		}

		TEST_METHOD(Copy)
		{
			SList<int> intList = { 10, 20, 30 };
			SList<double> doubleList = { 10, 20, 30 };
			SList<Foo> fooList = { Foo(10), Foo(20), Foo(30) };

			SList<int> intListCopy(intList);
			Assert::AreEqual(intListCopy, intList);
			intListCopy = intList;
			Assert::AreEqual(intListCopy, intList);

			SList<double> doubleListCopy(doubleList);
			Assert::AreEqual(doubleListCopy, doubleList);
			doubleListCopy = doubleList;
			Assert::AreEqual(doubleListCopy, doubleList);

			SList<Foo> fooListCopy(fooList);
			Assert::AreEqual(fooListCopy, fooList);
			fooListCopy = fooList;
			Assert::AreEqual(fooListCopy, fooList);
		}

		TEST_METHOD(MoveOperators)
		{
			SList<int> intList = { 10, 20, 30 };
			SList<double> doubleList = { 10, 20, 30 };
			SList<Foo> fooList = { Foo(10), Foo(20), Foo(30) };
			

			/* Integer Data */

			SList<int> intListCopy = intList;

			// Validates moved element is equal to copy
			SList<int> intListMoved = std::move(intList);
			Assert::AreEqual(intListMoved, intListCopy);
			Assert::IsTrue(intList.IsEmpty());

			intList = intListCopy;
			intListMoved = std::move(intList);
			Assert::AreEqual(intListMoved, intListCopy);
			Assert::IsTrue(intList.IsEmpty());

			/* Double Data */

			SList<double> doubleListCopy = doubleList;

			// Validates moved element is equal to copy
			SList<double> doubleListMoved = std::move(doubleList);
			Assert::AreEqual(doubleListMoved, doubleListCopy);
			Assert::IsTrue(doubleList.IsEmpty());

			doubleList.PushBack(10);
			doubleList.PushBack(20);

			doubleList = doubleListCopy;
			doubleListMoved = std::move(doubleList);
			Assert::AreEqual(doubleListMoved, doubleListCopy);
			Assert::IsTrue(doubleList.IsEmpty());

			/* Foo Data */

			SList<Foo> fooListCopy = fooList;

			// Validates moved element is equal to copy
			SList<Foo> fooListMoved = std::move(fooList);
			Assert::AreEqual(fooListMoved, fooListCopy);
			Assert::IsTrue(fooList.IsEmpty());

			fooList = fooListCopy;
			fooListMoved = std::move(fooList);
			Assert::AreEqual(fooListMoved, fooListCopy);
			Assert::IsTrue(fooList.IsEmpty());
		}

		TEST_METHOD(EqualityOperators)
		{
			/* Integer Data */

			SList<int> intList1;
			SList<int> intList2;

			// Equality with self
			Assert::AreEqual(intList1, intList1);

			// Equality when empty
			Assert::AreEqual(intList1, intList2);
			const SList<int> emptyConstIntList;
			Assert::AreEqual(emptyConstIntList, intList2);

			// Inequality when filled and different sizes
			intList1.PushBack(10);
			intList1.PushBack(20);
			intList2.PushBack(10);
			Assert::IsTrue(intList1 != intList2);
			const SList<int> filledConstIntList1 = intList1;
			Assert::IsTrue(filledConstIntList1 != intList2);

			// Equality when filled
			intList2.PushBack(20);
			Assert::AreEqual(intList1, intList2);
			const SList<int> filledConstIntList2 = intList1;
			Assert::AreEqual(filledConstIntList2, intList2);

			// Inequality when filled and equal size
			intList2.PopBack();
			intList2.PushBack(30);
			Assert::IsTrue(intList1 != intList2);
			Assert::IsTrue(filledConstIntList2 != intList2);


			/* Double Data */

			SList<double> doubleList1;
			SList<double> doubleList2;

			// Equality with self
			Assert::AreEqual(doubleList1, doubleList1);

			// Equality when empty
			Assert::AreEqual(doubleList1, doubleList2);
			const SList<double> emptyConstDoubleList = doubleList1;
			Assert::AreEqual(emptyConstDoubleList, doubleList2);

			// Inequality when filled and different sizes
			doubleList1.PushBack(10);
			doubleList1.PushBack(20);
			doubleList2.PushBack(10);
			Assert::IsTrue(doubleList1 != doubleList2);
			const SList<double> filledConstDoubleList1 = doubleList1;
			Assert::IsTrue(filledConstDoubleList1 != doubleList2);

			// Equality when filled
			doubleList2.PushBack(20);
			Assert::AreEqual(doubleList1, doubleList2);
			const SList<double> filledConstDoubleList2 = doubleList1;
			Assert::AreEqual(filledConstDoubleList2, doubleList2);

			// Inequality when filled and equal size
			doubleList2.PopBack();
			doubleList2.PushBack(30);
			Assert::IsTrue(doubleList1 != doubleList2);
			Assert::IsTrue(filledConstDoubleList2 != doubleList2);


			/* Foo Data */

			SList<Foo> fooList1;
			SList<Foo> fooList2;
			
			// Equality with self
			Assert::AreEqual(fooList1, fooList1);

			// Equality when empty
			Assert::AreEqual(fooList1, fooList2);
			const SList<Foo> emptyConstFooList = fooList1;
			Assert::AreEqual(emptyConstFooList, fooList2);

			// Inequality when filled and different sizes
			fooList1.PushBack(Foo(10));
			fooList1.PushBack(Foo(20));
			fooList2.PushBack(Foo(10));

			Assert::IsTrue(fooList1 != fooList2);
			const SList<Foo> filledConstFooList1 = fooList1;
			Assert::IsTrue(filledConstFooList1 != fooList2);

			// Equality when filled
			fooList2.PushBack(Foo(20));
			Assert::AreEqual(fooList1, fooList2);
			const SList<Foo> filledConstFooList2 = fooList1;
			Assert::AreEqual(filledConstFooList2, fooList2);

			// Inequality when filled and equal size
			fooList2.PopBack();
			fooList2.PushBack(Foo(30));
			Assert::IsTrue(fooList1 != fooList2);
			Assert::IsTrue(filledConstFooList2 != fooList2);
		}

		TEST_METHOD(Size)
		{
			/* Integer Data */

			// Size is zero
			SList<int> intList;
			Assert::AreEqual(intList.Size(), 0_z);

			const SList<int> emptyConstIntList = intList;
			Assert::AreEqual(emptyConstIntList.Size(), 0_z);

			// Size increased
			intList.PushBack(10);
			Assert::AreEqual(intList.Size(), 1_z);

			const SList<int> filledConstIntList = intList;
			Assert::AreEqual(filledConstIntList.Size(), 1_z);

			// Size decreased
			intList.PopBack();
			Assert::AreEqual(intList.Size(), 0_z);
			const SList<int> decreasedConstIntList = intList;
			Assert::AreEqual(decreasedConstIntList.Size(), 0_z);


			/* Double Data */

			// Size is zero
			SList<double> doubleList;
			Assert::AreEqual(doubleList.Size(), 0_z);

			const SList<double> emptyConstDoubleList = doubleList;
			Assert::AreEqual(emptyConstDoubleList.Size(), 0_z);

			// Size increased
			doubleList.PushBack(10);
			Assert::AreEqual(doubleList.Size(), 1_z);

			const SList<double> filledConstDoubleList = doubleList;
			Assert::AreEqual(filledConstDoubleList.Size(), 1_z);

			// Size decreased
			doubleList.PopBack();
			Assert::AreEqual(doubleList.Size(), 0_z);
			const SList<double> decreasedConstDoubleList = doubleList;
			Assert::AreEqual(decreasedConstDoubleList.Size(), 0_z);


			/* Foo Data */

			// Size is zero
			SList<Foo> fooList;
			Assert::AreEqual(fooList.Size(), 0_z);
			const SList<Foo> emptyConstFooList = fooList;
			Assert::AreEqual(emptyConstFooList.Size(), 0_z);

			// Size increased
			fooList.PushBack(Foo(10));
			Assert::AreEqual(fooList.Size(), 1_z);
			const SList<Foo> filledConstFooList = fooList;
			Assert::AreEqual(filledConstFooList.Size(), 1_z);

			// Size decreased
			fooList.PopBack();
			Assert::AreEqual(fooList.Size(), 0_z);
			const SList<Foo> decreasedConstFooList = fooList;
			Assert::AreEqual(decreasedConstFooList.Size(), 0_z);
		}

		TEST_METHOD(IsEmpty)
		{
			/* Integer Data */

			// Empty list and size
			SList<int> intList;
			Assert::IsTrue(intList.IsEmpty());

			const SList<int> emptyConstIntList = intList;
			Assert::IsTrue(emptyConstIntList.IsEmpty());

			// Non-empty list and size
			intList.PushBack(10);
			Assert::IsTrue(!intList.IsEmpty());

			const SList<int> filledConstIntList = intList;
			Assert::IsTrue(!filledConstIntList.IsEmpty());

			/* Double Data */

			// Empty list and size
			SList<double> doubleList;
			Assert::IsTrue(doubleList.IsEmpty());

			const SList<double> emptyConstDoubleList = doubleList;
			Assert::IsTrue(emptyConstDoubleList.IsEmpty());

			// Non-empty list and size
			doubleList.PushBack(10);
			Assert::IsTrue(!doubleList.IsEmpty());

			const SList<double> filledConstDoubleList = doubleList;
			Assert::IsTrue(!filledConstDoubleList.IsEmpty());

			/* Foo Data */

			// Empty list and size
			SList<Foo> fooList;
			Assert::IsTrue(fooList.IsEmpty());

			const SList<Foo> emptyConstFooList = fooList;
			Assert::IsTrue(emptyConstFooList.IsEmpty());

			// Non-empty list and size
			fooList.PushBack(Foo(10));
			Assert::IsTrue(!fooList.IsEmpty());

			const SList<Foo> filledConstFooList = fooList;
			Assert::IsTrue(!filledConstFooList.IsEmpty());
		}

		TEST_METHOD(Begin)
		{
			SList<int> intList;
			SList<double> doubleList;
			SList<Foo> fooList;

			Assert::ExpectException<std::out_of_range>([&intList] { *(intList.begin()); });
			Assert::ExpectException<std::out_of_range>([&doubleList] { *(doubleList.begin()); });
			Assert::ExpectException<std::out_of_range>([&fooList] { *(fooList.begin()); });

			const SList<int> emptyIntListCopy = intList;
			const SList<double> emptyDoubleListCopy = doubleList;
			const SList<Foo> emptyFooListCopy = fooList;

			Assert::ExpectException<std::out_of_range>([&emptyIntListCopy] { *(emptyIntListCopy.begin()); });
			Assert::ExpectException<std::out_of_range>([&emptyDoubleListCopy] { *(emptyDoubleListCopy.begin()); });
			Assert::ExpectException<std::out_of_range>([&emptyFooListCopy] { *(emptyFooListCopy.begin()); });

			Assert::ExpectException<std::out_of_range>([&intList] { *(intList.cbegin()); });
			Assert::ExpectException<std::out_of_range>([&doubleList] { *(doubleList.cbegin()); });
			Assert::ExpectException<std::out_of_range>([&fooList] { *(fooList.cbegin()); });

			intList.PushBack(10);
			doubleList.PushBack(10);
			fooList.PushBack(Foo(10));

			Assert::AreEqual(*intList.begin(), 10);
			Assert::AreEqual(*doubleList.begin(), 10.0);
			Assert::AreEqual(*fooList.begin(), Foo(10));

			const SList<int> filledIntListCopy = intList;
			const SList<double> filledDoubleListCopy = doubleList;
			const SList<Foo> filledFooListCopy = fooList;

			Assert::AreEqual(*filledIntListCopy.begin(), 10);
			Assert::AreEqual(*filledDoubleListCopy.begin(), 10.0);
			Assert::AreEqual(*filledFooListCopy.begin(), Foo(10));
			
			Assert::AreEqual(*intList.cbegin(), 10);
			Assert::AreEqual(*doubleList.cbegin(), 10.0);
			Assert::AreEqual(*fooList.cbegin(), Foo(10));
		}

		TEST_METHOD(End)
		{
			SList<int> intList;
			SList<double> doubleList;
			SList<Foo> fooList;

			Assert::ExpectException<std::out_of_range>([&intList] { *(intList.end()); });
			Assert::ExpectException<std::out_of_range>([&doubleList] { *(doubleList.end()); });
			Assert::ExpectException<std::out_of_range>([&fooList] { *(fooList.end()); });

			const SList<int> emptyIntListCopy = intList;
			const SList<double> emptyDoubleListCopy = doubleList;
			const SList<Foo> emptyFooListCopy = fooList;

			Assert::ExpectException<std::out_of_range>([&emptyIntListCopy] { *(emptyIntListCopy.end()); });
			Assert::ExpectException<std::out_of_range>([&emptyDoubleListCopy] { *(emptyDoubleListCopy.end()); });
			Assert::ExpectException<std::out_of_range>([&emptyFooListCopy] { *(emptyFooListCopy.end()); });

			Assert::ExpectException<std::out_of_range>([&intList] { *(intList.cend()); });
			Assert::ExpectException<std::out_of_range>([&doubleList] { *(doubleList.cend()); });
			Assert::ExpectException<std::out_of_range>([&fooList] { *(fooList.cend()); });

			intList.PushBack(10);
			doubleList.PushBack(10);
			fooList.PushBack(Foo(10));

			Assert::ExpectException<std::out_of_range>([&intList] { *(intList.end()); });
			Assert::ExpectException<std::out_of_range>([&intList] { *(intList.end()); });
			Assert::ExpectException<std::out_of_range>([&intList] { *(intList.end()); });

			const SList<int> filledIntListCopy = intList;
			const SList<double> filledDoubleListCopy = doubleList;
			const SList<Foo> filledFooListCopy = fooList;

			Assert::ExpectException<std::out_of_range>([&filledIntListCopy] { *(filledIntListCopy.end()); });
			Assert::ExpectException<std::out_of_range>([&filledDoubleListCopy] { *(filledDoubleListCopy.end()); });
			Assert::ExpectException<std::out_of_range>([&filledFooListCopy] { *(filledFooListCopy.end()); });

			Assert::ExpectException<std::out_of_range>([&intList] { *(intList.cend()); });
			Assert::ExpectException<std::out_of_range>([&doubleList] { *(doubleList.cend()); });
			Assert::ExpectException<std::out_of_range>([&fooList] { *(fooList.cend()); });

			SList<int>::Iterator intIterator = intList.begin();
			SList<double>::Iterator doubleIterator = doubleList.begin();
			SList<Foo>::Iterator fooIterator = fooList.begin();

			SList<int>::ConstIterator constIntIterator = intList.cbegin();
			SList<double>::ConstIterator constDoubleIterator = doubleList.cbegin();
			SList<Foo>::ConstIterator constFooIterator = fooList.cbegin();

			++intIterator;
			++doubleIterator;
			++fooIterator;

			++constIntIterator;
			++constDoubleIterator;
			++constFooIterator;

			Assert::AreEqual(intIterator, intList.end());
			Assert::AreEqual(doubleIterator, doubleList.end());
			Assert::AreEqual(fooIterator, fooList.end());

			Assert::AreEqual(constIntIterator, intList.cend());
			Assert::AreEqual(constDoubleIterator, doubleList.cend());
			Assert::AreEqual(constFooIterator, fooList.cend());
		}

		TEST_METHOD(PushFront)
		{
			SList<int> intList;
			SList<double> doubleList;
			SList<Foo> fooList;

			const SList<int> emptyConstIntList;
			const SList<double> emptyConstDoubleList;
			const SList<Foo> emptyConstFooList;

			// Empty list exception when calling Front
			Assert::ExpectException<std::runtime_error>([&intList] { intList.Front(); });
			Assert::ExpectException<std::runtime_error>([&doubleList] { doubleList.Front(); });
			Assert::ExpectException<std::runtime_error>([&fooList] { fooList.Front(); });
			Assert::ExpectException<std::runtime_error>([&emptyConstIntList] { emptyConstIntList.Front(); });
			Assert::ExpectException<std::runtime_error>([&emptyConstDoubleList] { emptyConstDoubleList.Front(); });
			Assert::ExpectException<std::runtime_error>([&emptyConstFooList] { emptyConstFooList.Front(); });

			intList.PushFront(10);
			doubleList.PushFront(10);
			fooList.PushFront(Foo(10));

			const SList<int> filledConstIntList1 = intList;
			const SList<double> filledConstDoubleList1 = doubleList;
			const SList<Foo> filledConstFooList1 = fooList;

			// Data was set correctly
			Assert::AreEqual(intList.Front(), 10);
			Assert::AreEqual(doubleList.Front(), 10.0);
			Assert::AreEqual(fooList.Front(), Foo(10));
			Assert::AreEqual(filledConstIntList1.Front(), 10);
			Assert::AreEqual(filledConstDoubleList1.Front(), 10.0);
			Assert::AreEqual(filledConstFooList1.Front(), Foo(10));

			// First data value sets front and back references
			Assert::AreEqual(intList.Front(), intList.Back());
			Assert::AreEqual(doubleList.Front(), doubleList.Back());
			Assert::AreEqual(fooList.Front(), fooList.Back());
			Assert::AreEqual(filledConstIntList1.Front(), intList.Back());
			Assert::AreEqual(filledConstDoubleList1.Front(), doubleList.Back());
			Assert::AreEqual(filledConstFooList1.Front(), fooList.Back());

			// Non-empty
			Assert::IsFalse(intList.IsEmpty());
			Assert::IsFalse(doubleList.IsEmpty());
			Assert::IsFalse(fooList.IsEmpty());
			Assert::IsFalse(filledConstIntList1.IsEmpty());
			Assert::IsFalse(filledConstDoubleList1.IsEmpty());
			Assert::IsFalse(filledConstFooList1.IsEmpty());

			// Size of 1
			Assert::AreEqual(intList.Size(), 1_z);
			Assert::AreEqual(doubleList.Size(), 1_z);
			Assert::AreEqual(fooList.Size(), 1_z);
			Assert::AreEqual(filledConstIntList1.Size(), 1_z);
			Assert::AreEqual(filledConstDoubleList1.Size(), 1_z);
			Assert::AreEqual(filledConstFooList1.Size(), 1_z);

			intList.PushFront(20);
			doubleList.PushFront(20);
			fooList.PushFront(Foo(20));

			const SList<int> filledConstIntList2 = intList;
			const SList<double> filledConstDoubleList2 = doubleList;
			const SList<Foo> filledConstFooList2 = fooList;

			// Data was set correctly
			Assert::AreEqual(intList.Front(), 20);
			Assert::AreEqual(doubleList.Front(), 20.0);
			Assert::AreEqual(fooList.Front(), Foo(20));
			Assert::AreEqual(filledConstIntList2.Front(), 20);
			Assert::AreEqual(filledConstDoubleList2.Front(), 20.0);
			Assert::AreEqual(filledConstFooList2.Front(), Foo(20));

			// Size of 2
			Assert::AreEqual(intList.Size(), 2_z);
			Assert::AreEqual(doubleList.Size(), 2_z);
			Assert::AreEqual(fooList.Size(), 2_z);
			Assert::AreEqual(filledConstIntList2.Size(), 2_z);
			Assert::AreEqual(filledConstDoubleList2.Size(), 2_z);
			Assert::AreEqual(filledConstFooList2.Size(), 2_z);
		}

		TEST_METHOD(PopFront)
		{
			SList<int> intList = { 10, 20, 30 };
			SList<double> doubleList = { 10, 20, 30 };
			SList<Foo> fooList = { Foo(10), Foo(20), Foo(30) };

			intList.PopFront();
			doubleList.PopFront();
			fooList.PopFront();

			// Front is set correctly with more than 2 elements
			Assert::AreEqual(intList.Front(), 20);
			Assert::AreEqual(doubleList.Front(), 20.0);
			Assert::AreEqual(fooList.Front(), Foo(20));

			// Size is reduced correctly with more than 2 elements
			Assert::AreEqual(intList.Size(), 2_z);
			Assert::AreEqual(doubleList.Size(), 2_z);
			Assert::AreEqual(fooList.Size(), 2_z);

			intList.PopFront();
			doubleList.PopFront();
			fooList.PopFront();

			// Front is set correctly with 1 element remaining
			Assert::AreEqual(intList.Front(), 30);
			Assert::AreEqual(doubleList.Front(), 30.0);
			Assert::AreEqual(fooList.Front(), Foo(30));

			// Size is reduced correctly with 1 element remaining
			Assert::AreEqual(intList.Size(), 1_z);
			Assert::AreEqual(doubleList.Size(), 1_z);
			Assert::AreEqual(fooList.Size(), 1_z);

			// Back is set correctly with 1 element remaining
			Assert::AreEqual(intList.Front(), intList.Back());
			Assert::AreEqual(doubleList.Front(), doubleList.Back());
			Assert::AreEqual(fooList.Front(), fooList.Back());

			intList.PopFront();
			doubleList.PopFront();
			fooList.PopFront();

			// Size is reduced correctly with no elements remaining
			Assert::AreEqual(intList.Size(), 0_z);
			Assert::AreEqual(doubleList.Size(), 0_z);
			Assert::AreEqual(fooList.Size(), 0_z);

			// Now empty
			Assert::IsTrue(intList.IsEmpty());
			Assert::IsTrue(doubleList.IsEmpty());
			Assert::IsTrue(fooList.IsEmpty());

			intList.PopFront();
			doubleList.PopFront();
			fooList.PopFront();

			// Size does not change when called on empty list
			Assert::AreEqual(intList.Size(), 0_z);
			Assert::AreEqual(doubleList.Size(), 0_z);
			Assert::AreEqual(fooList.Size(), 0_z);

			// Emptiness does not change when called on empty list
			Assert::IsTrue(intList.IsEmpty());
			Assert::IsTrue(doubleList.IsEmpty());
			Assert::IsTrue(fooList.IsEmpty());
		}

		TEST_METHOD(PushBack)
		{
			SList<int> intList;
			SList<double> doubleList;
			SList<Foo> fooList;

			const SList<int> emptyConstIntList;
			const SList<double> emptyConstDoubleList;
			const SList<Foo> emptyConstFooList;

			// Empty list exception when calling Back
			Assert::ExpectException<std::runtime_error>([&intList] { intList.Back(); });
			Assert::ExpectException<std::runtime_error>([&doubleList] { doubleList.Back(); });
			Assert::ExpectException<std::runtime_error>([&fooList] { fooList.Back(); });
			Assert::ExpectException<std::runtime_error>([&emptyConstIntList] { emptyConstIntList.Back(); });
			Assert::ExpectException<std::runtime_error>([&emptyConstDoubleList] { emptyConstDoubleList.Back(); });
			Assert::ExpectException<std::runtime_error>([&emptyConstFooList] { emptyConstFooList.Back(); });

			intList.PushBack(10);
			doubleList.PushBack(10);
			fooList.PushBack(Foo(10));

			const SList<int> filledConstIntList1 = intList;
			const SList<double> filledConstDoubleList1 = doubleList;
			const SList<Foo> filledConstFooList1 = fooList;

			// Data was set correctly
			Assert::AreEqual(intList.Back(), 10);
			Assert::AreEqual(doubleList.Back(), 10.0);
			Assert::AreEqual(fooList.Back(), Foo(10));
			Assert::AreEqual(filledConstIntList1.Back(), 10);
			Assert::AreEqual(filledConstDoubleList1.Back(), 10.0);
			Assert::AreEqual(filledConstFooList1.Back(), Foo(10));

			// First data value sets front and back references
			Assert::AreEqual(intList.Back(), intList.Front());
			Assert::AreEqual(doubleList.Back(), doubleList.Front());
			Assert::AreEqual(fooList.Back(), fooList.Front());
			Assert::AreEqual(filledConstIntList1.Back(), intList.Front());
			Assert::AreEqual(filledConstDoubleList1.Back(), doubleList.Front());
			Assert::AreEqual(filledConstFooList1.Back(), fooList.Front());

			// Non-empty
			Assert::IsFalse(intList.IsEmpty());
			Assert::IsFalse(doubleList.IsEmpty());
			Assert::IsFalse(fooList.IsEmpty());
			Assert::IsFalse(filledConstIntList1.IsEmpty());
			Assert::IsFalse(filledConstDoubleList1.IsEmpty());
			Assert::IsFalse(filledConstFooList1.IsEmpty());

			// Size of 1
			Assert::AreEqual(intList.Size(), 1_z);
			Assert::AreEqual(doubleList.Size(), 1_z);
			Assert::AreEqual(fooList.Size(), 1_z);
			Assert::AreEqual(filledConstIntList1.Size(), 1_z);
			Assert::AreEqual(filledConstDoubleList1.Size(), 1_z);
			Assert::AreEqual(filledConstFooList1.Size(), 1_z);

			intList.PushBack(20);
			doubleList.PushBack(20);
			fooList.PushBack(Foo(20));

			const SList<int> filledConstIntList2 = intList;
			const SList<double> filledConstDoubleList2 = doubleList;
			const SList<Foo> filledConstFooList2 = fooList;

			// Data was set correctly
			Assert::AreEqual(intList.Back(), 20);
			Assert::AreEqual(doubleList.Back(), 20.0);
			Assert::AreEqual(fooList.Back(), Foo(20));
			Assert::AreEqual(filledConstIntList2.Back(), 20);
			Assert::AreEqual(filledConstDoubleList2.Back(), 20.0);
			Assert::AreEqual(filledConstFooList2.Back(), Foo(20));

			// Size of 2
			Assert::AreEqual(intList.Size(), 2_z);
			Assert::AreEqual(doubleList.Size(), 2_z);
			Assert::AreEqual(fooList.Size(), 2_z);
			Assert::AreEqual(filledConstIntList2.Size(), 2_z);
			Assert::AreEqual(filledConstDoubleList2.Size(), 2_z);
			Assert::AreEqual(filledConstFooList2.Size(), 2_z);
		}

		TEST_METHOD(PopBack)
		{
			SList<int> intList = { 10, 20, 30 };
			SList<double> doubleList = { 10, 20, 30 };
			SList<Foo> fooList = { Foo(10), Foo(20), Foo(30) };

			intList.PopBack();
			doubleList.PopBack();
			fooList.PopBack();

			// Back is set correctly with more than 2 elements
			Assert::AreEqual(intList.Back(), 20);
			Assert::AreEqual(doubleList.Back(), 20.0);
			Assert::AreEqual(fooList.Back(), Foo(20));

			// Size is reduced correctly with more than 2 elements
			Assert::AreEqual(intList.Size(), 2_z);
			Assert::AreEqual(doubleList.Size(), 2_z);
			Assert::AreEqual(fooList.Size(), 2_z);

			intList.PopBack();
			doubleList.PopBack();
			fooList.PopBack();

			// Back is set correctly with 1 element remaining
			Assert::AreEqual(intList.Back(), 10);
			Assert::AreEqual(doubleList.Back(), 10.0);
			Assert::AreEqual(fooList.Back(), Foo(10));

			// Size is reduced correctly with 1 element remaining
			Assert::AreEqual(intList.Size(), 1_z);
			Assert::AreEqual(doubleList.Size(), 1_z);
			Assert::AreEqual(fooList.Size(), 1_z);

			// Back is set correctly with 1 element remaining
			Assert::AreEqual(intList.Back(), intList.Back());
			Assert::AreEqual(doubleList.Back(), doubleList.Back());
			Assert::AreEqual(fooList.Back(), fooList.Front());

			intList.PopBack();
			doubleList.PopBack();
			fooList.PopBack();

			// Size is reduced correctly with no elements remaining
			Assert::AreEqual(intList.Size(), 0_z);
			Assert::AreEqual(doubleList.Size(), 0_z);
			Assert::AreEqual(fooList.Size(), 0_z);

			// Now empty
			Assert::IsTrue(intList.IsEmpty());
			Assert::IsTrue(doubleList.IsEmpty());
			Assert::IsTrue(fooList.IsEmpty());

			intList.PopBack();
			doubleList.PopBack();
			fooList.PopBack();

			// Size does not change when called on empty list
			Assert::AreEqual(intList.Size(), 0_z);
			Assert::AreEqual(doubleList.Size(), 0_z);
			Assert::AreEqual(fooList.Size(), 0_z);

			// Emptiness does not change when called on empty list
			Assert::IsTrue(intList.IsEmpty());
			Assert::IsTrue(doubleList.IsEmpty());
			Assert::IsTrue(fooList.IsEmpty());
		}

		TEST_METHOD(Find)
		{
			SList<int> missingEquality = SList(SList<int>::EqualityFunctor());
			Assert::ExpectException<std::runtime_error>([&missingEquality] { missingEquality.Find(1); });
			
			const SList<int> constMissingEquality = SList(SList<int>::EqualityFunctor());
			Assert::ExpectException<std::runtime_error>([&constMissingEquality] { constMissingEquality.Find(1); });
			
			SList<int> intList = { 10, 20, 30 };
			SList<double> doubleList = { 10, 20, 30 };
			SList<Foo> fooList = { Foo(10), Foo(20), Foo(30) };
			SList<Bar> barList = SList<Bar>({ Bar(10), Bar(20), Bar(30) }, [](Bar a, Bar b) { return a.Data() == b.Data(); });

			const SList<int> constIntList = intList;
			const SList<double> constDoubleList = doubleList;
			const SList<Foo> constFooList = fooList;
			const SList<Bar> constBarList = barList;

			SList<int>::Iterator intIterator = intList.begin();
			SList<double>::Iterator doubleIterator = doubleList.begin();
			SList<Foo>::Iterator fooIterator = fooList.begin();
			SList<Bar>::Iterator barIterator = barList.begin();

			Assert::AreEqual(intIterator++, intList.Find(10));
			Assert::AreEqual(doubleIterator++, doubleList.Find(10));
			Assert::AreEqual(fooIterator++, fooList.Find(Foo(10)));
			Assert::AreEqual(barIterator++, barList.Find(Bar(10)));

			Assert::AreEqual(intIterator++, intList.Find(20));
			Assert::AreEqual(doubleIterator++, doubleList.Find(20));
			Assert::AreEqual(fooIterator++, fooList.Find(Foo(20)));
			Assert::AreEqual(barIterator++, barList.Find(Bar(20)));

			Assert::AreEqual(intIterator, intList.Find(30));
			Assert::AreEqual(doubleIterator, doubleList.Find(30));
			Assert::AreEqual(fooIterator, fooList.Find(Foo(30)));
			Assert::AreEqual(barIterator, barList.Find(Bar(30)));

			Assert::AreEqual(intList.end(), intList.Find(40));
			Assert::AreEqual(doubleList.end(), doubleList.Find(40));
			Assert::AreEqual(fooList.end(), fooList.Find(Foo(40)));
			Assert::AreEqual(barList.end(), barList.Find(Bar(40)));
			
			SList<int>::ConstIterator constIntIterator = constIntList.begin();
			SList<double>::ConstIterator constDoubleIterator = constDoubleList.begin();
			SList<Foo>::ConstIterator constFooIterator = constFooList.begin();
			SList<Bar>::ConstIterator constBarIterator = constBarList.begin();

			Assert::AreEqual(constIntIterator++, constIntList.Find(10));
			Assert::AreEqual(constDoubleIterator++, constDoubleList.Find(10));
			Assert::AreEqual(constFooIterator++, constFooList.Find(Foo(10)));
			Assert::AreEqual(constBarIterator++, constBarList.Find(Bar(10)));

			Assert::AreEqual(constIntIterator++, constIntList.Find(20));
			Assert::AreEqual(constDoubleIterator++, constDoubleList.Find(20));
			Assert::AreEqual(constFooIterator++, constFooList.Find(Foo(20)));
			Assert::AreEqual(constBarIterator++, constBarList.Find(Bar(20)));

			Assert::AreEqual(constIntIterator, constIntList.Find(30));
			Assert::AreEqual(constDoubleIterator, constDoubleList.Find(30));
			Assert::AreEqual(constFooIterator, constFooList.Find(Foo(30)));
			Assert::AreEqual(constBarIterator, constBarList.Find(Bar(30)));

			Assert::AreEqual(constIntList.cend(), constIntList.Find(40));
			Assert::AreEqual(constDoubleList.cend(), constDoubleList.Find(40));
			Assert::AreEqual(constFooList.cend(), constFooList.Find(Foo(40)));
			Assert::AreEqual(constBarList.cend(), constBarList.Find(Bar(40)));
		}

		TEST_METHOD(InsertAfter)
		{
			SList<int> intList;
			SList<double> doubleList;
			SList<Foo> fooList;

			Assert::ExpectException<std::runtime_error>([&intList] { auto it = intList.InsertAfter(SList<int>::Iterator(), 10); });
			Assert::ExpectException<std::runtime_error>([&doubleList] { auto it = doubleList.InsertAfter(SList<double>::Iterator(), 10); });
			Assert::ExpectException<std::runtime_error>([&fooList] { auto it = fooList.InsertAfter(SList<Foo>::Iterator(), Foo(10)); });

			SList<int>::Iterator intIterator = intList.InsertAfter(intList.begin(), 10);
			SList<double>::Iterator doubleIterator = doubleList.InsertAfter(doubleList.begin(), 10);
			SList<Foo>::Iterator fooIterator = fooList.InsertAfter(fooList.begin(), Foo(10));

			Assert::AreEqual(*intIterator, 10);
			Assert::AreEqual(*doubleIterator, 10.0);
			Assert::AreEqual(*fooIterator, Foo(10));

			intIterator = intList.InsertAfter(intIterator, 20);
			doubleIterator = doubleList.InsertAfter(doubleIterator, 20);
			fooIterator = fooList.InsertAfter(fooIterator, Foo(20));

			intIterator = intList.begin();
			doubleIterator = doubleList.begin();
			fooIterator = fooList.begin();

			intIterator = intList.InsertAfter(intIterator, 30);
			doubleIterator = doubleList.InsertAfter(doubleIterator, 30);
			fooIterator = fooList.InsertAfter(fooIterator, Foo(30));

			Assert::AreEqual(*intIterator, 30);
			Assert::AreEqual(*doubleIterator, 30.0);
			Assert::AreEqual(*fooIterator, Foo(30));

			++intIterator;
			++doubleIterator;
			++fooIterator;

			Assert::AreEqual(*intIterator, 20);
			Assert::AreEqual(*doubleIterator, 20.0);
			Assert::AreEqual(*fooIterator, Foo(20));

			++intIterator;
			++doubleIterator;
			++fooIterator;

			Assert::AreEqual(intIterator, intList.end());
			Assert::AreEqual(doubleIterator, doubleList.end());
			Assert::AreEqual(fooIterator, fooList.end());

			intIterator = intList.begin();
			doubleIterator = doubleList.begin();
			fooIterator = fooList.begin();

			Assert::AreEqual(*intIterator, 10);
			Assert::AreEqual(*doubleIterator, 10.0);
			Assert::AreEqual(*fooIterator, Foo(10));
		}

		TEST_METHOD(Remove)
		{
			/* Integer Data */

			SList<int> intList;

			Assert::ExpectException<std::runtime_error>([&intList] { intList.Remove(SList<int>::Iterator()); });

			intList.PushBack(10);
			intList.PushBack(20);
			intList.Remove(intList.begin());
			
			SList<int>::Iterator intIterator = intList.begin();
			Assert::AreEqual(*intIterator, 20);

			intList.PushBack(30);
			intList.PushBack(40);
			intList.PushBack(50);
			intList.Remove(40);
			intList.Remove(50);

			intIterator = intList.begin();
			Assert::AreEqual(*intIterator, 20);

			intList.Remove(intList.begin());
			intList.Remove(intList.begin());

			Assert::IsTrue(intList.IsEmpty());


			/* Double Data */

			SList<double> doubleList;

			Assert::ExpectException<std::runtime_error>([&doubleList] { doubleList.Remove(SList<double>::Iterator()); });

			doubleList.PushBack(10);
			doubleList.PushBack(20);
			doubleList.Remove(doubleList.begin());

			SList<double>::Iterator doubleIterator = doubleList.begin();
			Assert::AreEqual(*doubleIterator, 20.0);

			doubleList.PushBack(30);
			doubleList.PushBack(40);
			doubleList.PushBack(50);
			doubleList.Remove(40);
			doubleList.Remove(50);

			doubleIterator = doubleList.begin();
			Assert::AreEqual(*doubleIterator, 20.0);

			doubleList.Remove(doubleList.begin());
			doubleList.Remove(doubleList.begin());

			Assert::IsTrue(doubleList.IsEmpty());


			/* Foo Data */
	
			SList<Foo> fooList;

			Assert::ExpectException<std::runtime_error>([&fooList] { fooList.Remove(SList<Foo>::Iterator()); });

			fooList.PushBack(Foo(10));
			fooList.PushBack(Foo(20));
			fooList.Remove(fooList.begin());

			SList<Foo>::Iterator fooIterator = fooList.begin();
			Assert::AreEqual(*fooIterator, Foo(20));

			fooList.PushBack(Foo(30));
			fooList.PushBack(Foo(40));
			fooList.PushBack(Foo(50));
			fooList.Remove(Foo(40));
			fooList.Remove(Foo(50));

			fooIterator = fooList.begin();
			Assert::AreEqual(*fooIterator, Foo(20));

			fooList.Remove(fooList.begin());
			fooList.Remove(fooList.begin());

			Assert::IsTrue(fooList.IsEmpty());
		}

		TEST_METHOD(Clear)
		{
			/* Integer Data */

			SList<int> intList;
		
			// Clearing an empty list does not change emptiness or size
			intList.Clear();
			Assert::IsTrue(intList.IsEmpty());
			Assert::AreEqual(intList.Size(), 0_z);

			intList = { 10, 20, 30 };

			// Clearing a filled lists empties correctly and sets size to zero
			intList.Clear();
			Assert::IsTrue(intList.IsEmpty());
			Assert::AreEqual(intList.Size(), 0_z);


			/* Double Data */

			SList<double> doubleList;

			// Clearing an empty list does not change emptiness or size
			doubleList.Clear();
			Assert::IsTrue(doubleList.IsEmpty());
			Assert::AreEqual(doubleList.Size(), 0_z);

			doubleList = { 10, 20, 30 };

			// Clearing a filled lists empties correctly and sets size to zero
			doubleList.Clear();
			Assert::IsTrue(doubleList.IsEmpty());
			Assert::AreEqual(doubleList.Size(), 0_z);
			

			/* Foo Data */

			SList<Foo> fooList;

			// Clearing an empty list does not change emptiness or size
			fooList.Clear();
			Assert::IsTrue(fooList.IsEmpty());
			Assert::AreEqual(fooList.Size(), 0_z);

			fooList = { Foo(10), Foo(20), Foo(30) };

			// Clearing a filled lists empties correctly and sets size to zero
			fooList.Clear();
			Assert::IsTrue(fooList.IsEmpty());
			Assert::AreEqual(fooList.Size(), 0_z);
		}

	
	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState SListTest::sStartMemState;
}
