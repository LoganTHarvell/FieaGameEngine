#include "pch.h"

#include "ToStringSpecialization.h"
#include "JsonParseMaster.h"
#include "JsonScopeParseHelper.h"


using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(JsonTableParseTest)
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
			JsonScopeParseHelper::SharedData sharedData;
			JsonScopeParseHelper helper;
			JsonParseMaster parser;

			parser.AddHelper(helper);
			parser.SetSharedData(sharedData);

			JsonParseMaster moveConstructed(std::move(parser));
			Assert::IsNull(parser.GetSharedData());

			{
				const JsonParseMaster::SharedData* sharedDataPtr = &sharedData;
				Assert::AreEqual(sharedDataPtr, moveConstructed.GetSharedData());
			}

			moveConstructed.Parse(R"({ "mInt": 10 })");

			JsonParseMaster moveAssigned;
			moveAssigned = std::move(moveConstructed);
			Assert::IsNull(parser.GetSharedData());
			Assert::IsNull(moveConstructed.GetSharedData());

// 			{
// 				const JsonParseMaster::SharedData* sharedDataPtr = &sharedData;
// 				Assert::AreEqual(sharedDataPtr, moveAssigned.GetSharedData());
// 			}
// 
// 			moveAssigned.Parse(R"({ "mInt": 10 })");
// 
// 			JsonScopeParseHelper::SharedData moveConstructedSharedData(std::move(sharedData));
// 			{
// 				const JsonParseMaster::SharedData* sharedDataPtr = &moveConstructedSharedData;
// 				Assert::AreEqual(sharedDataPtr, moveAssigned.GetSharedData());
// 			}
// 
// 			JsonScopeParseHelper::SharedData moveAssignedSharedData;
// 			moveAssignedSharedData = std::move(moveConstructedSharedData);
// 			{
// 				const JsonParseMaster::SharedData* sharedDataPtr = &moveAssignedSharedData;
// 				Assert::AreEqual(sharedDataPtr, moveAssigned.GetSharedData());
// 			}
		}

		TEST_METHOD(Accessors)
		{
			JsonScopeParseHelper::SharedData sharedData;

			JsonScopeParseHelper helper;
			JsonParseMaster parser;

			Assert::IsNull(sharedData.GetJsonParseMaster());
			Assert::IsNull(parser.GetSharedData());
			parser.SetSharedData(sharedData);
			Assert::IsNotNull(sharedData.GetJsonParseMaster());
			Assert::IsNotNull(parser.GetSharedData());

			{
				const JsonScopeParseHelper::SharedData constSharedData;
				Assert::IsNull(constSharedData.GetJsonParseMaster());
			}
			{
				const JsonScopeParseHelper::SharedData constSharedData = sharedData;
				Assert::AreEqual(sharedData.GetJsonParseMaster(), constSharedData.GetJsonParseMaster());
			}

			const JsonParseMaster constParser;
			Assert::IsNull(constParser.GetSharedData());
		}

		TEST_METHOD(Modifiers)
		{
			JsonScopeParseHelper::SharedData sharedData;
			JsonScopeParseHelper helper;
			JsonParseMaster parser;

			parser.SetSharedData(sharedData);

			parser.Parse(R"({ "mInt": 10 })");

			parser.AddHelper(helper);
			Assert::ExpectException<std::runtime_error>([&parser, &helper] { parser.AddHelper(helper); });

			parser.Parse(R"({ "mInt": 10, "null": null })");

			Assert::IsTrue(parser.RemoveHelper(helper));

			parser.Parse(R"({ "mInt": 10 })");

			JsonScopeParseHelper::SharedData sharedData2;
			parser.SetSharedData(sharedData2);
			Assert::IsNull(sharedData.GetJsonParseMaster());
		}

		TEST_METHOD(Clone)
		{
			JsonScopeParseHelper::SharedData sharedData;
			JsonScopeParseHelper helper;
			JsonParseMaster parser;

			parser.AddHelper(helper);
			parser.SetSharedData(sharedData);

			JsonParseMaster* clone = parser.Clone();

			Assert::IsNotNull(clone->GetSharedData());

			clone->Parse(R"({ "mInt": 10 })");


			delete clone;
		}

		TEST_METHOD(ParsePrimitives)
		{
			JsonScopeParseHelper::SharedData sharedData;
			JsonScopeParseHelper helper;
			JsonParseMaster parser;

			parser.AddHelper(helper);
			parser.SetSharedData(sharedData);

			std::string filename("Content/ScopeOfPrimitives.json");
			parser.ParseFromFile(filename);

			const Scope& scope = sharedData.GetScope();

			Assert::AreEqual(10, scope.Find("Integer")->Get<int>());
			Assert::AreEqual(10.0f, scope.Find("Float")->Get<float>());
			Assert::AreEqual("10"s, scope.Find("String")->Get<std::string>());
			Assert::AreEqual(glm::vec4(10), scope.Find("Vector")->Get<glm::vec4>());
			Assert::AreEqual(glm::mat4(10), scope.Find("Matrix")->Get<glm::mat4>());
		}

		TEST_METHOD(ParseArray)
		{
			JsonScopeParseHelper::SharedData sharedData;
			JsonScopeParseHelper helper;
			JsonParseMaster parser;

			parser.AddHelper(helper);
			parser.SetSharedData(sharedData);

			std::string filename("Content/ScopeOfArrays.json");
			parser.ParseFromFile(filename);

			const Scope& scope = sharedData.GetScope();

			Assert::AreEqual(10, scope.Find("IntegerArray")->Get<int>(0));
			Assert::AreEqual(10.0f, scope.Find("FloatArray")->Get<float>(0));
			Assert::AreEqual("10"s, scope.Find("StringArray")->Get<std::string>(0));
			Assert::AreEqual(glm::vec4(10), scope.Find("VectorArray")->Get<glm::vec4>(0));
			Assert::AreEqual(glm::mat4(10), scope.Find("MatrixArray")->Get<glm::mat4>(0));

			Assert::AreEqual(10, scope.Find("IntegerArray")->Get<int>(1));
			Assert::AreEqual(10.0f, scope.Find("FloatArray")->Get<float>(1));
			Assert::AreEqual("10"s, scope.Find("StringArray")->Get<std::string>(1));
			Assert::AreEqual(glm::vec4(10), scope.Find("VectorArray")->Get<glm::vec4>(1));
			Assert::AreEqual(glm::mat4(10), scope.Find("MatrixArray")->Get<glm::mat4>(1));
		}

		TEST_METHOD(ParseNestedScopes)
		{
			JsonScopeParseHelper::SharedData sharedData;
			JsonScopeParseHelper helper;
			JsonParseMaster parser;

			parser.AddHelper(helper);
			parser.SetSharedData(sharedData);

			std::string filename("Content/ScopeOfScopes.json");
			parser.ParseFromFile(filename);

			const Scope& scope = sharedData.GetScope();
			const Scope::DataType* nestedScopeData1 = scope.Find("NestedScope1");
			const Scope::DataType* nestedScopeData2 = scope.Find("NestedScope2");

			Assert::IsNotNull(nestedScopeData1);
			Assert::IsNotNull(nestedScopeData2);

			const Scope* nestedScope1 = nestedScopeData1->Get<Scope*>();
			const Scope* nestedScope2 = nestedScopeData2->Get<Scope*>();

			Assert::AreEqual(10, nestedScope1->Find("Integer")->Get<int>());
			Assert::AreEqual(10.0f, nestedScope1->Find("Float")->Get<float>());
			Assert::AreEqual("10"s, nestedScope1->Find("String")->Get<std::string>());
			Assert::AreEqual(glm::vec4(10), nestedScope1->Find("Vector")->Get<glm::vec4>());
			Assert::AreEqual(glm::mat4(10), nestedScope1->Find("Matrix")->Get<glm::mat4>());

			Assert::AreEqual(10, nestedScope2->Find("Integer")->Get<int>());
			Assert::AreEqual(10.0f, nestedScope2->Find("Float")->Get<float>());
			Assert::AreEqual("10"s, nestedScope2->Find("String")->Get<std::string>());
			Assert::AreEqual(glm::vec4(10), nestedScope2->Find("Vector")->Get<glm::vec4>());
			Assert::AreEqual(glm::mat4(10), nestedScope2->Find("Matrix")->Get<glm::mat4>());
		}

		TEST_METHOD(RTTITest)
		{
			std::shared_ptr<JsonScopeParseHelper> derivedHelper = std::make_shared<JsonScopeParseHelper>();

			Assert::IsTrue(derivedHelper->Is(IJsonParseHelper::TypeIdClass()));
			Assert::IsNotNull(derivedHelper->As<JsonScopeParseHelper>());

			std::shared_ptr<IJsonParseHelper> helper = derivedHelper;
			Assert::IsTrue(helper->Is(JsonScopeParseHelper::TypeIdClass()));
			Assert::IsNotNull(helper->As<JsonScopeParseHelper>());

			std::shared_ptr<JsonScopeParseHelper::SharedData> derivedSharedData = std::make_shared<JsonScopeParseHelper::SharedData>();

			Assert::IsTrue(derivedSharedData->Is(JsonParseMaster::SharedData::TypeIdClass()));
			Assert::IsNotNull(derivedSharedData->As<JsonParseMaster::SharedData>());

			std::shared_ptr<JsonParseMaster::SharedData> sharedData = derivedSharedData;

			Assert::IsTrue(sharedData->Is(JsonScopeParseHelper::SharedData::TypeIdClass()));
			Assert::IsNotNull(sharedData->As<JsonScopeParseHelper::SharedData>());
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState JsonTableParseTest::sStartMemState;
}