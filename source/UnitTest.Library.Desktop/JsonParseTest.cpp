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

		TEST_METHOD(Move)
		{
			JsonParseTestHelper::SharedData sharedData;
			JsonParseTestHelper helper;
			JsonParseMaster parser;

			parser.AddHelper(helper);
			parser.SetSharedData(sharedData);

			parser.Parse(R"({ "mTypo": 10, "mFloat": 10, "mString": "10", "mTypo": [ 1, 2, 3 ] })");

			JsonParseMaster moveConstructed(std::move(parser));
			Assert::IsNull(parser.GetSharedData());

			{
				const JsonParseMaster::SharedData* sharedDataPtr = &sharedData;
				Assert::AreEqual(sharedDataPtr, moveConstructed.GetSharedData());
			}

			moveConstructed.Parse(R"({ "mInt": 10, "mFloat": 10, "mString": "10", "mArray": [ 1, 2, 3 ] })");

			Assert::AreEqual(10, sharedData.GetInt());
			Assert::AreEqual(10.0f, sharedData.GetFloat());
			Assert::AreEqual("10"s, sharedData.GetString());

			gsl::span array = sharedData.GetArray();
			for (ptrdiff_t i = 0; i < array.size(); ++i)
			{
				Assert::AreEqual(static_cast<int>(i + 1), array[i]);
			}

			JsonParseMaster moveAssigned;
			moveAssigned = std::move(moveConstructed);
			Assert::IsNull(parser.GetSharedData());
			Assert::IsNull(moveConstructed.GetSharedData());

			{
				const JsonParseMaster::SharedData* sharedDataPtr = &sharedData;
				Assert::AreEqual(sharedDataPtr, moveAssigned.GetSharedData());
			}

			moveAssigned.Parse(R"({ "mInt": 10, "mFloat": 10, "mString": "10", "mArray": [ 1, 2, 3 ] })");

			Assert::AreEqual(10, sharedData.GetInt());
			Assert::AreEqual(10.0f, sharedData.GetFloat());
			Assert::AreEqual("10"s, sharedData.GetString());

			array = sharedData.GetArray();
			for (ptrdiff_t i = 0; i < array.size(); ++i)
			{
				Assert::AreEqual(static_cast<int>(i + 1), array[i]);
			}

			JsonParseTestHelper::SharedData moveConstructedSharedData(std::move(sharedData));
			{
				const JsonParseMaster::SharedData* sharedDataPtr = &moveConstructedSharedData;
				Assert::AreEqual(sharedDataPtr, moveAssigned.GetSharedData());
			}

			JsonParseTestHelper::SharedData moveAssignedSharedData;
			moveAssignedSharedData = std::move(moveConstructedSharedData);
			{
				const JsonParseMaster::SharedData* sharedDataPtr = &moveAssignedSharedData;
				Assert::AreEqual(sharedDataPtr, moveAssigned.GetSharedData());
			}
		}

		TEST_METHOD(Accessors)
		{
			JsonParseTestHelper::SharedData sharedData;

			Assert::AreEqual(0, sharedData.GetInt());
			Assert::AreEqual(0.0f, sharedData.GetFloat());
			Assert::AreEqual(std::string(), sharedData.GetString());

			gsl::span array = sharedData.GetArray();
			for (ptrdiff_t i = 0; i < array.size(); ++i)
			{
				Assert::AreEqual(0, array[i]);
			}

			JsonParseTestHelper helper;
			JsonParseMaster parser;

			Assert::IsNull(sharedData.GetJsonParseMaster());
			Assert::IsNull(parser.GetSharedData());
			parser.SetSharedData(sharedData);
			Assert::IsNotNull(sharedData.GetJsonParseMaster());
			Assert::IsNotNull(parser.GetSharedData());

			{
				const JsonParseTestHelper::SharedData constSharedData;
				Assert::IsNull(constSharedData.GetJsonParseMaster());
			}
			{
				const JsonParseTestHelper::SharedData constSharedData = sharedData;
				Assert::AreEqual(sharedData.GetJsonParseMaster(), constSharedData.GetJsonParseMaster());
			}

			const JsonParseMaster constParser;
			Assert::IsNull(constParser.GetSharedData());
		}

		TEST_METHOD(Modifiers)
		{
			JsonParseTestHelper::SharedData sharedData;
			JsonParseTestHelper helper;
			JsonParseMaster parser;

			parser.SetSharedData(sharedData);

			parser.Parse(R"({ "mInt": 10, "mFloat": 10, "mString": "10", "mArray": [ 1, 2, 3 ] })");

			Assert::AreEqual(0, sharedData.GetInt());
			Assert::AreEqual(0.0f, sharedData.GetFloat());
			Assert::AreEqual(std::string(), sharedData.GetString());

			gsl::span array = sharedData.GetArray();
			for (ptrdiff_t i = 0; i < array.size(); ++i)
			{
				Assert::AreEqual(0, array[i]);
			}

			parser.AddHelper(helper);

			parser.Parse(R"({ "mInt": 10, "mFloat": 10, "mString": "10", "mArray": [ 1, 2, 3 ] })");

			Assert::AreEqual(10, sharedData.GetInt());
			Assert::AreEqual(10.0f, sharedData.GetFloat());
			Assert::AreEqual("10"s, sharedData.GetString());

			array = sharedData.GetArray();
			for (ptrdiff_t i = 0; i < array.size(); ++i)
			{
				Assert::AreEqual(static_cast<int>(i + 1), array[i]);
			}

			Assert::IsTrue(parser.RemoveHelper(helper));

			parser.Parse(R"({ "mInt": 10, "mFloat": 10, "mString": "10", "mArray": [ 1, 2, 3 ] })");

			Assert::AreEqual(0, sharedData.GetInt());
			Assert::AreEqual(0.0f, sharedData.GetFloat());
			Assert::AreEqual(std::string(), sharedData.GetString());

			array = sharedData.GetArray();
			for (ptrdiff_t i = 0; i < array.size(); ++i)
			{
				Assert::AreEqual(0, array[i]);
			}

			JsonParseTestHelper::SharedData sharedData2;
			parser.SetSharedData(sharedData2);
			Assert::IsNull(sharedData.GetJsonParseMaster());
		}

		TEST_METHOD(Clone)
		{
			JsonParseTestHelper::SharedData sharedData;
			JsonParseTestHelper helper;
			JsonParseMaster parser;

 			parser.AddHelper(helper);
 			parser.SetSharedData(sharedData);

			JsonParseMaster* clone = parser.Clone();

 			Assert::IsNotNull(clone->GetSharedData());
 
 			clone->Parse(R"({ "mInt": 10, "mFloat": 10, "mString": "10", "mArray": [ 1, 2, 3 ] })");

 			Assert::AreEqual(10, clone->GetSharedData()->As<JsonParseTestHelper::SharedData>()->GetInt());
 			Assert::AreEqual(10.0f, clone->GetSharedData()->As<JsonParseTestHelper::SharedData>()->GetFloat());
 			Assert::AreEqual("10"s, clone->GetSharedData()->As<JsonParseTestHelper::SharedData>()->GetString());
 
 			gsl::span array = clone->GetSharedData()->As<JsonParseTestHelper::SharedData>()->GetArray();
 			for (ptrdiff_t i = 0; i < array.size(); ++i)
 			{
 				Assert::AreEqual(static_cast<int>(i + 1), array[i]);
 			}

			delete clone;
		}

		TEST_METHOD(Parse)
		{
			JsonParseTestHelper::SharedData sharedData;
			JsonParseTestHelper helper;
			JsonParseMaster parser;

			parser.AddHelper(helper);
			parser.SetSharedData(sharedData);

			parser.Parse(R"({ "mTypo": 10, "mFloat": 10, "mString": "10", "mTypo": [ 1, 2, 3 ] })");

			Assert::AreEqual(0, sharedData.GetInt());
			Assert::AreEqual(10.0f, sharedData.GetFloat());
			Assert::AreEqual("10"s, sharedData.GetString());

			gsl::span array = sharedData.GetArray();
			for (ptrdiff_t i = 0; i < array.size(); ++i)
			{
				Assert::AreEqual(0, array[i]);
			}

			parser.Parse(R"({ "mInt": 10, "mFloat": 10, "mString": "10", "mArray": [ 1, 2, 3 ] })");

			Assert::AreEqual(10, sharedData.GetInt());
			Assert::AreEqual(10.0f, sharedData.GetFloat());
			Assert::AreEqual("10"s, sharedData.GetString());
			
 			array = sharedData.GetArray();
 			for (ptrdiff_t i = 0; i < array.size(); ++i)
 			{
 				Assert::AreEqual(static_cast<int>(i+1), array[i]);
 			}

			std::string filename("JsonTest.json");
			std::ofstream fileTest;
			fileTest.open(filename);
			fileTest << R"({ "mInt": 10, "mFloat": 10, "mString": "10", "mArray": [ 1, 2, 3 ] })";
			fileTest.close();

			Assert::IsNull(parser.GetFilename());
			
			parser.ParseFromFile(filename);

			Assert::AreEqual(filename, *parser.GetFilename());
			std::remove(filename.c_str());

			Assert::AreEqual(10, sharedData.GetInt());
			Assert::AreEqual(10.0f, sharedData.GetFloat());
			Assert::AreEqual("10"s, sharedData.GetString());
			
 			array = sharedData.GetArray();
 			for (ptrdiff_t i = 0; i < array.size(); ++i)
 			{
 				Assert::AreEqual(static_cast<int>(i+1), array[i]);
 			}
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState JsonParseTest::sStartMemState;
}