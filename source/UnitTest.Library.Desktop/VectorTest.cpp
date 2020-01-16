#include "pch.h"

#include "ToStringSpecialization.h"
#include "Foo.h"
#include "Bar.h"
#include "Vector.h"


using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;
using namespace Library;

namespace UnitTestLibraryDesktop
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

			intVector.PushBack(10);
			doubleVector.PushBack(10);
			fooVector.PushBack(Foo(10));

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
			Vector<int> intVector;
			Vector<double> doubleVector;
			Vector<Foo> fooVector;

			Assert::ExpectException<std::runtime_error>([&intVector] { *(intVector.begin()); });
			Assert::ExpectException<std::runtime_error>([&doubleVector] { *(doubleVector.begin()); });
			Assert::ExpectException<std::runtime_error>([&fooVector] { *(fooVector.begin()); });

			Assert::ExpectException<std::runtime_error>([&intVector] { *(intVector.cbegin()); });
			Assert::ExpectException<std::runtime_error>([&doubleVector] { *(doubleVector.cbegin()); });
			Assert::ExpectException<std::runtime_error>([&fooVector] { *(fooVector.cbegin()); });

			intVector.PushBack(10);
			doubleVector.PushBack(10);
			fooVector.PushBack(Foo(10));

			Assert::AreEqual(*intVector.begin(), 10);
			Assert::AreEqual(*doubleVector.begin(), 10.0);
			Assert::AreEqual(*fooVector.begin(), Foo(10));

			Assert::AreEqual(*intVector.cbegin(), 10);
			Assert::AreEqual(*doubleVector.cbegin(), 10.0);
			Assert::AreEqual(*fooVector.cbegin(), Foo(10));
		}

		TEST_METHOD(IteratorEquality)
		{
			Vector<int> intVector;
			Vector<double> doubleVector;
			Vector<Foo> fooVector;

			Assert::AreEqual(intVector.begin(), intVector.end());
			Assert::AreEqual(doubleVector.begin(), doubleVector.end());
			Assert::AreEqual(fooVector.begin(), fooVector.end());

			Assert::AreEqual(intVector.cbegin(), intVector.cend());
			Assert::AreEqual(doubleVector.cbegin(), doubleVector.cend());
			Assert::AreEqual(fooVector.cbegin(), fooVector.cend());

			intVector.PushBack(10);
			doubleVector.PushBack(10);
			fooVector.PushBack(Foo(10));

			intVector.PushBack(10);
			doubleVector.PushBack(10);
			fooVector.PushBack(Foo(10));

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
			Vector<int> intVector = { 10, 20, 30 };
			Vector<double> doubleVector = { 10, 20, 30 };
			Vector<Foo> fooVector = { Foo(10), Foo(20), Foo(30) };

			Vector<int>::Iterator intIterator = intVector.begin();
			Vector<double>::Iterator doubleIterator = doubleVector.begin();
			Vector<Foo>::Iterator fooIterator = fooVector.begin();

			Vector<int>::ConstIterator intConstIterator = intVector.cbegin();
			Vector<double>::ConstIterator doubleConstIterator = doubleVector.cbegin();
			Vector<Foo>::ConstIterator fooConstIterator = fooVector.cbegin();

			Assert::AreEqual(*intIterator, 10);
			Assert::AreEqual(*doubleIterator, 10.0);
			Assert::AreEqual(*fooIterator, Foo(10));

			Assert::AreEqual(*intConstIterator, 10);
			Assert::AreEqual(*doubleConstIterator, 10.0);
			Assert::AreEqual(*fooConstIterator, Foo(10));

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

			Assert::ExpectException<std::runtime_error>([&intIterator] { intIterator + 5; });
			Assert::ExpectException<std::runtime_error>([&doubleIterator] { doubleIterator + 5; });
			Assert::ExpectException<std::runtime_error>([&fooIterator] { fooIterator + 5; });

			Assert::ExpectException<std::runtime_error>([&intConstIterator] { intConstIterator + 5; });
			Assert::ExpectException<std::runtime_error>([&doubleConstIterator] { doubleConstIterator + 5; });
			Assert::ExpectException<std::runtime_error>([&fooConstIterator] { fooConstIterator + 5; });

			Assert::ExpectException<std::runtime_error>([&intIterator] { intIterator - 5; });
			Assert::ExpectException<std::runtime_error>([&doubleIterator] { doubleIterator - 5; });
			Assert::ExpectException<std::runtime_error>([&fooIterator] { fooIterator - 5; });

			Assert::ExpectException<std::runtime_error>([&intConstIterator] { intConstIterator - 5; });
			Assert::ExpectException<std::runtime_error>([&doubleConstIterator] { doubleConstIterator - 5; });
			Assert::ExpectException<std::runtime_error>([&fooConstIterator] { fooConstIterator - 5; });
		}

		TEST_METHOD(VectorInitialization)
		{
			Vector<int> intVectorEmpty;
			Vector<double> doubleVectorEmpty;
			Vector<Foo> fooVectorEmpty;

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
			const Vector<Foo> emptyConstFooVector = fooVector1;
			Assert::AreEqual(emptyConstFooVector, fooVector2);

			// Inequality when filled and different sizes
			fooVector1.PushBack(Foo(10));
			fooVector1.PushBack(Foo(20));
			fooVector2.PushBack(Foo(10));

			Assert::IsTrue(fooVector1 != fooVector2);
			const Vector<Foo> filledConstFooVector1 = fooVector1;
			Assert::IsTrue(filledConstFooVector1 != fooVector2);

			// Equality when filled
			fooVector2.PushBack(Foo(20));
			Assert::AreEqual(fooVector1, fooVector2);
			const Vector<Foo> filledConstFooVector2 = fooVector1;
			Assert::AreEqual(filledConstFooVector2, fooVector2);

			// Inequality when filled and equal size
			fooVector2.PopBack();
			fooVector2.PushBack(Foo(30));
			Assert::IsTrue(fooVector1 != fooVector2);
			Assert::IsTrue(filledConstFooVector2 != fooVector2);
		}

		TEST_METHOD(Size)
		{
			/* Integer Data */

			// Size is zero
			Vector<int> intVector;
			Assert::AreEqual(intVector.Size(), 0_z);

			const Vector<int> emptyConstIntVector = intVector;
			Assert::AreEqual(emptyConstIntVector.Size(), 0_z);

			// Size increased
			intVector.PushBack(10);
			Assert::AreEqual(intVector.Size(), 1_z);

			const Vector<int> filledConstIntVector = intVector;
			Assert::AreEqual(filledConstIntVector.Size(), 1_z);

			// Size decreased
			intVector.PopBack();
			Assert::AreEqual(intVector.Size(), 0_z);
			const Vector<int> decreasedConstIntVector = intVector;
			Assert::AreEqual(decreasedConstIntVector.Size(), 0_z);


			/* Double Data */

			// Size is zero
			Vector<double> doubleVector;
			Assert::AreEqual(doubleVector.Size(), 0_z);

			const Vector<double> emptyConstDoubleVector = doubleVector;
			Assert::AreEqual(emptyConstDoubleVector.Size(), 0_z);

			// Size increased
			doubleVector.PushBack(10);
			Assert::AreEqual(doubleVector.Size(), 1_z);

			const Vector<double> filledConstDoubleVector = doubleVector;
			Assert::AreEqual(filledConstDoubleVector.Size(), 1_z);

			// Size decreased
			doubleVector.PopBack();
			Assert::AreEqual(doubleVector.Size(), 0_z);
			const Vector<double> decreasedConstDoubleVector = doubleVector;
			Assert::AreEqual(decreasedConstDoubleVector.Size(), 0_z);


			/* Foo Data */

			// Size is zero
			Vector<Foo> fooVector;
			Assert::AreEqual(fooVector.Size(), 0_z);
			const Vector<Foo> emptyConstFooVector = fooVector;
			Assert::AreEqual(emptyConstFooVector.Size(), 0_z);

			// Size increased
			fooVector.PushBack(Foo(10));
			Assert::AreEqual(fooVector.Size(), 1_z);
			const Vector<Foo> filledConstFooVector = fooVector;
			Assert::AreEqual(filledConstFooVector.Size(), 1_z);

			// Size decreased
			fooVector.PopBack();
			Assert::AreEqual(fooVector.Size(), 0_z);
			const Vector<Foo> decreasedConstFooVector = fooVector;
			Assert::AreEqual(decreasedConstFooVector.Size(), 0_z);
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

			const Vector<Foo> emptyConstFooVector = fooVector;
			Assert::IsTrue(emptyConstFooVector.IsEmpty());

			// Non-empty list and size
			fooVector.PushBack(Foo(10));
			Assert::IsTrue(!fooVector.IsEmpty());

			const Vector<Foo> filledConstFooVector = fooVector;
			Assert::IsTrue(!filledConstFooVector.IsEmpty());
		}

		TEST_METHOD(Begin)
		{
			Vector<int> intVector;
			Vector<double> doubleVector;
			Vector<Foo> fooVector;

			Assert::ExpectException<std::runtime_error>([&intVector] { *(intVector.begin()); });
			Assert::ExpectException<std::runtime_error>([&doubleVector] { *(doubleVector.begin()); });
			Assert::ExpectException<std::runtime_error>([&fooVector] { *(fooVector.begin()); });

			const Vector<int> emptyIntVectorCopy = intVector;
			const Vector<double> emptyDoubleVectorCopy = doubleVector;
			const Vector<Foo> emptyFooVectorCopy = fooVector;

			Assert::ExpectException<std::runtime_error>([&emptyIntVectorCopy] { *(emptyIntVectorCopy.begin()); });
			Assert::ExpectException<std::runtime_error>([&emptyDoubleVectorCopy] { *(emptyDoubleVectorCopy.begin()); });
			Assert::ExpectException<std::runtime_error>([&emptyFooVectorCopy] { *(emptyFooVectorCopy.begin()); });

			Assert::ExpectException<std::runtime_error>([&intVector] { *(intVector.cbegin()); });
			Assert::ExpectException<std::runtime_error>([&doubleVector] { *(doubleVector.cbegin()); });
			Assert::ExpectException<std::runtime_error>([&fooVector] { *(fooVector.cbegin()); });

			intVector.PushBack(10);
			doubleVector.PushBack(10);
			fooVector.PushBack(Foo(10));

			Assert::AreEqual(*intVector.begin(), 10);
			Assert::AreEqual(*doubleVector.begin(), 10.0);
			Assert::AreEqual(*fooVector.begin(), Foo(10));

			const Vector<int> filledIntVectorCopy = intVector;
			const Vector<double> filledDoubleVectorCopy = doubleVector;
			const Vector<Foo> filledFooVectorCopy = fooVector;

			Assert::AreEqual(*filledIntVectorCopy.begin(), 10);
			Assert::AreEqual(*filledDoubleVectorCopy.begin(), 10.0);
			Assert::AreEqual(*filledFooVectorCopy.begin(), Foo(10));

			Assert::AreEqual(*intVector.cbegin(), 10);
			Assert::AreEqual(*doubleVector.cbegin(), 10.0);
			Assert::AreEqual(*fooVector.cbegin(), Foo(10));
		}

		TEST_METHOD(End)
		{
			Vector<int> intVector;
			Vector<double> doubleVector;
			Vector<Foo> fooVector;

			Assert::ExpectException<std::runtime_error>([&intVector] { *(intVector.end()); });
			Assert::ExpectException<std::runtime_error>([&doubleVector] { *(doubleVector.end()); });
			Assert::ExpectException<std::runtime_error>([&fooVector] { *(fooVector.end()); });

			const Vector<int> emptyIntVectorCopy = intVector;
			const Vector<double> emptyDoubleVectorCopy = doubleVector;
			const Vector<Foo> emptyFooVectorCopy = fooVector;

			Assert::ExpectException<std::runtime_error>([&emptyIntVectorCopy] { *(emptyIntVectorCopy.end()); });
			Assert::ExpectException<std::runtime_error>([&emptyDoubleVectorCopy] { *(emptyDoubleVectorCopy.end()); });
			Assert::ExpectException<std::runtime_error>([&emptyFooVectorCopy] { *(emptyFooVectorCopy.end()); });

			Assert::ExpectException<std::runtime_error>([&intVector] { *(intVector.cend()); });
			Assert::ExpectException<std::runtime_error>([&doubleVector] { *(doubleVector.cend()); });
			Assert::ExpectException<std::runtime_error>([&fooVector] { *(fooVector.cend()); });

			intVector.PushBack(10);
			doubleVector.PushBack(10);
			fooVector.PushBack(Foo(10));

			Assert::ExpectException<std::runtime_error>([&intVector] { *(intVector.end()); });
			Assert::ExpectException<std::runtime_error>([&intVector] { *(intVector.end()); });
			Assert::ExpectException<std::runtime_error>([&intVector] { *(intVector.end()); });

			const Vector<int> filledIntVectorCopy = intVector;
			const Vector<double> filledDoubleVectorCopy = doubleVector;
			const Vector<Foo> filledFooVectorCopy = fooVector;

			Assert::ExpectException<std::runtime_error>([&filledIntVectorCopy] { *(filledIntVectorCopy.end()); });
			Assert::ExpectException<std::runtime_error>([&filledDoubleVectorCopy] { *(filledDoubleVectorCopy.end()); });
			Assert::ExpectException<std::runtime_error>([&filledFooVectorCopy] { *(filledFooVectorCopy.end()); });

			Assert::ExpectException<std::runtime_error>([&intVector] { *(intVector.cend()); });
			Assert::ExpectException<std::runtime_error>([&doubleVector] { *(doubleVector.cend()); });
			Assert::ExpectException<std::runtime_error>([&fooVector] { *(fooVector.cend()); });

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

		TEST_METHOD(PushBack)
		{
			Vector<int> intVector;
			Vector<double> doubleVector;
			Vector<Foo> fooVector;

			const Vector<int> emptyConstIntVector;
			const Vector<double> emptyConstDoubleVector;
			const Vector<Foo> emptyConstFooVector;

			// Empty list exception when calling Front
			Assert::ExpectException<std::runtime_error>([&intVector] { intVector.Front(); });
			Assert::ExpectException<std::runtime_error>([&doubleVector] { doubleVector.Front(); });
			Assert::ExpectException<std::runtime_error>([&fooVector] { fooVector.Front(); });
			Assert::ExpectException<std::runtime_error>([&emptyConstIntVector] { emptyConstIntVector.Front(); });
			Assert::ExpectException<std::runtime_error>([&emptyConstDoubleVector] { emptyConstDoubleVector.Front(); });
			Assert::ExpectException<std::runtime_error>([&emptyConstFooVector] { emptyConstFooVector.Front(); });

			// Empty list exception when calling Back
			Assert::ExpectException<std::runtime_error>([&intVector] { intVector.Back(); });
			Assert::ExpectException<std::runtime_error>([&doubleVector] { doubleVector.Back(); });
			Assert::ExpectException<std::runtime_error>([&fooVector] { fooVector.Back(); });
			Assert::ExpectException<std::runtime_error>([&emptyConstIntVector] { emptyConstIntVector.Back(); });
			Assert::ExpectException<std::runtime_error>([&emptyConstDoubleVector] { emptyConstDoubleVector.Back(); });
			Assert::ExpectException<std::runtime_error>([&emptyConstFooVector] { emptyConstFooVector.Back(); });

			intVector.PushBack(10);
			doubleVector.PushBack(10);
			fooVector.PushBack(Foo(10));

			const Vector<int> filledConstIntVector1 = intVector;
			const Vector<double> filledConstDoubleVector1 = doubleVector;
			const Vector<Foo> filledConstFooVector1 = fooVector;

			// Data was set correctly
			Assert::AreEqual(intVector.Back(), 10);
			Assert::AreEqual(doubleVector.Back(), 10.0);
			Assert::AreEqual(fooVector.Back(), Foo(10));
			Assert::AreEqual(filledConstIntVector1.Back(), 10);
			Assert::AreEqual(filledConstDoubleVector1.Back(), 10.0);
			Assert::AreEqual(filledConstFooVector1.Back(), Foo(10));

			// First data value sets front and back references
			Assert::AreEqual(intVector.Back(), intVector.Front());
			Assert::AreEqual(doubleVector.Back(), doubleVector.Front());
			Assert::AreEqual(fooVector.Back(), fooVector.Front());
			Assert::AreEqual(filledConstIntVector1.Back(), intVector.Front());
			Assert::AreEqual(filledConstDoubleVector1.Back(), doubleVector.Front());
			Assert::AreEqual(filledConstFooVector1.Back(), fooVector.Front());

			// Non-empty
			Assert::IsFalse(intVector.IsEmpty());
			Assert::IsFalse(doubleVector.IsEmpty());
			Assert::IsFalse(fooVector.IsEmpty());
			Assert::IsFalse(filledConstIntVector1.IsEmpty());
			Assert::IsFalse(filledConstDoubleVector1.IsEmpty());
			Assert::IsFalse(filledConstFooVector1.IsEmpty());

			// Size of 1
			Assert::AreEqual(intVector.Size(), 1_z);
			Assert::AreEqual(doubleVector.Size(), 1_z);
			Assert::AreEqual(fooVector.Size(), 1_z);
			Assert::AreEqual(filledConstIntVector1.Size(), 1_z);
			Assert::AreEqual(filledConstDoubleVector1.Size(), 1_z);
			Assert::AreEqual(filledConstFooVector1.Size(), 1_z);

			intVector.PushBack(20);
			doubleVector.PushBack(20);
			fooVector.PushBack(Foo(20));

			const Vector<int> filledConstIntVector2 = intVector;
			const Vector<double> filledConstDoubleVector2 = doubleVector;
			const Vector<Foo> filledConstFooVector2 = fooVector;

			// Data was set correctly
			Assert::AreEqual(intVector.Front(), 10);
			Assert::AreEqual(doubleVector.Front(), 10.0);
			Assert::AreEqual(fooVector.Front(), Foo(10));
			Assert::AreEqual(filledConstIntVector2.Front(), 10);
			Assert::AreEqual(filledConstDoubleVector2.Front(), 10.0);
			Assert::AreEqual(filledConstFooVector2.Front(), Foo(10));

			// Data was set correctly
			Assert::AreEqual(intVector.Back(), 20);
			Assert::AreEqual(doubleVector.Back(), 20.0);
			Assert::AreEqual(fooVector.Back(), Foo(20));
			Assert::AreEqual(filledConstIntVector2.Back(), 20);
			Assert::AreEqual(filledConstDoubleVector2.Back(), 20.0);
			Assert::AreEqual(filledConstFooVector2.Back(), Foo(20));

			// Size of 2
			Assert::AreEqual(intVector.Size(), 2_z);
			Assert::AreEqual(doubleVector.Size(), 2_z);
			Assert::AreEqual(fooVector.Size(), 2_z);
			Assert::AreEqual(filledConstIntVector2.Size(), 2_z);
			Assert::AreEqual(filledConstDoubleVector2.Size(), 2_z);
			Assert::AreEqual(filledConstFooVector2.Size(), 2_z);
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
			Vector<int> intVector;
			Vector<double> doubleVector;
			Vector<Foo> fooVector;
			Vector<Bar> barVector;

			std::function<bool(Bar a, Bar b)> barEqual = [](Bar a, Bar b) { return a.Data() == b.Data(); };

			intVector.PushBack(10);
			doubleVector.PushBack(10);
			fooVector.PushBack(Foo(10));
			barVector.PushBack(Bar(10));

			intVector.PushBack(20);
			doubleVector.PushBack(20);
			fooVector.PushBack(Foo(20));
			barVector.PushBack(Bar(20));

			intVector.PushBack(30);
			doubleVector.PushBack(30);
			fooVector.PushBack(Foo(30));
			barVector.PushBack(Bar(30));


			Vector<int>::Iterator intIterator = intVector.begin();
			Vector<double>::Iterator doubleIterator = doubleVector.begin();
			Vector<Foo>::Iterator fooIterator = fooVector.begin();
			Vector<Bar>::Iterator barIterator = barVector.begin();

			Assert::AreEqual(intIterator++, intVector.Find(10));
			Assert::AreEqual(doubleIterator++, doubleVector.Find(10));
			Assert::AreEqual(fooIterator++, fooVector.Find(Foo(10)));
			Assert::AreEqual(barIterator++, barVector.Find(Bar(10), barEqual));

			Assert::AreEqual(intIterator++, intVector.Find(20));
			Assert::AreEqual(doubleIterator++, doubleVector.Find(20));
			Assert::AreEqual(fooIterator++, fooVector.Find(Foo(20)));
			Assert::AreEqual(barIterator++, barVector.Find(Bar(20), barEqual));

			Assert::AreEqual(intIterator, intVector.Find(30));
			Assert::AreEqual(doubleIterator, doubleVector.Find(30));
			Assert::AreEqual(fooIterator, fooVector.Find(Foo(30)));
			Assert::AreEqual(barIterator, barVector.Find(Bar(30), barEqual));

			Assert::AreEqual(intVector.end(), intVector.Find(40));
			Assert::AreEqual(doubleVector.end(), doubleVector.Find(40));
			Assert::AreEqual(fooVector.end(), fooVector.Find(Foo(40)));
			Assert::AreEqual(barVector.end(), barVector.Find(Bar(40), barEqual));

			const Vector<int> constIntVector = intVector;
			const Vector<double> constDoubleVector = doubleVector;
			const Vector<Foo> constFooVector = fooVector;
			const Vector<Bar> constBarVector = barVector;

			Vector<int>::ConstIterator constIntIterator = constIntVector.begin();
			Vector<double>::ConstIterator constDoubleIterator = constDoubleVector.begin();
			Vector<Foo>::ConstIterator constFooIterator = constFooVector.begin();
			Vector<Bar>::ConstIterator constBarIterator = constBarVector.begin();

			Assert::AreEqual(constIntIterator++, constIntVector.Find(10));
			Assert::AreEqual(constDoubleIterator++, constDoubleVector.Find(10));
			Assert::AreEqual(constFooIterator++, constFooVector.Find(Foo(10)));
			Assert::AreEqual(constBarIterator++, constBarVector.Find(Bar(10), barEqual));

			Assert::AreEqual(constIntIterator++, constIntVector.Find(20));
			Assert::AreEqual(constDoubleIterator++, constDoubleVector.Find(20));
			Assert::AreEqual(constFooIterator++, constFooVector.Find(Foo(20)));
			Assert::AreEqual(constBarIterator++, constBarVector.Find(Bar(20), barEqual));

			Assert::AreEqual(constIntIterator, constIntVector.Find(30));
			Assert::AreEqual(constDoubleIterator, constDoubleVector.Find(30));
			Assert::AreEqual(constFooIterator, constFooVector.Find(Foo(30)));
			Assert::AreEqual(constBarIterator, constBarVector.Find(Bar(30), barEqual));

			Assert::AreEqual(constIntVector.end(), constIntVector.Find(40));
			Assert::AreEqual(constDoubleVector.end(), constDoubleVector.Find(40));
			Assert::AreEqual(constFooVector.end(), constFooVector.Find(Foo(40)));
			Assert::AreEqual(constBarVector.end(), constBarVector.Find(Bar(40), barEqual));
		}

		TEST_METHOD(Remove)
		{
			/* Integer Data */

			Vector<int> intVector;

			Assert::ExpectException<std::runtime_error>([&intVector] { intVector.Remove(Vector<int>::Iterator()); });

			intVector.PushBack(10);
			intVector.PushBack(20);
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


			/* Double Data */

			Vector<double> doubleVector;

			Assert::ExpectException<std::runtime_error>([&doubleVector] { doubleVector.Remove(Vector<double>::Iterator()); });

			doubleVector.PushBack(10);
			doubleVector.PushBack(20);
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


			/* Foo Data */

			Vector<Foo> fooVector;

			Assert::ExpectException<std::runtime_error>([&fooVector] { fooVector.Remove(Vector<Foo>::Iterator()); });

			fooVector.PushBack(Foo(10));
			fooVector.PushBack(Foo(20));
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