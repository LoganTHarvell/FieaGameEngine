#include "pch.h"

#include "ToStringSpecialization.h"
#include "AttributedFoo.h"
#include "AttributedBar.h"


using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;


namespace ReflectionSystemTests
{
	TEST_CLASS(AttributedBarTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			TypeManager::Create();
			RegisterType<AttributedBar>();

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

			TypeManager::Destroy();
		}

		TEST_METHOD(Constructor)
		{
			AttributedBar a;
			Assert::AreEqual(0, a.Data());

			AttributedBar b(10);
			Assert::AreEqual(10, b.Data());

			AttributedBar c = AttributedBar(20);
		}

		TEST_METHOD(CopySemantics)
		{
			AttributedBar a(10);
			Assert::AreEqual(10, a.Data());

			AttributedBar b(a);
			Assert::AreEqual(a.Data(), b.Data());

			AttributedBar c;
			c = a;
			Assert::AreEqual(c.Data(), a.Data());

			AttributedBar d(a);
			Assert::AreEqual(d.Data(), a.Data());
		}

		TEST_METHOD(EqualityOperators)
		{
			AttributedBar a(10);
			AttributedBar b(a);
			Assert::AreEqual(a.Data(), b.Data());

			AttributedBar c;
			Assert::IsTrue(c.Data() != a.Data()); // Assert::AreNotEqual does not invoke operator!=
		}

		TEST_METHOD(SetData)
		{
			AttributedBar a;
			Assert::AreEqual(0, a.Data());

			const int data = 10;
			a.SetData(data);
			Assert::AreEqual(data, a.Data());
		}

		TEST_METHOD(MoveSemantics)
		{
			const int data = 10;
			AttributedBar a(data);
			Assert::AreEqual(data, a.Data());

			AttributedBar b(std::move(a));
			Assert::AreEqual(data, b.Data());

			AttributedBar c;
			Assert::AreEqual(0, c.Data());
			c = std::move(b);
			Assert::AreEqual(data, c.Data());
		}

		TEST_METHOD(RTTITest)
		{
			AttributedBar a = AttributedBar(10);
			AttributedBar b = AttributedBar(10);

			Assert::IsTrue(a.Is(AttributedBar::TypeIdClass()));
			Assert::IsTrue(a.Equals(&b));
		}
		
		TEST_METHOD(ToString)
		{
			Assert::AreEqual("AttributedBar: 0", AttributedBar().ToString().c_str());
		}

		TEST_METHOD(Clone)
		{
			Attributed* scope = new AttributedBar(10);
			Scope* tmp = scope->Clone();
			Assert::AreEqual(10, tmp->Find("integer")->Get<int>());
			delete scope;
			delete tmp;
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState AttributedBarTest::sStartMemState;
}