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

			intVectorFilled.Clear();
			doubleVectorFilled.Clear();
			fooVectorFilled.Clear();

			Assert::AreEqual(intVectorEmpty.Size(), 0_z);
			Assert::AreEqual(doubleVectorEmpty.Size(), 0_z);
			Assert::AreEqual(fooVectorEmpty.Size(), 0_z);
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

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState VectorTest::sStartMemState;
}