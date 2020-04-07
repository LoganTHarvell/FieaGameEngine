#include "pch.h"

#include "ToStringSpecialization.h"
#include "Foo.h"
#include "Bar.h"
#include "Vector.h"


using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;
using namespace Library;

namespace ContainerTests
{
	TEST_CLASS(VectorTest)
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
			Vector<int> intVector;
			Vector<double> doubleVector;
			Vector<Foo> fooVector;

			Vector<int>::Iterator intIterator1 = Vector<int>::Iterator(intVector.begin());
			Vector<double>::Iterator doubleIterator1 = Vector<double>::Iterator(doubleVector.begin());
			Vector<Foo>::Iterator fooIterator1 = Vector<Foo>::Iterator(fooVector.begin());

			Vector<int>::Iterator intIterator2 = intVector.begin();
			Vector<double>::Iterator doubleIterator2 = doubleVector.begin();
			Vector<Foo>::Iterator fooIterator2 = fooVector.begin();

			Assert::AreEqual(intIterator1, intIterator2);
			Assert::AreEqual(doubleIterator1, doubleIterator2);
			Assert::AreEqual(fooIterator1, fooIterator2);

			Vector<int>::ConstIterator constIntIterator1 = Vector<int>::ConstIterator(intVector.begin());
			Vector<double>::ConstIterator constDoubleIterator1 = Vector<double>::ConstIterator(doubleVector.begin());
			Vector<Foo>::ConstIterator constFooIterator1 = Vector<Foo>::ConstIterator(fooVector.begin());

			Vector<int>::ConstIterator constIntIterator2 = intVector.begin();
			Vector<double>::ConstIterator constDoubleIterator2 = doubleVector.begin();
			Vector<Foo>::ConstIterator constFooIterator2 = fooVector.begin();

			Assert::AreEqual(constIntIterator1, constIntIterator2);
			Assert::AreEqual(constDoubleIterator1, constDoubleIterator2);
			Assert::AreEqual(constFooIterator1, constFooIterator2);
		}

		TEST_METHOD(IteratorDereference)
		{
			Assert::ExpectException<std::runtime_error>([] { *Vector<int>::Iterator(); });
			Assert::ExpectException<std::runtime_error>([] { *Vector<double>::Iterator(); });
			Assert::ExpectException<std::runtime_error>([] { *Vector<Foo>::Iterator(); });
			Assert::ExpectException<std::runtime_error>([] { Vector<Foo>::Iterator()->Data(); });

			Assert::ExpectException<std::runtime_error>([] { *Vector<int>::ConstIterator(); });
			Assert::ExpectException<std::runtime_error>([] { *Vector<double>::ConstIterator(); });
			Assert::ExpectException<std::runtime_error>([] { *Vector<Foo>::ConstIterator(); });
			Assert::ExpectException<std::runtime_error>([] { Vector<Foo>::ConstIterator()->Data(); });

			Vector<int> intVector;
			Vector<double> doubleVector;
			Vector<Foo> fooVector;

			Assert::ExpectException<std::out_of_range>([&intVector] { *intVector.begin(); });
			Assert::ExpectException<std::out_of_range>([&doubleVector] { *doubleVector.begin(); });
			Assert::ExpectException<std::out_of_range>([&fooVector] { *fooVector.begin(); });
			Assert::ExpectException<std::out_of_range>([&fooVector] { fooVector.begin()->Data(); });

			Assert::ExpectException<std::out_of_range>([&intVector] { *intVector.cbegin(); });
			Assert::ExpectException<std::out_of_range>([&doubleVector] { *doubleVector.cbegin(); });
			Assert::ExpectException<std::out_of_range>([&fooVector] { *fooVector.cbegin(); });
			Assert::ExpectException<std::out_of_range>([&fooVector] { fooVector.cbegin()->Data(); });

			intVector.PushBack(10);
			doubleVector.PushBack(10);
			fooVector.PushBack(Foo(10));

			Assert::AreEqual(*intVector.begin(), 10);
			Assert::AreEqual(*doubleVector.begin(), 10.0);
			Assert::AreEqual(*fooVector.begin(), Foo(10));
			Assert::AreEqual(fooVector.begin()->Data(), 10);

			Assert::AreEqual(*intVector.cbegin(), 10);
			Assert::AreEqual(*doubleVector.cbegin(), 10.0);
			Assert::AreEqual(*fooVector.cbegin(), Foo(10));
			Assert::AreEqual(fooVector.cbegin()->Data(), 10);
		}

		TEST_METHOD(IteratorEquality)
		{
			Vector<int> intVector{ 10, 10 };
			Vector<double> doubleVector{ 10, 10 };
			Vector<Foo> fooVector{ Foo(10), Foo(10) };

			Vector<int>::Iterator intIterator = intVector.begin();
			Vector<double>::Iterator doubleIterator = doubleVector.begin();
			Vector<Foo>::Iterator fooIterator = fooVector.begin();

			Vector<int>::ConstIterator constIntIterator = intVector.cbegin();
			Vector<double>::ConstIterator constDoubleIterator = doubleVector.cbegin();
			Vector<Foo>::ConstIterator constFooIterator = fooVector.cbegin();

			Assert::AreEqual(intIterator, intVector.begin());
			Assert::AreEqual(doubleIterator, doubleVector.begin());
			Assert::AreEqual(fooIterator, fooVector.begin());

			Assert::AreEqual(constIntIterator, intVector.cbegin());
			Assert::AreEqual(constDoubleIterator, doubleVector.cbegin());
			Assert::AreEqual(constFooIterator, fooVector.cbegin());

			Vector<int>::Iterator prevIntIterator = intIterator++;
			Vector<double>::Iterator prevDoubleIterator = doubleIterator++;
			Vector<Foo>::Iterator prevFooIterator = fooIterator++;

			Vector<int>::ConstIterator prevConstIntIterator = constIntIterator++;
			Vector<double>::ConstIterator prevConstDoubleIterator = constDoubleIterator++;
			Vector<Foo>::ConstIterator prevConstFooIterator = constFooIterator++;

			Assert::IsTrue(intIterator != prevIntIterator);
			Assert::IsTrue(doubleIterator != prevDoubleIterator);
			Assert::IsTrue(fooIterator != prevFooIterator);

			Assert::IsTrue(constIntIterator != prevConstIntIterator);
			Assert::IsTrue(constDoubleIterator != prevConstDoubleIterator);
			Assert::IsTrue(constFooIterator != prevConstFooIterator);
		}

		TEST_METHOD(IteratorArithmetic)
		{
			Vector<int>::Iterator intIterator;
			Vector<double>::Iterator doubleIterator;
			Vector<Foo>::Iterator fooIterator;

			Vector<int>::ConstIterator intConstIterator;
			Vector<double>::ConstIterator doubleConstIterator;
			Vector<Foo>::ConstIterator fooConstIterator;

			Assert::ExpectException<std::runtime_error>([&intIterator] { ++intIterator; });
			Assert::ExpectException<std::runtime_error>([&doubleIterator] { ++doubleIterator; });
			Assert::ExpectException<std::runtime_error>([&fooIterator] { ++fooIterator; });

			Assert::ExpectException<std::runtime_error>([&intConstIterator] { ++intConstIterator; });
			Assert::ExpectException<std::runtime_error>([&doubleConstIterator] { ++doubleConstIterator; });
			Assert::ExpectException<std::runtime_error>([&fooConstIterator] { ++fooConstIterator; });

			Assert::ExpectException<std::runtime_error>([&intIterator] { --intIterator; });
			Assert::ExpectException<std::runtime_error>([&doubleIterator] { --doubleIterator; });
			Assert::ExpectException<std::runtime_error>([&fooIterator] { --fooIterator; });

			Assert::ExpectException<std::runtime_error>([&intConstIterator] { --intConstIterator; });
			Assert::ExpectException<std::runtime_error>([&doubleConstIterator] { --doubleConstIterator; });
			Assert::ExpectException<std::runtime_error>([&fooConstIterator] { --fooConstIterator; });

			Assert::ExpectException<std::runtime_error>([&intIterator] { intIterator += 1; });
			Assert::ExpectException<std::runtime_error>([&doubleIterator] { doubleIterator += 1; });
			Assert::ExpectException<std::runtime_error>([&fooIterator] { fooIterator += 1; });

			Assert::ExpectException<std::runtime_error>([&intConstIterator] { intConstIterator += 1; });
			Assert::ExpectException<std::runtime_error>([&doubleConstIterator] { doubleConstIterator += 1; });
			Assert::ExpectException<std::runtime_error>([&fooConstIterator] { fooConstIterator += 1; });

			Assert::ExpectException<std::runtime_error>([&intIterator] { intIterator -= 1; });
			Assert::ExpectException<std::runtime_error>([&doubleIterator] { doubleIterator -= 1; });
			Assert::ExpectException<std::runtime_error>([&fooIterator] { fooIterator -= 1; });

			Assert::ExpectException<std::runtime_error>([&intConstIterator] { intConstIterator -= 1; });
			Assert::ExpectException<std::runtime_error>([&doubleConstIterator] { doubleConstIterator -= 1; });
			Assert::ExpectException<std::runtime_error>([&fooConstIterator] { fooConstIterator -= 1; });

			Vector<int> intVector = { 10, 20, 30 };
			Vector<double> doubleVector = { 10, 20, 30 };
			Vector<Foo> fooVector = { Foo(10), Foo(20), Foo(30) };

			intIterator = intVector.end();
			doubleIterator = doubleVector.end();
			fooIterator = fooVector.end();

			intConstIterator = intVector.cend();
			doubleConstIterator = doubleVector.cend();
			fooConstIterator = fooVector.cend();

			Assert::ExpectException<std::out_of_range>([&intIterator] { ++intIterator; });
			Assert::ExpectException<std::out_of_range>([&doubleIterator] { ++doubleIterator; });
			Assert::ExpectException<std::out_of_range>([&fooIterator] { ++fooIterator; });

			Assert::ExpectException<std::out_of_range>([&intConstIterator] { ++intConstIterator; });
			Assert::ExpectException<std::out_of_range>([&doubleConstIterator] { ++doubleConstIterator; });
			Assert::ExpectException<std::out_of_range>([&fooConstIterator] { ++fooConstIterator; });

			Assert::ExpectException<std::out_of_range>([&intIterator] { intIterator += 1; });
			Assert::ExpectException<std::out_of_range>([&doubleIterator] { doubleIterator += 1; });
			Assert::ExpectException<std::out_of_range>([&fooIterator] { fooIterator += 1; });

			Assert::ExpectException<std::out_of_range>([&intConstIterator] { intConstIterator += 1; });
			Assert::ExpectException<std::out_of_range>([&doubleConstIterator] { doubleConstIterator += 1; });
			Assert::ExpectException<std::out_of_range>([&fooConstIterator] { fooConstIterator += 1; });

			intIterator = intVector.begin();
			doubleIterator = doubleVector.begin();
			fooIterator = fooVector.begin();

			intConstIterator = intVector.cbegin();
			doubleConstIterator = doubleVector.cbegin();
			fooConstIterator = fooVector.cbegin();

			Assert::ExpectException<std::out_of_range>([&intIterator] { --intIterator; });
			Assert::ExpectException<std::out_of_range>([&doubleIterator] { --doubleIterator; });
			Assert::ExpectException<std::out_of_range>([&fooIterator] { --fooIterator; });

			Assert::ExpectException<std::out_of_range>([&intConstIterator] { --intConstIterator; });
			Assert::ExpectException<std::out_of_range>([&doubleConstIterator] { --doubleConstIterator; });
			Assert::ExpectException<std::out_of_range>([&fooConstIterator] { --fooConstIterator; });

			Assert::ExpectException<std::out_of_range>([&intIterator] { intIterator -= 1; });
			Assert::ExpectException<std::out_of_range>([&doubleIterator] { doubleIterator -= 1; });
			Assert::ExpectException<std::out_of_range>([&fooIterator] { fooIterator -= 1; });

			Assert::ExpectException<std::out_of_range>([&intConstIterator] { intConstIterator -= 1; });
			Assert::ExpectException<std::out_of_range>([&doubleConstIterator] { doubleConstIterator -= 1; });
			Assert::ExpectException<std::out_of_range>([&fooConstIterator] { fooConstIterator -= 1; });

			intIterator = intVector.begin();
			doubleIterator = doubleVector.begin();
			fooIterator = fooVector.begin();

			intConstIterator = intVector.cbegin();
			doubleConstIterator = doubleVector.cbegin();
			fooConstIterator = fooVector.cbegin();

			Assert::AreEqual(*intIterator, 10);
			Assert::AreEqual(*doubleIterator, 10.0);
			Assert::AreEqual(*fooIterator, Foo(10));

			Assert::AreEqual(*intConstIterator, 10);
			Assert::AreEqual(*doubleConstIterator, 10.0);
			Assert::AreEqual(*fooConstIterator, Foo(10));

			Assert::AreEqual(intIterator[2], 30);
			Assert::AreEqual(doubleIterator[2], 30.0);
			Assert::AreEqual(fooIterator[2], Foo(30));

			Assert::AreEqual(intConstIterator[2], 30);
			Assert::AreEqual(doubleConstIterator[2], 30.0);
			Assert::AreEqual(fooConstIterator[2], Foo(30));

			Vector<int>::Iterator intPrevIterator = intIterator++;
			Vector<double>::Iterator doublePrevIterator = doubleIterator++;
			Vector<Foo>::Iterator fooPrevIterator = fooIterator++;

			Assert::AreEqual(*intPrevIterator, 10);
			Assert::AreEqual(*doublePrevIterator, 10.0);
			Assert::AreEqual(*fooPrevIterator, Foo(10));

			Assert::AreEqual(*intIterator, 20);
			Assert::AreEqual(*doubleIterator, 20.0);
			Assert::AreEqual(*fooIterator, Foo(20));

			Vector<int>::ConstIterator intPrevConstIterator = intConstIterator++;
			Vector<double>::ConstIterator doublePrevConstIterator = doubleConstIterator++;
			Vector<Foo>::ConstIterator fooPrevConstIterator = fooConstIterator++;

			Assert::AreEqual(*intPrevConstIterator, 10);
			Assert::AreEqual(*doublePrevConstIterator, 10.0);
			Assert::AreEqual(*fooPrevConstIterator, Foo(10));

			Assert::AreEqual(*intConstIterator, 20);
			Assert::AreEqual(*doubleConstIterator, 20.0);
			Assert::AreEqual(*fooConstIterator, Foo(20));

			intIterator = intIterator + 1;
			doubleIterator = doubleIterator + 1;
			fooIterator = fooIterator + 1;

			intConstIterator = intConstIterator + 1;
			doubleConstIterator = doubleConstIterator + 1;
			fooConstIterator = fooConstIterator + 1;

			Assert::AreEqual(*(intIterator--), 30);
			Assert::AreEqual(*(doubleIterator--), 30.0);
			Assert::AreEqual(*(fooIterator--), Foo(30));

			Assert::AreEqual(*(intConstIterator--), 30);
			Assert::AreEqual(*(doubleConstIterator--), 30.0);
			Assert::AreEqual(*(fooConstIterator--), Foo(30));

			Assert::AreEqual(*intIterator, 20);
			Assert::AreEqual(*doubleIterator, 20.0);
			Assert::AreEqual(*fooIterator, Foo(20));

			Assert::AreEqual(*intConstIterator, 20);
			Assert::AreEqual(*doubleConstIterator, 20.0);
			Assert::AreEqual(*fooConstIterator, Foo(20));

			intIterator = intIterator - 1;
			doubleIterator = doubleIterator - 1;
			fooIterator = fooIterator - 1;

			intConstIterator = intConstIterator - 1;
			doubleConstIterator = doubleConstIterator - 1;
			fooConstIterator = fooConstIterator - 1;

			Assert::AreEqual(*intIterator, 10);
			Assert::AreEqual(*doubleIterator, 10.0);
			Assert::AreEqual(*fooIterator, Foo(10));

			Assert::AreEqual(*intConstIterator, 10);
			Assert::AreEqual(*doubleConstIterator, 10.0);
			Assert::AreEqual(*fooConstIterator, Foo(10));
	}

		TEST_METHOD(Initialization)
		{
			Vector<int> intVectorEmpty = Vector<int>(1);
			Vector<double> doubleVectorEmpty = Vector<double>(1);
			Vector<Foo> fooVectorEmpty = Vector<Foo>(1);

			Assert::AreEqual(intVectorEmpty.begin(), intVectorEmpty.end());
			Assert::AreEqual(doubleVectorEmpty.begin(), doubleVectorEmpty.end());
			Assert::AreEqual(fooVectorEmpty.begin(), fooVectorEmpty.end());

			Assert::AreEqual(intVectorEmpty.cbegin(), intVectorEmpty.cend());
			Assert::AreEqual(doubleVectorEmpty.cbegin(), doubleVectorEmpty.cend());
			Assert::AreEqual(fooVectorEmpty.cbegin(), fooVectorEmpty.cend());

			Assert::AreEqual(intVectorEmpty.Size(), 0_z);
			Assert::AreEqual(doubleVectorEmpty.Size(), 0_z);
			Assert::AreEqual(fooVectorEmpty.Size(), 0_z);

			Vector<int> intVectorFilled = { 10, 20, 30 };
			Vector<double> doubleVectorFilled = { 10, 20, 30 };
			Vector<Foo> fooVectorFilled = { Foo(10), Foo(20), Foo(30) };

			Assert::AreEqual(intVectorFilled.Size(), 3_z);
			Assert::AreEqual(doubleVectorFilled.Size(), 3_z);
			Assert::AreEqual(fooVectorFilled.Size(), 3_z);

			intVectorFilled = { 0 };
			doubleVectorFilled = { 0 };
			fooVectorFilled = { Foo(0) };

			Assert::AreEqual(intVectorFilled.Front(), 0);
			Assert::AreEqual(doubleVectorFilled.Front(), 0.0);
			Assert::AreEqual(fooVectorFilled.Front(), Foo(0));

			Assert::AreEqual(intVectorFilled.Size(), 1_z);
			Assert::AreEqual(doubleVectorFilled.Size(), 1_z);
			Assert::AreEqual(fooVectorFilled.Size(), 1_z);
		}

		TEST_METHOD(Copy)
		{
			Vector<int> intVector = { 10, 20, 30 };
			Vector<double> doubleVector = { 10, 20, 30 };
			Vector<Foo> fooVector = { Foo(10), Foo(20), Foo(30) };

			Vector<int> intVectorCopy(intVector);
			Assert::AreEqual(intVectorCopy, intVector);
			intVectorCopy = intVector;
			Assert::AreEqual(intVectorCopy, intVector);

			Vector<double> doubleVectorCopy(doubleVector);
			Assert::AreEqual(doubleVectorCopy, doubleVector);
			doubleVectorCopy = doubleVector;
			Assert::AreEqual(doubleVectorCopy, doubleVector);

			Vector<Foo> fooVectorCopy(fooVector);
			Assert::AreEqual(fooVectorCopy, fooVector);
			fooVectorCopy = fooVector;
			Assert::AreEqual(fooVectorCopy, fooVector);
		}

		TEST_METHOD(MoveOperators)
		{
			Vector<int> intVector = { 10, 20, 30 };
			Vector<double> doubleVector = { 10, 20, 30 };
			Vector<Foo> fooVector = { Foo(10), Foo(20), Foo(30) };


			/* Integer Data */

			// Validates copies are equal to source
			Vector<int> intVectorCopy = intVector;
			intVector = std::move(intVector);
			Assert::AreEqual(intVectorCopy, intVector);

			// Validates moved element is equal to copy
			Vector<int> intVectorMoved = std::move(intVector);
			Assert::AreEqual(intVectorMoved, intVectorCopy);

			intVector.PushBack(10);
			intVector.PushBack(20);

			intVectorCopy = intVector;
			intVectorMoved = std::move(intVector);
			Assert::AreEqual(intVectorMoved, intVectorCopy);


			/* Double Data */

			// Validates copies are equal to source
			Vector<double> doubleVectorCopy = doubleVector;
			doubleVector = std::move(doubleVector);
			Assert::AreEqual(doubleVectorCopy, doubleVector);

			// Validates moved element is equal to copy
			Vector<double> doubleVectorMoved = std::move(doubleVector);
			Assert::AreEqual(doubleVectorMoved, doubleVectorCopy);

			doubleVector.PushBack(10);
			doubleVector.PushBack(20);

			doubleVectorCopy = doubleVector;
			doubleVectorMoved = std::move(doubleVector);
			Assert::AreEqual(doubleVectorMoved, doubleVectorCopy);


			/* Foo Data */

			// Validates copies are equal to source
			Vector<Foo> fooVectorCopy = fooVector;
			fooVector = std::move(fooVector);
			Assert::AreEqual(fooVectorCopy, fooVector);

			// Validates moved element is equal to copy
			Vector<Foo> fooVectorMoved = std::move(fooVector);
			Assert::AreEqual(fooVectorMoved, fooVectorCopy);

			fooVector.PushBack(Foo(10));
			fooVector.PushBack(Foo(20));

			fooVectorCopy = fooVector;
			fooVectorMoved = std::move(fooVector);
			Assert::AreEqual(fooVectorMoved, fooVectorCopy);
		}

		TEST_METHOD(EqualityOperators)
		{
			/* Integer Data */

			Vector<int> intVector1;
			Vector<int> intVector2;

			// Equality with self
			Assert::AreEqual(intVector1, intVector1);

			// Equality when empty
			Assert::AreEqual(intVector1, intVector2);
			const Vector<int> emptyConstIntVector;
			Assert::AreEqual(emptyConstIntVector, intVector2);

			// Inequality when filled and different sizes
			intVector1.PushBack(10);
			intVector1.PushBack(20);
			intVector2.PushBack(10);
			Assert::IsTrue(intVector1 != intVector2);
			const Vector<int> filledConstIntVector1 = intVector1;
			Assert::IsTrue(filledConstIntVector1 != intVector2);

			// Equality when filled
			intVector2.PushBack(20);
			Assert::AreEqual(intVector1, intVector2);
			const Vector<int> filledConstIntVector2 = intVector1;
			Assert::AreEqual(filledConstIntVector2, intVector2);

			// Inequality when filled and equal size
			intVector2.PopBack();
			intVector2.PushBack(30);
			Assert::IsTrue(intVector1 != intVector2);
			Assert::IsTrue(filledConstIntVector2 != intVector2);


			/* Double Data */

			Vector<double> doubleVector1;
			Vector<double> doubleVector2;

			// Equality with self
			Assert::AreEqual(doubleVector1, doubleVector1);

			// Equality when empty
			Assert::AreEqual(doubleVector1, doubleVector2);
			const Vector<double> emptyConstDoubleVector = doubleVector1;
			Assert::AreEqual(emptyConstDoubleVector, doubleVector2);

			// Inequality when filled and different sizes
			doubleVector1.PushBack(10);
			doubleVector1.PushBack(20);
			doubleVector2.PushBack(10);
			Assert::IsTrue(doubleVector1 != doubleVector2);
			const Vector<double> filledConstDoubleVector1 = doubleVector1;
			Assert::IsTrue(filledConstDoubleVector1 != doubleVector2);

			// Equality when filled
			doubleVector2.PushBack(20);
			Assert::AreEqual(doubleVector1, doubleVector2);
			const Vector<double> filledConstDoubleVector2 = doubleVector1;
			Assert::AreEqual(filledConstDoubleVector2, doubleVector2);

			// Inequality when filled and equal size
			doubleVector2.PopBack();
			doubleVector2.PushBack(30);
			Assert::IsTrue(doubleVector1 != doubleVector2);
			Assert::IsTrue(filledConstDoubleVector2 != doubleVector2);


			/* Foo Data */

			Vector<Foo> fooVector1;
			Vector<Foo> fooVector2;

			// Equality with self
			Assert::AreEqual(fooVector1, fooVector1);

			// Equality when empty
			Assert::AreEqual(fooVector1, fooVector2);
			const Vector<Foo> emptyConstfooVector = fooVector1;
			Assert::AreEqual(emptyConstfooVector, fooVector2);

			// Inequality when filled and different sizes
			fooVector1.PushBack(Foo(10));
			fooVector1.PushBack(Foo(20));
			fooVector2.PushBack(Foo(10));

			Assert::IsTrue(fooVector1 != fooVector2);
			const Vector<Foo> filledConstfooVector1 = fooVector1;
			Assert::IsTrue(filledConstfooVector1 != fooVector2);

			// Equality when filled
			fooVector2.PushBack(Foo(20));
			Assert::AreEqual(fooVector1, fooVector2);
			const Vector<Foo> filledConstfooVector2 = fooVector1;
			Assert::AreEqual(filledConstfooVector2, fooVector2);

			// Inequality when filled and equal size
			fooVector2.PopBack();
			fooVector2.PushBack(Foo(30));
			Assert::IsTrue(fooVector1 != fooVector2);
			Assert::IsTrue(filledConstfooVector2 != fooVector2);
		}

		TEST_METHOD(SizeCapacity)
		{
			/* Integer Data */

			{
				Vector<int> doubleVector(0, DefaultEquality<int>(), [](const std::size_t, const std::size_t capacity) { return capacity * 2; });
				doubleVector.Resize(10);
				doubleVector.PushBack(10);

				Assert::AreEqual(doubleVector.Size(), 11_z);
				Assert::AreEqual(doubleVector.Capacity(), 20_z);
			}

			// Size is zero
			Vector<int> intVector;
			Assert::AreEqual(intVector.Size(), 0_z);
			Assert::AreEqual(intVector.Capacity(), 0_z);

			const Vector<int> emptyConstIntVector;
			Assert::AreEqual(emptyConstIntVector.Size(), 0_z);
			Assert::AreEqual(emptyConstIntVector.Capacity(), 0_z);

			// Capacity and Size increase
			intVector.Reserve(1);
			Assert::AreEqual(intVector.Capacity(), 1_z);
			Assert::AreEqual(intVector.Size(), 0_z);

			intVector.Resize(5);
			Assert::AreEqual(intVector.Capacity(), 5_z);
			Assert::AreEqual(intVector.Size(), 5_z);

			intVector.Resize(10, 10);
			Assert::AreEqual(intVector.Capacity(), 10_z);
			Assert::AreEqual(intVector.Size(), 10_z);

			for (std::size_t i = 0; i < 5; ++i)
			{
				Assert::AreEqual(intVector[i], int());
			}

			for (std::size_t i = 5; i < intVector.Size(); ++i)
			{
				Assert::AreEqual(intVector[i], 10);
			}

			intVector.Resize(5, 20);
			Assert::AreEqual(intVector.Size(), 5_z);
			Assert::AreEqual(intVector.Capacity(), 10_z);

			for (std::size_t i = 0; i < intVector.Size(); ++i)
			{
				Assert::AreEqual(intVector[i], int());
			}

			intVector.Resize(4);
			Assert::AreEqual(intVector.Size(), 4_z);
			Assert::AreEqual(intVector.Capacity(), 10_z);

			intVector.ShrinkToFit();
			Assert::AreEqual(intVector.Capacity(), 4_z);

			intVector.PushBack(10);
			Assert::AreEqual(intVector.Size(), 5_z);
			Assert::AreEqual(intVector.Capacity(), 6_z);

			// Size decreased
			intVector.PopBack();
			Assert::AreEqual(intVector.Size(), 4_z);
			Assert::AreEqual(intVector.Capacity(), 6_z);

			const Vector<int> decreasedConstIntVector = intVector;
			Assert::AreEqual(decreasedConstIntVector.Size(), 4_z);
			Assert::AreEqual(decreasedConstIntVector.Capacity(), 6_z);
			
			intVector.Clear();
			Assert::AreEqual(intVector.Size(), 0_z);
			intVector.ShrinkToFit();
			Assert::AreEqual(intVector.Capacity(), 0_z);
			Assert::IsTrue(intVector.IsEmpty());


			/* Double Data */

			{
				Vector<double> doubleVector(0, DefaultEquality<double>(), [](const std::size_t, const std::size_t capacity) { return capacity * 2; });
				doubleVector.Resize(10);
				doubleVector.PushBack(10);

				Assert::AreEqual(doubleVector.Size(), 11_z);
				Assert::AreEqual(doubleVector.Capacity(), 20_z);
			}

			// Size is zero
			Vector<double> doubleVector;
			Assert::AreEqual(doubleVector.Size(), 0_z);
			Assert::AreEqual(doubleVector.Capacity(), 0_z);

			const Vector<double> emptyConstdoubleVector;
			Assert::AreEqual(emptyConstdoubleVector.Size(), 0_z);
			Assert::AreEqual(emptyConstdoubleVector.Capacity(), 0_z);

			// Capacity and Size increase
			doubleVector.Reserve(1);
			Assert::AreEqual(doubleVector.Capacity(), 1_z);
			Assert::AreEqual(doubleVector.Size(), 0_z);

			doubleVector.Resize(5);
			Assert::AreEqual(doubleVector.Capacity(), 5_z);
			Assert::AreEqual(doubleVector.Size(), 5_z);

			doubleVector.Resize(10, 10);
			Assert::AreEqual(doubleVector.Capacity(), 10_z);
			Assert::AreEqual(doubleVector.Size(), 10_z);

			for (std::size_t i = 0; i < 5; ++i)
			{
				Assert::AreEqual(doubleVector[i], double());
			}

			for (std::size_t i = 5; i < doubleVector.Size(); ++i)
			{
				Assert::AreEqual(doubleVector[i], 10.0);
			}

			doubleVector.Resize(5, 20);
			Assert::AreEqual(doubleVector.Size(), 5_z);
			Assert::AreEqual(doubleVector.Capacity(), 10_z);

			for (std::size_t i = 0; i < doubleVector.Size(); ++i)
			{
				Assert::AreEqual(doubleVector[i], double());
			}

			doubleVector.Resize(4);
			Assert::AreEqual(doubleVector.Size(), 4_z);
			Assert::AreEqual(doubleVector.Capacity(), 10_z);

			doubleVector.ShrinkToFit();
			Assert::AreEqual(doubleVector.Capacity(), 4_z);

			doubleVector.PushBack(10);
			Assert::AreEqual(doubleVector.Size(), 5_z);
			Assert::AreEqual(doubleVector.Capacity(), 6_z);

			// Size decreased
			doubleVector.PopBack();
			Assert::AreEqual(doubleVector.Size(), 4_z);
			Assert::AreEqual(doubleVector.Capacity(), 6_z);

			const Vector<double> doubleDecreasedConstVector = doubleVector;
			Assert::AreEqual(doubleDecreasedConstVector.Size(), 4_z);
			Assert::AreEqual(doubleDecreasedConstVector.Capacity(), 6_z);

			doubleVector.Clear();
			Assert::AreEqual(doubleVector.Size(), 0_z);
			doubleVector.ShrinkToFit();
			Assert::AreEqual(doubleVector.Capacity(), 0_z);
			Assert::IsTrue(doubleVector.IsEmpty());


			/* Foo Data */

			{
				Vector<Foo> fooVector(0, DefaultEquality<Foo>(), [](const std::size_t, const std::size_t capacity) { return capacity * 2; });
				fooVector.Resize(10);
				fooVector.PushBack(Foo(10));

				Assert::AreEqual(fooVector.Size(), 11_z);
				Assert::AreEqual(fooVector.Capacity(), 20_z);
			}

			// Size is zero
			Vector<Foo> fooVector;
			Assert::AreEqual(fooVector.Size(), 0_z);
			Assert::AreEqual(fooVector.Capacity(), 0_z);

			const Vector<Foo> emptyConstfooVector;
			Assert::AreEqual(emptyConstfooVector.Size(), 0_z);
			Assert::AreEqual(emptyConstfooVector.Capacity(), 0_z);

			// Capacity and Size increase
			fooVector.Reserve(1);
			Assert::AreEqual(fooVector.Capacity(), 1_z);
			Assert::AreEqual(fooVector.Size(), 0_z);

			fooVector.Resize(5);
			Assert::AreEqual(fooVector.Capacity(), 5_z);
			Assert::AreEqual(fooVector.Size(), 5_z);

			fooVector.Resize(10, Foo(10));
			Assert::AreEqual(fooVector.Capacity(), 10_z);
			Assert::AreEqual(fooVector.Size(), 10_z);

			for (std::size_t i = 0; i < 5; ++i)
			{
				Assert::AreEqual(fooVector[i], Foo());
			}

			for (std::size_t i = 5; i < fooVector.Size(); ++i)
			{
				Assert::AreEqual(fooVector[i], Foo(10));
			}

			fooVector.Resize(5, Foo(20));
			Assert::AreEqual(fooVector.Size(), 5_z);
			Assert::AreEqual(fooVector.Capacity(), 10_z);

			for (std::size_t i = 0; i < fooVector.Size(); ++i)
			{
				Assert::AreEqual(fooVector[i], Foo());
			}

			fooVector.Resize(4);
			Assert::AreEqual(fooVector.Size(), 4_z);
			Assert::AreEqual(fooVector.Capacity(), 10_z);

			fooVector.ShrinkToFit();
			Assert::AreEqual(fooVector.Capacity(), 4_z);

			fooVector.PushBack(Foo(10));
			Assert::AreEqual(fooVector.Size(), 5_z);
			Assert::AreEqual(fooVector.Capacity(), 6_z);

			// Size decreased
			fooVector.PopBack();
			Assert::AreEqual(fooVector.Size(), 4_z);
			Assert::AreEqual(fooVector.Capacity(), 6_z);

			const Vector<Foo> decreasedConstfooVector = fooVector;
			Assert::AreEqual(decreasedConstfooVector.Size(), 4_z);
			Assert::AreEqual(decreasedConstfooVector.Capacity(), 6_z);

			fooVector.Clear();
			Assert::AreEqual(fooVector.Size(), 0_z);
			fooVector.ShrinkToFit();
			Assert::AreEqual(fooVector.Capacity(), 0_z);
			Assert::IsTrue(fooVector.IsEmpty());
		}

		TEST_METHOD(IsEmpty)
		{
			/* Integer Data */

			// Empty list and size
			Vector<int> intVector;
			Assert::IsTrue(intVector.IsEmpty());

			const Vector<int> emptyConstIntVector = intVector;
			Assert::IsTrue(emptyConstIntVector.IsEmpty());

			// Non-empty list and size
			intVector.PushBack(10);
			Assert::IsTrue(!intVector.IsEmpty());

			const Vector<int> filledConstIntVector = intVector;
			Assert::IsTrue(!filledConstIntVector.IsEmpty());

			/* Double Data */

			// Empty list and size
			Vector<double> doubleVector;
			Assert::IsTrue(doubleVector.IsEmpty());

			const Vector<double> emptyConstDoubleVector = doubleVector;
			Assert::IsTrue(emptyConstDoubleVector.IsEmpty());

			// Non-empty list and size
			doubleVector.PushBack(10);
			Assert::IsTrue(!doubleVector.IsEmpty());

			const Vector<double> filledConstDoubleVector = doubleVector;
			Assert::IsTrue(!filledConstDoubleVector.IsEmpty());

			/* Foo Data */

			// Empty list and size
			Vector<Foo> fooVector;
			Assert::IsTrue(fooVector.IsEmpty());

			const Vector<Foo> emptyConstfooVector = fooVector;
			Assert::IsTrue(emptyConstfooVector.IsEmpty());

			// Non-empty list and size
			fooVector.PushBack(Foo(10));
			Assert::IsTrue(!fooVector.IsEmpty());

			const Vector<Foo> filledConstfooVector = fooVector;
			Assert::IsTrue(!filledConstfooVector.IsEmpty());
		}

		TEST_METHOD(Begin)
		{
			Vector<int> intVector;
			Vector<double> doubleVector;
			Vector<Foo> fooVector;

			Assert::ExpectException<std::out_of_range>([&intVector] { *(intVector.begin()); });
			Assert::ExpectException<std::out_of_range>([&doubleVector] { *(doubleVector.begin()); });
			Assert::ExpectException<std::out_of_range>([&fooVector] { *(fooVector.begin()); });

			const Vector<int> emptyIntVectorCopy = intVector;
			const Vector<double> emptyDoubleVectorCopy = doubleVector;
			const Vector<Foo> emptyfooVectorCopy = fooVector;

			Assert::ExpectException<std::out_of_range>([&emptyIntVectorCopy] { *(emptyIntVectorCopy.begin()); });
			Assert::ExpectException<std::out_of_range>([&emptyDoubleVectorCopy] { *(emptyDoubleVectorCopy.begin()); });
			Assert::ExpectException<std::out_of_range>([&emptyfooVectorCopy] { *(emptyfooVectorCopy.begin()); });

			Assert::ExpectException<std::out_of_range>([&intVector] { *(intVector.cbegin()); });
			Assert::ExpectException<std::out_of_range>([&doubleVector] { *(doubleVector.cbegin()); });
			Assert::ExpectException<std::out_of_range>([&fooVector] { *(fooVector.cbegin()); });

			intVector.PushBack(10);
			doubleVector.PushBack(10);
			fooVector.PushBack(Foo(10));

			Assert::AreEqual(*intVector.begin(), 10);
			Assert::AreEqual(*doubleVector.begin(), 10.0);
			Assert::AreEqual(*fooVector.begin(), Foo(10));
		}

		TEST_METHOD(End)
		{
			Vector<int> intVector;
			Vector<double> doubleVector;
			Vector<Foo> fooVector;

			Assert::ExpectException<std::out_of_range>([&intVector] { *(intVector.end()); });
			Assert::ExpectException<std::out_of_range>([&doubleVector] { *(doubleVector.end()); });
			Assert::ExpectException<std::out_of_range>([&fooVector] { *(fooVector.end()); });

			const Vector<int> emptyIntVectorCopy = intVector;
			const Vector<double> emptyDoubleVectorCopy = doubleVector;
			const Vector<Foo> emptyfooVectorCopy = fooVector;

			Assert::ExpectException<std::out_of_range>([&emptyIntVectorCopy] { *(emptyIntVectorCopy.end()); });
			Assert::ExpectException<std::out_of_range>([&emptyDoubleVectorCopy] { *(emptyDoubleVectorCopy.end()); });
			Assert::ExpectException<std::out_of_range>([&emptyfooVectorCopy] { *(emptyfooVectorCopy.end()); });

			Assert::ExpectException<std::out_of_range>([&intVector] { *(intVector.cend()); });
			Assert::ExpectException<std::out_of_range>([&doubleVector] { *(doubleVector.cend()); });
			Assert::ExpectException<std::out_of_range>([&fooVector] { *(fooVector.cend()); });

			intVector.PushBack(10);
			doubleVector.PushBack(10);
			fooVector.PushBack(Foo(10));

			Assert::ExpectException<std::out_of_range>([&intVector] { *(intVector.end()); });
			Assert::ExpectException<std::out_of_range>([&intVector] { *(intVector.end()); });
			Assert::ExpectException<std::out_of_range>([&intVector] { *(intVector.end()); });

			const Vector<int> filledIntVectorCopy = intVector;
			const Vector<double> filledDoubleVectorCopy = doubleVector;
			const Vector<Foo> filledfooVectorCopy = fooVector;

			Assert::ExpectException<std::out_of_range>([&filledIntVectorCopy] { *(filledIntVectorCopy.end()); });
			Assert::ExpectException<std::out_of_range>([&filledDoubleVectorCopy] { *(filledDoubleVectorCopy.end()); });
			Assert::ExpectException<std::out_of_range>([&filledfooVectorCopy] { *(filledfooVectorCopy.end()); });

			Assert::ExpectException<std::out_of_range>([&intVector] { *(intVector.cend()); });
			Assert::ExpectException<std::out_of_range>([&doubleVector] { *(doubleVector.cend()); });
			Assert::ExpectException<std::out_of_range>([&fooVector] { *(fooVector.cend()); });

			Vector<int>::Iterator intIterator = intVector.begin();
			Vector<double>::Iterator doubleIterator = doubleVector.begin();
			Vector<Foo>::Iterator fooIterator = fooVector.begin();

			Vector<int>::ConstIterator constIntIterator = intVector.cbegin();
			Vector<double>::ConstIterator constDoubleIterator = doubleVector.cbegin();
			Vector<Foo>::ConstIterator constFooIterator = fooVector.cbegin();

			++intIterator;
			++doubleIterator;
			++fooIterator;

			++constIntIterator;
			++constDoubleIterator;
			++constFooIterator;

			Assert::AreEqual(intIterator, intVector.end());
			Assert::AreEqual(doubleIterator, doubleVector.end());
			Assert::AreEqual(fooIterator, fooVector.end());

			Assert::AreEqual(constIntIterator, intVector.cend());
			Assert::AreEqual(constDoubleIterator, doubleVector.cend());
			Assert::AreEqual(constFooIterator, fooVector.cend());
		}

		TEST_METHOD(RandomAccess)
		{
			Vector<int> intVector = { 10, 20, 30 };
			Vector<double> doubleVector = { 10, 20, 30 };
			Vector<Foo> fooVector = { Foo(10), Foo(20), Foo(30) };

			Assert::AreEqual(intVector[0], 10);
			Assert::AreEqual(doubleVector[0], 10.0);
			Assert::AreEqual(fooVector[0], Foo(10));

			Assert::AreEqual(intVector[1], 20);
			Assert::AreEqual(doubleVector[1], 20.0);
			Assert::AreEqual(fooVector[1], Foo(20));

			Assert::AreEqual(intVector.At(2), 30);
			Assert::AreEqual(doubleVector.At(2), 30.0);
			Assert::AreEqual(fooVector.At(2), Foo(30));

			Assert::ExpectException<std::out_of_range>([&intVector] { intVector[3]; });
			Assert::ExpectException<std::out_of_range>([&doubleVector] { doubleVector[3]; });
			Assert::ExpectException<std::out_of_range>([&fooVector] { fooVector[3]; });

			Assert::ExpectException<std::out_of_range>([&intVector] { intVector.At(3); });
			Assert::ExpectException<std::out_of_range>([&doubleVector] { doubleVector.At(3); });
			Assert::ExpectException<std::out_of_range>([&fooVector] { fooVector.At(3); });

			const Vector<int> intConstVector = { 10, 20, 30 };
			const Vector<double> doubleConstVector = { 10, 20, 30 };
			const Vector<Foo> fooConstVector = { Foo(10), Foo(20), Foo(30) };

			Assert::AreEqual(intConstVector[0], 10);
			Assert::AreEqual(doubleConstVector[0], 10.0);
			Assert::AreEqual(fooConstVector[0], Foo(10));

			Assert::AreEqual(intConstVector[1], 20);
			Assert::AreEqual(doubleConstVector[1], 20.0);
			Assert::AreEqual(fooConstVector[1], Foo(20));

			Assert::AreEqual(intConstVector.At(2), 30);
			Assert::AreEqual(doubleConstVector.At(2), 30.0);
			Assert::AreEqual(fooConstVector.At(2), Foo(30));

			Assert::ExpectException<std::out_of_range>([&intConstVector] { intConstVector[3]; });
			Assert::ExpectException<std::out_of_range>([&doubleConstVector] { doubleConstVector[3]; });
			Assert::ExpectException<std::out_of_range>([&fooConstVector] { fooConstVector[3]; });

			Assert::ExpectException<std::out_of_range>([&intConstVector] { intConstVector.At(3); });
			Assert::ExpectException<std::out_of_range>([&doubleConstVector] { doubleConstVector.At(3); });
			Assert::ExpectException<std::out_of_range>([&fooConstVector] { fooConstVector.At(3); });
		}

		TEST_METHOD(PushBack)
		{
			Vector<int> intVector;
			Vector<double> doubleVector;
			Vector<Foo> fooVector;

			const Vector<int> emptyConstIntVector;
			const Vector<double> emptyConstDoubleVector;
			const Vector<Foo> emptyConstfooVector;

			// Empty list exception when calling Front
			Assert::ExpectException<std::runtime_error>([&intVector] { intVector.Front(); });
			Assert::ExpectException<std::runtime_error>([&doubleVector] { doubleVector.Front(); });
			Assert::ExpectException<std::runtime_error>([&fooVector] { fooVector.Front(); });
			Assert::ExpectException<std::runtime_error>([&emptyConstIntVector] { emptyConstIntVector.Front(); });
			Assert::ExpectException<std::runtime_error>([&emptyConstDoubleVector] { emptyConstDoubleVector.Front(); });
			Assert::ExpectException<std::runtime_error>([&emptyConstfooVector] { emptyConstfooVector.Front(); });

			// Empty list exception when calling Back
			Assert::ExpectException<std::runtime_error>([&intVector] { intVector.Back(); });
			Assert::ExpectException<std::runtime_error>([&doubleVector] { doubleVector.Back(); });
			Assert::ExpectException<std::runtime_error>([&fooVector] { fooVector.Back(); });
			Assert::ExpectException<std::runtime_error>([&emptyConstIntVector] { emptyConstIntVector.Back(); });
			Assert::ExpectException<std::runtime_error>([&emptyConstDoubleVector] { emptyConstDoubleVector.Back(); });
			Assert::ExpectException<std::runtime_error>([&emptyConstfooVector] { emptyConstfooVector.Back(); });

			intVector.PushBack(10);
			doubleVector.PushBack(10);
			fooVector.PushBack(Foo(10));

			const Vector<int> filledConstIntVector1 = intVector;
			const Vector<double> filledConstDoubleVector1 = doubleVector;
			const Vector<Foo> filledConstfooVector1 = fooVector;

			// Data was set correctly
			Assert::AreEqual(intVector.Back(), 10);
			Assert::AreEqual(doubleVector.Back(), 10.0);
			Assert::AreEqual(fooVector.Back(), Foo(10));
			Assert::AreEqual(filledConstIntVector1.Back(), 10);
			Assert::AreEqual(filledConstDoubleVector1.Back(), 10.0);
			Assert::AreEqual(filledConstfooVector1.Back(), Foo(10));

			// First data value sets front and back references
			Assert::AreEqual(intVector.Back(), intVector.Front());
			Assert::AreEqual(doubleVector.Back(), doubleVector.Front());
			Assert::AreEqual(fooVector.Back(), fooVector.Front());
			Assert::AreEqual(filledConstIntVector1.Back(), intVector.Front());
			Assert::AreEqual(filledConstDoubleVector1.Back(), doubleVector.Front());
			Assert::AreEqual(filledConstfooVector1.Back(), fooVector.Front());

			// Non-empty
			Assert::IsFalse(intVector.IsEmpty());
			Assert::IsFalse(doubleVector.IsEmpty());
			Assert::IsFalse(fooVector.IsEmpty());
			Assert::IsFalse(filledConstIntVector1.IsEmpty());
			Assert::IsFalse(filledConstDoubleVector1.IsEmpty());
			Assert::IsFalse(filledConstfooVector1.IsEmpty());

			// Size of 1
			Assert::AreEqual(intVector.Size(), 1_z);
			Assert::AreEqual(doubleVector.Size(), 1_z);
			Assert::AreEqual(fooVector.Size(), 1_z);
			Assert::AreEqual(filledConstIntVector1.Size(), 1_z);
			Assert::AreEqual(filledConstDoubleVector1.Size(), 1_z);
			Assert::AreEqual(filledConstfooVector1.Size(), 1_z);

			intVector.PushBack(20);
			doubleVector.PushBack(20);
			fooVector.PushBack(Foo(20));

			const Vector<int> filledConstIntVector2 = intVector;
			const Vector<double> filledConstDoubleVector2 = doubleVector;
			const Vector<Foo> filledConstfooVector2 = fooVector;

			// Data was set correctly
			Assert::AreEqual(intVector.Front(), 10);
			Assert::AreEqual(doubleVector.Front(), 10.0);
			Assert::AreEqual(fooVector.Front(), Foo(10));
			Assert::AreEqual(filledConstIntVector2.Front(), 10);
			Assert::AreEqual(filledConstDoubleVector2.Front(), 10.0);
			Assert::AreEqual(filledConstfooVector2.Front(), Foo(10));

			// Data was set correctly
			Assert::AreEqual(intVector.Back(), 20);
			Assert::AreEqual(doubleVector.Back(), 20.0);
			Assert::AreEqual(fooVector.Back(), Foo(20));
			Assert::AreEqual(filledConstIntVector2.Back(), 20);
			Assert::AreEqual(filledConstDoubleVector2.Back(), 20.0);
			Assert::AreEqual(filledConstfooVector2.Back(), Foo(20));

			// Size of 2
			Assert::AreEqual(intVector.Size(), 2_z);
			Assert::AreEqual(doubleVector.Size(), 2_z);
			Assert::AreEqual(fooVector.Size(), 2_z);
			Assert::AreEqual(filledConstIntVector2.Size(), 2_z);
			Assert::AreEqual(filledConstDoubleVector2.Size(), 2_z);
			Assert::AreEqual(filledConstfooVector2.Size(), 2_z);

			intVector = Vector<int>(0, Vector<int>::EqualityFunctor(), Vector<int>::ReserveFunctor());
			doubleVector = Vector<double>(0, Vector<double>::EqualityFunctor(), Vector<double>::ReserveFunctor());
			fooVector = Vector<Foo>(0, Vector<Foo>::EqualityFunctor(), Vector<Foo>::ReserveFunctor());

			Assert::ExpectException<std::runtime_error>([&intVector] { intVector.PushBack(10); });
			Assert::ExpectException<std::runtime_error>([&doubleVector] { doubleVector.PushBack(10); });
			Assert::ExpectException<std::runtime_error>([&fooVector] { fooVector.PushBack(Foo(10)); });
		}

		TEST_METHOD(PopBack)
		{
			Vector<int> intVector = { 10, 20, 30 };
			Vector<double> doubleVector = { 10, 20, 30 };
			Vector<Foo> fooVector = { Foo(10), Foo(20), Foo(30) };

			intVector.PopBack();
			doubleVector.PopBack();
			fooVector.PopBack();

			// Back is set correctly with more than 2 elements
			Assert::AreEqual(intVector.Back(), 20);
			Assert::AreEqual(doubleVector.Back(), 20.0);
			Assert::AreEqual(fooVector.Back(), Foo(20));

			// Size is reduced correctly with more than 2 elements
			Assert::AreEqual(intVector.Size(), 2_z);
			Assert::AreEqual(doubleVector.Size(), 2_z);
			Assert::AreEqual(fooVector.Size(), 2_z);

			intVector.PopBack();
			doubleVector.PopBack();
			fooVector.PopBack();

			// Back is set correctly with 1 element remaining
			Assert::AreEqual(intVector.Back(), 10);
			Assert::AreEqual(doubleVector.Back(), 10.0);
			Assert::AreEqual(fooVector.Back(), Foo(10));

			// Size is reduced correctly with 1 element remaining
			Assert::AreEqual(intVector.Size(), 1_z);
			Assert::AreEqual(doubleVector.Size(), 1_z);
			Assert::AreEqual(fooVector.Size(), 1_z);

			// Back is set correctly with 1 element remaining
			Assert::AreEqual(intVector.Back(), intVector.Back());
			Assert::AreEqual(doubleVector.Back(), doubleVector.Back());
			Assert::AreEqual(fooVector.Back(), fooVector.Front());

			intVector.PopBack();
			doubleVector.PopBack();
			fooVector.PopBack();

			// Size is reduced correctly with no elements remaining
			Assert::AreEqual(intVector.Size(), 0_z);
			Assert::AreEqual(doubleVector.Size(), 0_z);
			Assert::AreEqual(fooVector.Size(), 0_z);

			// Now empty
			Assert::IsTrue(intVector.IsEmpty());
			Assert::IsTrue(doubleVector.IsEmpty());
			Assert::IsTrue(fooVector.IsEmpty());

			intVector.PopBack();
			doubleVector.PopBack();
			fooVector.PopBack();

			// Size does not change when called on empty list
			Assert::AreEqual(intVector.Size(), 0_z);
			Assert::AreEqual(doubleVector.Size(), 0_z);
			Assert::AreEqual(fooVector.Size(), 0_z);

			// Emptiness does not change when called on empty list
			Assert::IsTrue(intVector.IsEmpty());
			Assert::IsTrue(doubleVector.IsEmpty());
			Assert::IsTrue(fooVector.IsEmpty());
		}

		TEST_METHOD(Find)
		{
			Vector<int> intVector = { 10, 20, 30 };
			Vector<double> doubleVector = { 10, 20, 30 };
			Vector<Foo> fooVector = { Foo(10), Foo(20), Foo(30) };
			Vector<Bar> barVector = Vector<Bar>({ Bar(10), Bar(20), Bar(30) }, [](Bar a, Bar b) { return a.Data() == b.Data(); });

			const Vector<int> constIntVector = intVector;
			const Vector<double> constDoubleVector = doubleVector;
			const Vector<Foo> constfooVector = fooVector;
			const Vector<Bar> constBarVector = barVector;

			Vector<int>::Iterator intIterator = intVector.begin();
			Vector<double>::Iterator doubleIterator = doubleVector.begin();
			Vector<Foo>::Iterator fooIterator = fooVector.begin();
			Vector<Bar>::Iterator barIterator = barVector.begin();

			Assert::AreEqual(intIterator++, intVector.Find(10));
			Assert::AreEqual(doubleIterator++, doubleVector.Find(10));
			Assert::AreEqual(fooIterator++, fooVector.Find(Foo(10)));
			Assert::AreEqual(barIterator++, barVector.Find(Bar(10)));

			Assert::AreEqual(intIterator++, intVector.Find(20));
			Assert::AreEqual(doubleIterator++, doubleVector.Find(20));
			Assert::AreEqual(fooIterator++, fooVector.Find(Foo(20)));
			Assert::AreEqual(barIterator++, barVector.Find(Bar(20)));

			Assert::AreEqual(intIterator, intVector.Find(30));
			Assert::AreEqual(doubleIterator, doubleVector.Find(30));
			Assert::AreEqual(fooIterator, fooVector.Find(Foo(30)));
			Assert::AreEqual(barIterator, barVector.Find(Bar(30)));

			Assert::AreEqual(intVector.end(), intVector.Find(40));
			Assert::AreEqual(doubleVector.end(), doubleVector.Find(40));
			Assert::AreEqual(fooVector.end(), fooVector.Find(Foo(40)));
			Assert::AreEqual(barVector.end(), barVector.Find(Bar(40)));

			Vector<int>::ConstIterator constIntIterator = constIntVector.begin();
			Vector<double>::ConstIterator constDoubleIterator = constDoubleVector.begin();
			Vector<Foo>::ConstIterator constFooIterator = constfooVector.begin();
			Vector<Bar>::ConstIterator constBarIterator = constBarVector.begin();

			Assert::AreEqual(constIntIterator++, constIntVector.Find(10));
			Assert::AreEqual(constDoubleIterator++, constDoubleVector.Find(10));
			Assert::AreEqual(constFooIterator++, constfooVector.Find(Foo(10)));
			Assert::AreEqual(constBarIterator++, constBarVector.Find(Bar(10)));

			Assert::AreEqual(constIntIterator++, constIntVector.Find(20));
			Assert::AreEqual(constDoubleIterator++, constDoubleVector.Find(20));
			Assert::AreEqual(constFooIterator++, constfooVector.Find(Foo(20)));
			Assert::AreEqual(constBarIterator++, constBarVector.Find(Bar(20)));

			Assert::AreEqual(constIntIterator, constIntVector.Find(30));
			Assert::AreEqual(constDoubleIterator, constDoubleVector.Find(30));
			Assert::AreEqual(constFooIterator, constfooVector.Find(Foo(30)));
			Assert::AreEqual(constBarIterator, constBarVector.Find(Bar(30)));

			Assert::AreEqual(constIntVector.end(), constIntVector.Find(40));
			Assert::AreEqual(constDoubleVector.end(), constDoubleVector.Find(40));
			Assert::AreEqual(constfooVector.end(), constfooVector.Find(Foo(40)));
			Assert::AreEqual(constBarVector.end(), constBarVector.Find(Bar(40)));

			intVector = Vector<int>(0, Vector<int>::EqualityFunctor());
			doubleVector = Vector<double>(0, Vector<double>::EqualityFunctor());
			fooVector = Vector<Foo>(0, Vector<Foo>::EqualityFunctor());

			Assert::ExpectException<std::runtime_error>([&intVector] { intVector.Find(10); });
			Assert::ExpectException<std::runtime_error>([&doubleVector] { doubleVector.Find(10); });
			Assert::ExpectException<std::runtime_error>([&fooVector] { fooVector.Find(Foo(10)); });
		}

		TEST_METHOD(Remove)
		{
			/* Integer Data */

			Vector<int> intVector = { 10, 20 };
			Assert::ExpectException<std::runtime_error>([&intVector] { intVector.Remove(Vector<int>::Iterator()); });

			intVector.Remove(intVector.begin());

			Vector<int>::Iterator intIterator = intVector.begin();
			Assert::AreEqual(*intIterator, 20);

			intVector.PushBack(30);
			intVector.PushBack(40);
			intVector.PushBack(50);
			intVector.Remove(40);
			intVector.Remove(50);

			intIterator = intVector.begin();
			Assert::AreEqual(*intIterator, 20);

			intVector.Remove(intVector.begin());
			intVector.Remove(intVector.begin());

			Assert::IsTrue(intVector.IsEmpty());
			Assert::IsFalse(intVector.Remove(60));


			/* Double Data */

			Vector<double> doubleVector = { 10, 20 };
			Assert::ExpectException<std::runtime_error>([&doubleVector] { doubleVector.Remove(Vector<double>::Iterator()); });

			doubleVector.Remove(doubleVector.begin());

			Vector<double>::Iterator doubleIterator = doubleVector.begin();
			Assert::AreEqual(*doubleIterator, 20.0);

			doubleVector.PushBack(30);
			doubleVector.PushBack(40);
			doubleVector.PushBack(50);
			doubleVector.Remove(40);
			doubleVector.Remove(50);

			doubleIterator = doubleVector.begin();
			Assert::AreEqual(*doubleIterator, 20.0);

			doubleVector.Remove(doubleVector.begin());
			doubleVector.Remove(doubleVector.begin());

			Assert::IsTrue(doubleVector.IsEmpty());
			Assert::IsFalse(doubleVector.Remove(60));


			/* Foo Data */

			Vector<Foo> fooVector = { Foo(10), Foo(20) };
			Assert::ExpectException<std::runtime_error>([&fooVector] { fooVector.Remove(Vector<Foo>::Iterator()); });

			fooVector.Remove(fooVector.begin());

			Vector<Foo>::Iterator fooIterator = fooVector.begin();
			Assert::AreEqual(*fooIterator, Foo(20));

			fooVector.PushBack(Foo(30));
			fooVector.PushBack(Foo(40));
			fooVector.PushBack(Foo(50));
			fooVector.Remove(Foo(40));
			fooVector.Remove(Foo(50));

			fooIterator = fooVector.begin();
			Assert::AreEqual(*fooIterator, Foo(20));

			fooVector.Remove(fooVector.begin());
			fooVector.Remove(fooVector.begin());

			Assert::IsTrue(fooVector.IsEmpty());
			Assert::IsFalse(fooVector.Remove(Foo(60)));

			intVector = Vector<int>(0, Vector<int>::EqualityFunctor());
			doubleVector = Vector<double>(0, Vector<double>::EqualityFunctor());
			fooVector = Vector<Foo>(0, Vector<Foo>::EqualityFunctor());

			Assert::ExpectException<std::runtime_error>([&intVector] { intVector.Remove(10); });
			Assert::ExpectException<std::runtime_error>([&doubleVector] { doubleVector.Remove(10); });
			Assert::ExpectException<std::runtime_error>([&fooVector] { fooVector.Remove(Foo(10)); });
		}

		TEST_METHOD(Clear)
		{
			/* Integer Data */

			Vector<int> intVector;

			// Clearing an empty list does not change emptiness or size
			intVector.Clear();
			Assert::IsTrue(intVector.IsEmpty());
			Assert::AreEqual(intVector.Size(), 0_z);

			intVector = { 10, 20, 30 };

			// Clearing a filled lists empties correctly and sets size to zero
			intVector.Clear();
			Assert::IsTrue(intVector.IsEmpty());
			Assert::AreEqual(intVector.Size(), 0_z);


			/* Double Data */

			Vector<double> doubleVector;

			// Clearing an empty list does not change emptiness or size
			doubleVector.Clear();
			Assert::IsTrue(doubleVector.IsEmpty());
			Assert::AreEqual(doubleVector.Size(), 0_z);

			doubleVector = { 10, 20, 30 };

			// Clearing a filled lists empties correctly and sets size to zero
			doubleVector.Clear();
			Assert::IsTrue(doubleVector.IsEmpty());
			Assert::AreEqual(doubleVector.Size(), 0_z);


			/* Foo Data */

			Vector<Foo> fooVector;

			// Clearing an empty list does not change emptiness or size
			fooVector.Clear();
			Assert::IsTrue(fooVector.IsEmpty());
			Assert::AreEqual(fooVector.Size(), 0_z);

			fooVector = { Foo(10), Foo(20), Foo(30) };

			// Clearing a filled lists empties correctly and sets size to zero
			fooVector.Clear();
			Assert::IsTrue(fooVector.IsEmpty());
			Assert::AreEqual(fooVector.Size(), 0_z);
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState VectorTest::sStartMemState;
}