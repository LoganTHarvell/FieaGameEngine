#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"

#include "SList.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;


namespace UnitTestLibraryDesktop
{
	TEST_CLASS(SListTests)
	{
	public:
		
		TEST_METHOD(SizeAndIsEmpty)
		{
			size_t size = 0;

			SList<int> intList;
			Assert::AreEqual(intList.Size(), size);
			Assert::IsTrue(intList.IsEmpty());

			intList.PushBack(10);
			Assert::AreNotEqual(intList.Size(), size);
			Assert::IsTrue(!intList.IsEmpty());

			SList<double> doubleList;
			Assert::AreEqual(doubleList.Size(), size);
			Assert::IsTrue(doubleList.IsEmpty());

			doubleList.PushBack(10);
			Assert::AreNotEqual(doubleList.Size(), size);
			Assert::IsTrue(!doubleList.IsEmpty());

			SList<Foo> fooList;
			Assert::AreEqual(fooList.Size(), size);
			Assert::IsTrue(fooList.IsEmpty());

			fooList.PushBack(Foo(10));

			Assert::AreNotEqual(fooList.Size(), size);
			Assert::IsTrue(!fooList.IsEmpty());
		}

		TEST_METHOD(PushFront)
		{
			SList<int> intList;
			SList<double> doubleList;
			SList<Foo> fooList;

			bool exception = false;

			try 
			{
				intList.Front();
			}
			catch (std::exception)
			{
				exception = true;
			}

			Assert::IsTrue(exception);
			exception = false;

			try
			{
				doubleList.Front();
			}
			catch (std::exception)
			{
				exception = true;
			}

			Assert::IsTrue(exception);
			exception = false;

			try
			{
				fooList.Front();
			}
			catch (std::exception)
			{
				exception = true;
			}
			
			Assert::IsTrue(exception);
			exception = false;


			Assert::IsTrue(intList.IsEmpty());
			Assert::IsTrue(doubleList.IsEmpty());
			Assert::IsTrue(fooList.IsEmpty());

			intList.PushFront(10);
			doubleList.PushFront(10);
			fooList.PushFront(Foo(10));

			Assert::IsTrue(intList.Front() == 10);
			Assert::IsTrue(doubleList.Front() == 10);
			Assert::IsTrue(fooList.Front() == Foo(10));

			Assert::IsTrue(intList.Front() == intList.Back());
			Assert::IsTrue(doubleList.Front() == doubleList.Back());
			Assert::IsTrue(fooList.Front() == fooList.Back());

			Assert::IsFalse(intList.IsEmpty());
			Assert::IsFalse(doubleList.IsEmpty());
			Assert::IsFalse(fooList.IsEmpty());

			size_t size = 1;
			Assert::AreEqual(intList.Size(), size);
			Assert::AreEqual(doubleList.Size(), size);
			Assert::AreEqual(fooList.Size(), size);

			intList.PushFront(20);
			doubleList.PushFront(20);
			fooList.PushFront(Foo(20));

			Assert::IsTrue(intList.Front() == 20);
			Assert::IsTrue(doubleList.Front() == 20);
			Assert::IsTrue(fooList.Front() == Foo(20));

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

			Assert::IsTrue(intList.Front() == 20);
			Assert::IsTrue(doubleList.Front() == 20);
			Assert::IsTrue(fooList.Front() == Foo(20));

			size_t size = 2;
			Assert::AreEqual(intList.Size(), size);
			Assert::AreEqual(doubleList.Size(), size);
			Assert::AreEqual(fooList.Size(), size);

			intList.PopFront();
			doubleList.PopFront();
			fooList.PopFront();

			Assert::IsTrue(intList.Front() == 10);
			Assert::IsTrue(doubleList.Front() == 10);
			Assert::IsTrue(fooList.Front() == Foo(10));

			size = 1;
			Assert::AreEqual(intList.Size(), size);
			Assert::AreEqual(doubleList.Size(), size);
			Assert::AreEqual(fooList.Size(), size);

			Assert::IsTrue(intList.Front() == intList.Back());
			Assert::IsTrue(doubleList.Front() == doubleList.Back());
			Assert::IsTrue(fooList.Front() == fooList.Back());

			intList.PopFront();
			doubleList.PopFront();
			fooList.PopFront();

			size = 0;
			Assert::AreEqual(intList.Size(), size);
			Assert::AreEqual(doubleList.Size(), size);
			Assert::AreEqual(fooList.Size(), size);

			Assert::IsTrue(intList.IsEmpty());
			Assert::IsTrue(doubleList.IsEmpty());
			Assert::IsTrue(fooList.IsEmpty());

			intList.PopFront();
			doubleList.PopFront();
			fooList.PopFront();

			Assert::IsTrue(intList.IsEmpty());
			Assert::IsTrue(doubleList.IsEmpty());
			Assert::IsTrue(fooList.IsEmpty());
		}

		TEST_METHOD(PushBack)
		{
			SList<int> intList;
			SList<double> doubleList;
			SList<Foo> fooList;

			bool exception = false;

			try
			{
				intList.Back();
			}
			catch (std::exception)
			{
				exception = true;
			}

			Assert::IsTrue(exception);
			exception = false;

			try
			{
				doubleList.Back();
			}
			catch (std::exception)
			{
				exception = true;
			}

			Assert::IsTrue(exception);
			exception = false;

			try
			{
				fooList.Back();
			}
			catch (std::exception)
			{
				exception = true;
			}

			Assert::IsTrue(exception);
			exception = false;


			Assert::IsTrue(intList.IsEmpty());
			Assert::IsTrue(doubleList.IsEmpty());
			Assert::IsTrue(fooList.IsEmpty());

			intList.PushBack(10);
			doubleList.PushBack(10);
			fooList.PushBack(Foo(10));

			Assert::IsTrue(intList.Back() == 10);
			Assert::IsTrue(doubleList.Back() == 10);
			Assert::IsTrue(fooList.Back() == Foo(10));

			Assert::IsTrue(intList.Front() == intList.Back());
			Assert::IsTrue(doubleList.Front() == doubleList.Back());
			Assert::IsTrue(fooList.Front() == fooList.Back());

			Assert::IsFalse(intList.IsEmpty());
			Assert::IsFalse(doubleList.IsEmpty());
			Assert::IsFalse(fooList.IsEmpty());
			
			size_t size = 1;
			Assert::AreEqual(intList.Size(), size);
			Assert::AreEqual(doubleList.Size(), size);
			Assert::AreEqual(fooList.Size(), size);
			
			intList.PushBack(20);
			doubleList.PushBack(20);
			fooList.PushBack(Foo(20));

			Assert::IsTrue(intList.Back() == 20);
			Assert::IsTrue(doubleList.Back() == 20);
			Assert::IsTrue(fooList.Back() == Foo(20));

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

			Assert::IsTrue(intList.Back() == 20);
			Assert::IsTrue(doubleList.Back() == 20);
			Assert::IsTrue(fooList.Back() == Foo(20));

			size_t size = 2;
			Assert::AreEqual(intList.Size(), size);
			Assert::AreEqual(doubleList.Size(), size);
			Assert::AreEqual(fooList.Size(), size);

			intList.PopBack();
			doubleList.PopBack();
			fooList.PopBack();

			Assert::IsTrue(intList.Back() == 10);
			Assert::IsTrue(doubleList.Back() == 10);
			Assert::IsTrue(fooList.Back() == Foo(10));

			size = 1;
			Assert::AreEqual(intList.Size(), size);
			Assert::AreEqual(doubleList.Size(), size);
			Assert::AreEqual(fooList.Size(), size);

			Assert::IsTrue(intList.Front() == intList.Back());
			Assert::IsTrue(doubleList.Front() == doubleList.Back());
			Assert::IsTrue(fooList.Front() == fooList.Back());

			intList.PopBack();
			doubleList.PopBack();
			fooList.PopBack();

			size = 0;
			Assert::AreEqual(intList.Size(), size);
			Assert::AreEqual(doubleList.Size(), size);
			Assert::AreEqual(fooList.Size(), size);

			Assert::IsTrue(intList.IsEmpty());
			Assert::IsTrue(doubleList.IsEmpty());
			Assert::IsTrue(fooList.IsEmpty());

			intList.PopBack();
			doubleList.PopBack();
			fooList.PopBack();

			Assert::IsTrue(intList.IsEmpty());
			Assert::IsTrue(doubleList.IsEmpty());
			Assert::IsTrue(fooList.IsEmpty());
		}

		TEST_METHOD(Clear)
		{
			SList<int> intList;
			intList.Clear();
			Assert::IsTrue(intList.IsEmpty());

			size_t zeroSize = 0;
			Assert::AreEqual(intList.Size(), zeroSize);

			intList.PushBack(10);
			intList.PushBack(20);
			intList.PushBack(30);

			intList.Clear();
			Assert::IsTrue(intList.IsEmpty());
			Assert::AreEqual(intList.Size(), zeroSize);

			SList<double> doubleList;
			doubleList.Clear();
			Assert::IsTrue(doubleList.IsEmpty());

			Assert::AreEqual(doubleList.Size(), zeroSize);

			doubleList.PushBack(10);
			doubleList.PushBack(20);
			doubleList.PushBack(30);

			doubleList.Clear();
			Assert::IsTrue(doubleList.IsEmpty());
			Assert::AreEqual(doubleList.Size(), zeroSize);

			SList<Foo> fooList;
			fooList.Clear();
			Assert::IsTrue(fooList.IsEmpty());

			Assert::AreEqual(fooList.Size(), zeroSize);

			fooList.PushBack(Foo(10));
			fooList.PushBack(Foo(20));
			fooList.PushBack(Foo(30));

			fooList.Clear();
			Assert::IsTrue(fooList.IsEmpty());
			Assert::AreEqual(fooList.Size(), zeroSize);
		}

		TEST_METHOD(Copy)
		{
			SList<int> intList;
			intList.PushBack(10);
			intList.PushBack(20);
			intList.PushBack(30);

			SList<double> doubleList;
			doubleList.PushBack(10);
			doubleList.PushBack(20);
			doubleList.PushBack(30);

			SList<Foo> fooList;
			fooList.PushBack(Foo(10));
			fooList.PushBack(Foo(20));
			fooList.PushBack(Foo(30));

			{
				SList<int> listCopy(intList);
				Assert::IsTrue(listCopy == intList);
			}

			{
				SList<double> listCopy(doubleList);
				Assert::IsTrue(listCopy == doubleList);
			}

			{
				SList<Foo> listCopy(fooList);
				Assert::IsTrue(listCopy == fooList);
			}

			{
				SList<int> listCopy;
				listCopy.PushBack(40);

				Assert::IsFalse(listCopy == intList);

				listCopy.PushBack(50);
				listCopy.PushBack(60);

				Assert::IsFalse(listCopy == intList);
				
				listCopy.Clear();

				listCopy = intList;
				Assert::IsTrue(listCopy == intList);
			}

			{
				SList<double> listCopy;
				listCopy.PushBack(40);

				Assert::IsFalse(listCopy == doubleList);

				listCopy.PushBack(50);
				listCopy.PushBack(60);

				Assert::IsFalse(listCopy == doubleList);

				listCopy.Clear();

				listCopy = doubleList;
				Assert::IsTrue(listCopy == doubleList);
			}

			{
				SList<Foo> listCopy;
				listCopy.PushBack(Foo(40));
				Assert::IsFalse(listCopy == fooList);

				fooList.PushBack(Foo(50));
				fooList.PushBack(Foo(60));

				Assert::IsFalse(listCopy == fooList);

				listCopy.Clear();

				listCopy = fooList;
				Assert::IsTrue(listCopy == fooList);
			}
		}
	};
}
