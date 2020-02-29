#include "pch.h"

#include "ToStringSpecialization.h"
#include "JsonParseMaster.h"
#include "JsonParseTestHelper.h"


using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(JsonParseTest)
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

		TEST_METHOD(Parse)
		{
			JsonParseTestHelper::SharedData sharedData;
			JsonParseTestHelper helper;
			JsonParseMaster parser;

			parser.AddHelper(helper);
			parser.SetSharedData(sharedData);

			parser.Parse(R"({ "mInt": 10, "mFloat": 10, "mString": "10", "mArray": [ 1, 2, 3 ] })");

			Assert::AreEqual(10, sharedData.GetInt());
			Assert::AreEqual(10.0f, sharedData.GetFloat());
			Assert::AreEqual("10"s, sharedData.GetString());
			
// 			gsl::span array = sharedData.GetArray();
// 			for (ptrdiff_t i = 0; i < array.size(); ++i)
// 			{
// 				Assert::IsTrue(i == array[i]);
// 			}
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState JsonParseTest::sStartMemState;
}