#include "pch.h"

#include <crtdbg.h>

#include "CppUnitTest.h"

#include "ToStringSpecialization.h"
#include "Foo.h"
#include "SList.h"


using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;
using namespace Library;


namespace UnitTestLibraryDesktop
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

		TEST_METHOD(EqualityOperators)
		{
			/* Integer Data */

			SList<int> intList1;
			SList<int> intList2;

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
			size_t size;

			/* Integer Data */

			// Size is zero
			size = 0;
			SList<int> intList;
			Assert::AreEqual(intList.Size(), size);

			const SList<int> emptyConstIntList = intList;
			Assert::AreEqual(emptyConstIntList.Size(), size);

			// Size increased
			size++;
			intList.PushBack(10);
			Assert::AreEqual(intList.Size(), size);

			const SList<int> filledConstIntList = intList;
			Assert::AreEqual(filledConstIntList.Size(), size);

			// Size decreased
			size--;
			intList.PopBack();
			Assert::AreEqual(intList.Size(), size);
			const SList<int> decreasedConstIntList = intList;
			Assert::AreEqual(decreasedConstIntList.Size(), size);


			/* Double Data */

			// Size is zero
			size = 0;
			SList<double> doubleList;
			Assert::AreEqual(doubleList.Size(), size);

			const SList<double> emptyConstDoubleList = doubleList;
			Assert::AreEqual(emptyConstDoubleList.Size(), size);

			// Size increased
			size++;
			doubleList.PushBack(10);
			Assert::AreEqual(doubleList.Size(), size);

			const SList<double> filledConstDoubleList = doubleList;
			Assert::AreEqual(filledConstDoubleList.Size(), size);

			// Size decreased
			size--;
			doubleList.PopBack();
			Assert::AreEqual(doubleList.Size(), size);
			const SList<double> decreasedConstDoubleList = doubleList;
			Assert::AreEqual(decreasedConstDoubleList.Size(), size);


			/* Foo Data */

			// Size is zero
			size = 0;
			SList<Foo> fooList;
			Assert::AreEqual(fooList.Size(), size);
			const SList<Foo> emptyConstFooList = fooList;
			Assert::AreEqual(emptyConstFooList.Size(), size);

			// Size increased
			size++;
			fooList.PushBack(Foo(10));
			Assert::AreEqual(fooList.Size(), size);
			const SList<Foo> filledConstFooList = fooList;
			Assert::AreEqual(filledConstFooList.Size(), size);

			// Size decreased
			size--;
			fooList.PopBack();
			Assert::AreEqual(fooList.Size(), size);
			const SList<Foo> decreasedConstFooList = fooList;
			Assert::AreEqual(decreasedConstFooList.Size(), size);
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

		TEST_METHOD(PushFront)
		{
			SList<int> intList;
			SList<double> doubleList;
			SList<Foo> fooList;

			// Empty list exception when calling Front
			Assert::ExpectException<std::runtime_error>([&intList] { intList.Front(); });
			Assert::ExpectException<std::runtime_error>([&doubleList] { doubleList.Front(); });
			Assert::ExpectException<std::runtime_error>([&fooList] { fooList.Front(); });

			intList.PushFront(10);
			doubleList.PushFront(10);
			fooList.PushFront(Foo(10));

			// Data was set correctly
			Assert::AreEqual(intList.Front(), 10);
			Assert::AreEqual(doubleList.Front(), 10.0);
			Assert::AreEqual(fooList.Front(), Foo(10));

			// First data value sets front and back references
			Assert::AreEqual(intList.Front(), intList.Back());
			Assert::AreEqual(doubleList.Front(), doubleList.Back());
			Assert::AreEqual(fooList.Front(), fooList.Back());

			// Non-empty
			Assert::IsFalse(intList.IsEmpty());
			Assert::IsFalse(doubleList.IsEmpty());
			Assert::IsFalse(fooList.IsEmpty());

			// Size of 1
			size_t size = 1;
			Assert::AreEqual(intList.Size(), size);
			Assert::AreEqual(doubleList.Size(), size);
			Assert::AreEqual(fooList.Size(), size);

			intList.PushFront(20);
			doubleList.PushFront(20);
			fooList.PushFront(Foo(20));

			// Data was set correctly
			Assert::AreEqual(intList.Front(), 20);
			Assert::AreEqual(doubleList.Front(), 20.0);
			Assert::AreEqual(fooList.Front(), Foo(20));

			// Size of 2
			size = 2;
			Assert::AreEqual(intList.Size(), size);
			Assert::AreEqual(doubleList.Size(), size);
			Assert::AreEqual(fooList.Size(), size);
		}

		TEST_METHOD(PopFront)
		{
			SList<int> intList;
			SList<double> doubleList;
			SList<Foo> fooList;

			intList.PushFront(10);
			doubleList.PushFront(10);
			fooList.PushFront(Foo(10));

			intList.PushFront(20);
			doubleList.PushFront(20);
			fooList.PushFront(Foo(20));

			intList.PushFront(30);
			doubleList.PushFront(30);
			fooList.PushFront(Foo(30));

			intList.PopFront();
			doubleList.PopFront();
			fooList.PopFront();

			// Front is set correctly with more than 2 elements
			Assert::AreEqual(intList.Front(), 20);
			Assert::AreEqual(doubleList.Front(), 20.0);
			Assert::AreEqual(fooList.Front(), Foo(20));

			// Size is reduced correctly with more than 2 elements
			size_t size = 2;
			Assert::AreEqual(intList.Size(), size);
			Assert::AreEqual(doubleList.Size(), size);
			Assert::AreEqual(fooList.Size(), size);

			intList.PopFront();
			doubleList.PopFront();
			fooList.PopFront();

			// Front is set correctly with 1 element remaining
			Assert::AreEqual(intList.Front(), 10);
			Assert::AreEqual(doubleList.Front(), 10.0);
			Assert::AreEqual(fooList.Front(), Foo(10));

			// Size is reduced correctly with 1 element remaining
			size = 1;
			Assert::AreEqual(intList.Size(), size);
			Assert::AreEqual(doubleList.Size(), size);
			Assert::AreEqual(fooList.Size(), size);

			// Back is set correctly with 1 element remaining
			Assert::AreEqual(intList.Front(), intList.Back());
			Assert::AreEqual(doubleList.Front(), doubleList.Back());
			Assert::AreEqual(fooList.Front(), fooList.Back());

			intList.PopFront();
			doubleList.PopFront();
			fooList.PopFront();

			// Size is reduced correctly with no elements remaining
			size = 0;
			Assert::AreEqual(intList.Size(), size);
			Assert::AreEqual(doubleList.Size(), size);
			Assert::AreEqual(fooList.Size(), size);

			// Now empty
			Assert::IsTrue(intList.IsEmpty());
			Assert::IsTrue(doubleList.IsEmpty());
			Assert::IsTrue(fooList.IsEmpty());

			intList.PopFront();
			doubleList.PopFront();
			fooList.PopFront();

			// Size does not change when called on empty list
			size = 0;
			Assert::AreEqual(intList.Size(), size);
			Assert::AreEqual(doubleList.Size(), size);
			Assert::AreEqual(fooList.Size(), size);

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

			// Empty list exception when calling Back
			Assert::ExpectException<std::runtime_error>([&intList] { intList.Back(); });
			Assert::ExpectException<std::runtime_error>([&doubleList] { doubleList.Back(); });
			Assert::ExpectException<std::runtime_error>([&fooList] { fooList.Back(); });

			intList.PushBack(10);
			doubleList.PushBack(10);
			fooList.PushBack(Foo(10));

			// Data was set correctly
			Assert::AreEqual(intList.Back(), 10);
			Assert::AreEqual(doubleList.Back(), 10.0);
			Assert::AreEqual(fooList.Back(), Foo(10));

			// First data value sets front and back references
			Assert::AreEqual(intList.Back(), intList.Back());
			Assert::AreEqual(doubleList.Back(), doubleList.Back());
			Assert::AreEqual(fooList.Back(), fooList.Front());

			// Non-empty
			Assert::IsFalse(intList.IsEmpty());
			Assert::IsFalse(doubleList.IsEmpty());
			Assert::IsFalse(fooList.IsEmpty());

			// Size of 1
			size_t size = 1;
			Assert::AreEqual(intList.Size(), size);
			Assert::AreEqual(doubleList.Size(), size);
			Assert::AreEqual(fooList.Size(), size);

			intList.PushBack(20);
			doubleList.PushBack(20);
			fooList.PushBack(Foo(20));

			// Data was set correctly
			Assert::AreEqual(intList.Back(), 20);
			Assert::AreEqual(doubleList.Back(), 20.0);
			Assert::AreEqual(fooList.Back(), Foo(20));

			// Size of 2
			size = 2;
			Assert::AreEqual(intList.Size(), size);
			Assert::AreEqual(doubleList.Size(), size);
			Assert::AreEqual(fooList.Size(), size);
		}

		TEST_METHOD(PopBack)
		{
			SList<int> intList;
			SList<double> doubleList;
			SList<Foo> fooList;

			intList.PushBack(10);
			doubleList.PushBack(10);
			fooList.PushBack(Foo(10));

			intList.PushBack(20);
			doubleList.PushBack(20);
			fooList.PushBack(Foo(20));

			intList.PushBack(30);
			doubleList.PushBack(30);
			fooList.PushBack(Foo(30));

			intList.PopBack();
			doubleList.PopBack();
			fooList.PopBack();

			// Back is set correctly with more than 2 elements
			Assert::AreEqual(intList.Back(), 20);
			Assert::AreEqual(doubleList.Back(), 20.0);
			Assert::AreEqual(fooList.Back(), Foo(20));

			// Size is reduced correctly with more than 2 elements
			size_t size = 2;
			Assert::AreEqual(intList.Size(), size);
			Assert::AreEqual(doubleList.Size(), size);
			Assert::AreEqual(fooList.Size(), size);

			intList.PopBack();
			doubleList.PopBack();
			fooList.PopBack();

			// Back is set correctly with 1 element remaining
			Assert::AreEqual(intList.Back(), 10);
			Assert::AreEqual(doubleList.Back(), 10.0);
			Assert::AreEqual(fooList.Back(), Foo(10));

			// Size is reduced correctly with 1 element remaining
			size = 1;
			Assert::AreEqual(intList.Size(), size);
			Assert::AreEqual(doubleList.Size(), size);
			Assert::AreEqual(fooList.Size(), size);

			// Back is set correctly with 1 element remaining
			Assert::AreEqual(intList.Back(), intList.Back());
			Assert::AreEqual(doubleList.Back(), doubleList.Back());
			Assert::AreEqual(fooList.Back(), fooList.Front());

			intList.PopBack();
			doubleList.PopBack();
			fooList.PopBack();

			// Size is reduced correctly with no elements remaining
			size = 0;
			Assert::AreEqual(intList.Size(), size);
			Assert::AreEqual(doubleList.Size(), size);
			Assert::AreEqual(fooList.Size(), size);

			// Now empty
			Assert::IsTrue(intList.IsEmpty());
			Assert::IsTrue(doubleList.IsEmpty());
			Assert::IsTrue(fooList.IsEmpty());

			intList.PopBack();
			doubleList.PopBack();
			fooList.PopBack();

			// Size does not change when called on empty list
			size = 0;
			Assert::AreEqual(intList.Size(), size);
			Assert::AreEqual(doubleList.Size(), size);
			Assert::AreEqual(fooList.Size(), size);

			// Emptiness does not change when called on empty list
			Assert::IsTrue(intList.IsEmpty());
			Assert::IsTrue(doubleList.IsEmpty());
			Assert::IsTrue(fooList.IsEmpty());
		}

		TEST_METHOD(Clear)
		{
			/* Integer Data */

			SList<int> intList;
			size_t sizeZero = 0;
		
			// Clearing an empty list does not change emptiness or size
			intList.Clear();
			Assert::IsTrue(intList.IsEmpty());
			Assert::AreEqual(intList.Size(), sizeZero);

			intList.PushBack(10);
			intList.PushBack(20);
			intList.PushBack(30);

			// Clearing a filled lists empties correctly and sets size to zero
			intList.Clear();
			Assert::IsTrue(intList.IsEmpty());
			Assert::AreEqual(intList.Size(), sizeZero);


			/* Double Data */

			SList<double> doubleList;

			// Clearing an empty list does not change emptiness or size
			doubleList.Clear();
			Assert::IsTrue(doubleList.IsEmpty());
			Assert::AreEqual(doubleList.Size(), sizeZero);

			doubleList.PushBack(10);
			doubleList.PushBack(20);
			doubleList.PushBack(30);

			// Clearing a filled lists empties correctly and sets size to zero
			doubleList.Clear();
			Assert::IsTrue(doubleList.IsEmpty());
			Assert::AreEqual(doubleList.Size(), sizeZero);
			

			/* Foo Data */

			SList<Foo> fooList;

			// Clearing an empty list does not change emptiness or size
			fooList.Clear();
			Assert::IsTrue(fooList.IsEmpty());
			Assert::AreEqual(fooList.Size(), sizeZero);

			fooList.PushBack(Foo(10));
			fooList.PushBack(Foo(20));
			fooList.PushBack(Foo(30));

			// Clearing a filled lists empties correctly and sets size to zero
			fooList.Clear();
			Assert::IsTrue(fooList.IsEmpty());
			Assert::AreEqual(fooList.Size(), sizeZero);
		}

		TEST_METHOD(Copy)
		{
			/* Integer Data */

			SList<int> intList;
			intList.PushBack(10);
			intList.PushBack(20);
			intList.PushBack(30);

			// Validates copies are equal to source
			SList<int> intListCopy(intList);
			Assert::AreEqual(intListCopy, intList);
			intListCopy = intList;
			Assert::AreEqual(intListCopy, intList);
			

			/* Double Data */

			SList<double> doubleList;
			doubleList.PushBack(10);
			doubleList.PushBack(20);
			doubleList.PushBack(30);

			// Validates copies are equal to source		
			SList<double> doubleListCopy(doubleList);
			Assert::AreEqual(doubleListCopy, doubleList);
			doubleListCopy = doubleList;
			Assert::AreEqual(doubleListCopy, doubleList);
			

			/* Foo Data */

			SList<Foo> fooList;
			fooList.PushBack(Foo(10));
			fooList.PushBack(Foo(20));
			fooList.PushBack(Foo(30));

			// Validates copies are equal to source
			SList<Foo> fooListCopy(fooList);
			Assert::AreEqual(fooListCopy, fooList);
			fooListCopy = fooList;
			Assert::AreEqual(fooListCopy, fooList);
		}
	
	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState SListTest::sStartMemState;
}
