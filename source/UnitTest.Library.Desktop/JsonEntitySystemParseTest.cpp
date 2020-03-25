#include "pch.h"

#include "ToStringSpecialization.h"
#include "JsonParseMaster.h"
#include "JsonScopeParseHelper.h"
#include "Entity.h"
#include "FooEntity.h"
#include "Sector.h"
#include "World.h"

using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(JsonEntitySystemParseTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			TypeManager::Create();
			RegisterType<Entity>();
			RegisterType<FooEntity>();
			RegisterType<Sector>();
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

			JsonScopeParseHelper::SharedData sharedData;
			sharedData.SetScope(entity);

			JsonScopeParseHelper helper;
			JsonParseMaster parser;

			parser.AddHelper(helper);
			parser.SetSharedData(sharedData);

			std::string json = R"({
				"Name": {
				  "type": "string",
				  "value": "Entity"
				},
				"AuxiliaryInt": {
				  "type": "integer",
				  "value": 10
				}
			})";

			parser.Parse(json);

			Assert::AreEqual(3_z, entity.Size());

			Assert::AreEqual("Entity"s, entity.Name());

			Assert::IsNotNull(entity.Find("AuxiliaryInt"));
			Assert::AreEqual(10, entity.Find(("AuxiliaryInt"))->Get<int>());
		}

		TEST_METHOD(ParseSector)
		{
			Sector sector;

			JsonScopeParseHelper::SharedData sharedData;
			sharedData.SetScope(sector);

			JsonScopeParseHelper helper;
			JsonParseMaster parser;

			parser.AddHelper(helper);
			parser.SetSharedData(sharedData);

			std::string json = R"({
				"Name": {
				  "type": "string",
				  "value": "Sector"
				},
				"Entities": {
				  "type": "Entity",
				  "value": [
				    {
				      "type": "Entity",
				      "value": {
				        "Name": {
				          "type": "string",
				          "value": "Entity1"
				        },
				        "AuxiliaryInt": {
				          "type": "integer",
				          "value": 10
				        }
				      }
				    },
				    {
				      "type": "Entity",
				      "value": {
				        "Name": {
				          "type": "string",
				          "value": "Entity2"
				        },
				        "AuxiliaryInt": {
				          "type": "integer",
				          "value": 20
				        }
				      }
				    }
				  ]
				}
			})";

			parser.Parse(json);

			Assert::AreEqual(3_z, sector.Size());
			Assert::AreEqual("Sector"s, sector.Name());
			Assert::AreEqual(2_z, sector.Entities().Size());

			Assert::IsNotNull(sector.Find("Entities"));
			Assert::AreEqual(sector.Entities(), *sector.Find("Entities"));

			Entity* entity1 = sector.Entities().Get<Scope*>(0)->As<Entity>();
			Entity* entity2 = sector.Entities().Get<Scope*>(1)->As<Entity>();

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

			JsonScopeParseHelper::SharedData sharedData;
			sharedData.SetScope(world);

			JsonScopeParseHelper helper;
			JsonParseMaster parser;

			parser.AddHelper(helper);
			parser.SetSharedData(sharedData);

			parser.ParseFromFile("Content/World.json");

			Assert::AreEqual(3_z, world.Size());
			Assert::AreEqual("World"s, world.Name());
			Assert::AreEqual(2_z, world.Sectors().Size());

			/* Sector 1 */

			Sector* sector1 = world.Sectors().Get<Scope*>(0)->As<Sector>();
			Assert::IsNotNull(sector1);

			Assert::AreEqual("Sector1"s, sector1->Name());

			Assert::IsNotNull(sector1->Find("Entities"));
			Assert::AreEqual(sector1->Entities(), *sector1->Find("Entities"));
			Assert::AreEqual(2_z, sector1->Entities().Size());

			Entity* entity1 = sector1->Entities().Get<Scope*>(0)->As<Entity>();
			Entity* entity2 = sector1->Entities().Get<Scope*>(1)->As<Entity>();
			
			Assert::IsNotNull(entity1);
			Assert::IsNotNull(entity2);

			Assert::AreEqual("Entity1"s, entity1->Name());
			Assert::IsNotNull(entity1->Find("AuxiliaryInt"));
			Assert::AreEqual(10, entity1->Find(("AuxiliaryInt"))->Get<int>());

			Assert::AreEqual("Entity2"s, entity2->Name());
			Assert::IsNotNull(entity2->Find("AuxiliaryInt"));
			Assert::AreEqual(20, entity2->Find(("AuxiliaryInt"))->Get<int>());

			/* Sector 2*/

			Sector* sector2 = world.Sectors().Get<Scope*>(1)->As<Sector>();
			Assert::IsNotNull(sector2);
			
			Assert::AreEqual("Sector2"s, sector2->Name());
			
			Assert::IsNotNull(sector2->Find("Entities"));
			Assert::AreEqual(sector2->Entities(), *sector2->Find("Entities"));
			Assert::AreEqual(2_z, sector2->Entities().Size());

			entity1 = sector2->Entities().Get<Scope*>(0)->As<Entity>();
			entity2 = sector2->Entities().Get<Scope*>(1)->As<Entity>();

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
		SectorFactory sectorFactory;
	};

	_CrtMemState JsonEntitySystemParseTest::sStartMemState;
}