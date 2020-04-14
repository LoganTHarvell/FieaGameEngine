 #include "pch.h"

#include "ToStringSpecialization.h"
#include "Sector.h"
#include "World.h"
#include "FooEntity.h"

using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;


namespace EntitySystemTests
{
	TEST_CLASS(SectorTest)
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

		TEST_METHOD(Constructor)
		{
			Sector sector("Sector");
			Assert::AreEqual("Sector"s, sector.Name());
			Assert::IsNotNull(sector.Find("Name"));
			Assert::AreEqual("Sector"s, sector.Find("Name")->Get<std::string>());
			Assert::IsNotNull(sector.Find("Entities"));
			Assert::AreEqual(0_z, sector.Find("Entities")->Size());
		}

		TEST_METHOD(RTTITest)
		{
			Sector a;
			Sector b;

			Assert::IsTrue(a.Is(Attributed::TypeIdClass()));
			Assert::IsTrue(a.Equals(&b));

			Attributed* sector = new Sector();
			bool isEntity = sector->Is(Sector::TypeIdClass());

			Attributed* createdSector = isEntity ? createdSector = sector->CreateAs<Sector>() : nullptr;
			bool wasCreated = createdSector != nullptr;

			bool isFooEntity = wasCreated ? createdSector->Is(Sector::TypeIdClass()) : false;
			
			delete sector;
			delete createdSector;

			Assert::IsTrue(isEntity);
			Assert::IsTrue(wasCreated);
			Assert::IsTrue(isFooEntity);
		}

		TEST_METHOD(ToString)
		{
			Sector sector("Sector");
			Assert::AreEqual("Sector (Sector)"s, sector.ToString());
		}

		TEST_METHOD(Accessors)
		{
			Sector emptySector;
			emptySector.SetName("EmptySector");
			Assert::AreEqual("EmptySector"s, emptySector.Name());
			Assert::IsNull(emptySector.GetWorld());

			World world;
			Sector& sector = world.CreateSector("Sector");

			Assert::AreEqual("Sector"s, sector.Name());
			Assert::AreEqual(world, *sector.GetWorld());

			World adopter;
			sector.SetWorld(&adopter);
			Assert::IsNotNull(sector.GetWorld());
			Assert::AreEqual(adopter, *sector.GetWorld());
			Assert::IsNull(world.FindScope(sector).first);

			Assert::IsNull(sector.CreateEntity("Invalid", "Name"));

			Entity* entity1 = sector.CreateEntity("Entity", "Entity1");
			Entity* entity2 = sector.CreateEntity("Entity", "Entity2");
			
			Assert::AreEqual(2_z, sector.Entities().Size());
			Assert::AreEqual(entity1, sector.Entities().Get<Scope*>(0)->As<Entity>());
			Assert::AreEqual(entity2, sector.Entities().Get<Scope*>(1)->As<Entity>());

			const Sector copy = sector;

			Assert::AreEqual(2_z, copy.Entities().Size());
			Assert::AreEqual(entity1, copy.Entities().Get<Scope*>(0)->As<Entity>());
			Assert::AreEqual(entity2, copy.Entities().Get<Scope*>(1)->As<Entity>());

			sector.SetWorld(nullptr);
			Assert::IsNull(sector.GetWorld());
			delete &sector;
		}

		TEST_METHOD(Update)
		{
			Sector sector;
			Entity* fooEntity1 = sector.CreateEntity("FooEntity", "Foo1");
			Entity* fooEntity2 = sector.CreateEntity("FooEntity", "Foo2");

			WorldState worldState;
			sector.Update(worldState);

			Assert::IsTrue(fooEntity1->Is(FooEntity::TypeIdClass()));
			Assert::IsTrue(fooEntity1->As<FooEntity>()->IsUpdated());

			Assert::IsTrue(fooEntity2->Is(FooEntity::TypeIdClass()));
			Assert::IsTrue(fooEntity2->As<FooEntity>()->IsUpdated());
		}

		TEST_METHOD(Clone)
		{
 			Sector sector;
 			Attributed* sectorBasePtr = &sector;

			sectorBasePtr->AppendAuxiliaryAttribute("aux") = 10;

			Scope* sectorClone = sectorBasePtr->Clone();
			Assert::IsNotNull(sectorClone);

			bool isEntity = sectorClone->Is(Sector::TypeIdClass());

			Sector::Data* aux = sectorClone->Find("aux");
			int copiedAuxValue = aux ? aux->Get<int>() : 0;

			delete sectorClone;

			Assert::IsTrue(isEntity);
			Assert::IsNotNull(aux);
			Assert::AreEqual(10, copiedAuxValue);
		}

	private:
		static _CrtMemState sStartMemState;

		EntityFactory entityFactory;
		FooEntityFactory fooEntityFactory;
		SectorFactory sectorFactory;
	};

	_CrtMemState SectorTest::sStartMemState;
}