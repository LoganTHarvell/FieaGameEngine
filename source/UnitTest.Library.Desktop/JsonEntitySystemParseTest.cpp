#include "pch.h"

#include "ToStringSpecialization.h"
#include "JsonParseMaster.h"
#include "JsonEntityParseHelper.h"
#include "Entity.h"
#include "FooEntity.h"
#include "World.h"

using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;
using namespace Library;

namespace EntitySystemTests
{
	TEST_CLASS(JsonEntitySystemParseTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			TypeManager::Create();
			RegisterType<Entity>();
			RegisterType<FooEntity>();
			RegisterType<World>();

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

		TEST_METHOD(ParseEntity)
		{
			Entity entity;

			JsonEntityParseHelper::SharedData sharedData;
			sharedData.SetEntity(entity);

			JsonEntityParseHelper helper;
			JsonParseMaster parser;

			parser.AddHelper(helper);
			parser.SetSharedData(sharedData);

			std::string json = R"({
				"AuxiliaryInt": {
				  "type": "integer",
				  "value": 10
				}
			})"s;

			parser.Parse(json);

			Assert::AreEqual(2_z, entity.Size());

			Assert::IsNotNull(entity.Find("AuxiliaryInt"));
			Assert::AreEqual(10, entity.Find(("AuxiliaryInt"))->Get<int>());
		}

		TEST_METHOD(ParseSector)
		{
			Entity sector;

			JsonEntityParseHelper::SharedData sharedData;
			sharedData.SetEntity(sector);

			JsonEntityParseHelper helper;
			JsonParseMaster parser;

			parser.AddHelper(helper);
			parser.SetSharedData(sharedData);

			std::string json = R"({
				"Entity1": {
				  "type": "Entity",
				  "value": {
				    "AuxiliaryInt": {
				      "type": "integer",
				      "value": 10
				    }
				  }
				},
				"Entity2": {
				  "type": "Entity",
				  "value": {
				    "AuxiliaryInt": {
				      "type": "integer",
				      "value": 20
				    }
				  }
				}
			})"s;

			parser.Parse(json);

			Assert::AreEqual(3_z, sector.Size());
			Assert::AreEqual(2_z, sector.ChildCount());

			Entity* entity1 = sector.FindChild("Entity1");
			Entity* entity2 = sector.FindChild("Entity2");

			Assert::IsNotNull(entity1);
			Assert::IsNotNull(entity2);

			Assert::AreEqual("Entity1"s, entity1->Name());
			Assert::IsNotNull(entity1->Find("AuxiliaryInt"));
			Assert::AreEqual(10, entity1->Find(("AuxiliaryInt"))->Get<int>());

			Assert::AreEqual("Entity2"s, entity2->Name());
			Assert::IsNotNull(entity2->Find("AuxiliaryInt"));
			Assert::AreEqual(20, entity2->Find(("AuxiliaryInt"))->Get<int>());
		}

		TEST_METHOD(ParseWorld)
		{
			World world;

			JsonEntityParseHelper::SharedData sharedData;
			sharedData.SetEntity(world);

			JsonEntityParseHelper helper;
			JsonParseMaster parser;

			parser.AddHelper(helper);
			parser.SetSharedData(sharedData);

			parser.ParseFromFile("Content/World.json");
			Assert::AreEqual(2_z, world.ChildCount());

			/* Sector 1 */

			Entity* sector1 = world.FindChild("Sector1");
			Assert::IsNotNull(sector1);

			Assert::AreEqual("Sector1"s, sector1->Name());
			Assert::AreEqual(2_z, sector1->ChildCount());

			Entity* entity1 = sector1->FindChild("Entity1");
			Entity* entity2 = sector1->FindChild("Entity2");
			
			Assert::IsNotNull(entity1);
			Assert::IsNotNull(entity2);

			Assert::AreEqual("Entity1"s, entity1->Name());
			Assert::IsNotNull(entity1->Find("AuxiliaryInt"));
			Assert::AreEqual(10, entity1->Find(("AuxiliaryInt"))->Get<int>());

			Assert::AreEqual("Entity2"s, entity2->Name());
			Assert::IsNotNull(entity2->Find("AuxiliaryInt"));
			Assert::AreEqual(20, entity2->Find(("AuxiliaryInt"))->Get<int>());

			/* Sector 2*/

			Entity* sector2 = world.FindChild("Sector2");
			Assert::IsNotNull(sector2);
			
			Assert::AreEqual("Sector2"s, sector2->Name());			
			Assert::AreEqual(2_z, sector2->ChildCount());

			entity1 = sector2->FindChild("Entity1");
			entity2 = sector2->FindChild("Entity2");

			Assert::IsNotNull(entity1);
			Assert::IsNotNull(entity2);

			Assert::AreEqual("Entity1"s, entity1->Name());
			Assert::IsNotNull(entity1->Find("AuxiliaryInt"));
			Assert::AreEqual(10, entity1->Find(("AuxiliaryInt"))->Get<int>());

			Assert::AreEqual("Entity2"s, entity2->Name());
			Assert::IsNotNull(entity2->Find("AuxiliaryInt"));
			Assert::AreEqual(20, entity2->Find(("AuxiliaryInt"))->Get<int>());
		}

	private:
		static _CrtMemState sStartMemState;

		EntityFactory entityFactory;
		FooEntityFactory fooEntityFactory;
	};

	_CrtMemState JsonEntitySystemParseTest::sStartMemState;
}