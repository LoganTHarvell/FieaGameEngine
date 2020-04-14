 #include "pch.h"

#include "ToStringSpecialization.h"
#include "World.h"
#include "Sector.h"
#include "FooEntity.h"

using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;


namespace EntitySystemTests
{
	TEST_CLASS(WorldTest)
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
			World world("World");
			Assert::AreEqual(0_z, world.PendingChildren().Size());
			Assert::AreEqual("World"s, world.Name());
			Assert::IsNotNull(world.Find("Name"));
			Assert::AreEqual("World"s, world.Find("Name")->Get<std::string>());
			Assert::IsNotNull(world.Find("Sectors"));
			Assert::AreEqual(0_z, world.Find("Sectors")->Size());
		}

		TEST_METHOD(Move)
		{
			GameTime gameTime;
			EventQueue eventQueue;
			World world("World", &gameTime, &eventQueue);

			World moveConstructed(std::move(world));
			Assert::AreEqual("World"s, moveConstructed.Name());
			Assert::IsNotNull(moveConstructed.GetWorldState().World);
			Assert::AreEqual(&moveConstructed, moveConstructed.GetWorldState().World);
			Assert::IsNotNull(moveConstructed.GetWorldState().GameTime);
			Assert::AreEqual(&gameTime, moveConstructed.GetWorldState().GameTime);
			Assert::IsNotNull(moveConstructed.GetWorldState().EventQueue);
			Assert::AreEqual(&eventQueue, moveConstructed.GetWorldState().EventQueue);

			Assert::IsTrue(world.Name().empty());
			Assert::IsNull(world.GetWorldState().GameTime);
			Assert::IsNull(world.GetWorldState().EventQueue);

			World moveAssigned;
			moveAssigned = std::move(moveConstructed);

			Assert::AreEqual("World"s, moveAssigned.Name());
			Assert::IsNotNull(moveAssigned.GetWorldState().World);
			Assert::AreEqual(&moveAssigned, moveAssigned.GetWorldState().World);
			Assert::IsNotNull(moveAssigned.GetWorldState().GameTime);
			Assert::AreEqual(&gameTime, moveAssigned.GetWorldState().GameTime);
			Assert::IsNotNull(moveAssigned.GetWorldState().EventQueue);
			Assert::AreEqual(&eventQueue, moveAssigned.GetWorldState().EventQueue);

			Assert::IsTrue(moveConstructed.Name().empty());
			Assert::IsNull(moveConstructed.GetWorldState().GameTime);
			Assert::IsNull(moveConstructed.GetWorldState().EventQueue);
		}

		TEST_METHOD(RTTITest)
		{
			World a;
			World b;

			Assert::IsTrue(a.Is(Attributed::TypeIdClass()));
			Assert::IsTrue(a.Equals(&b));

			Attributed* world = new World();
			bool isEntity = world->Is(World::TypeIdClass());

			Attributed* createdWorld = isEntity ? createdWorld = world->CreateAs<World>() : nullptr;
			bool wasCreated = createdWorld != nullptr;

			bool isFooEntity = wasCreated ? createdWorld->Is(World::TypeIdClass()) : false;
			
			delete world;
			delete createdWorld;

			Assert::IsTrue(isEntity);
			Assert::IsTrue(wasCreated);
			Assert::IsTrue(isFooEntity);
		}

		TEST_METHOD(ToString)
		{
			World world("World");
			Assert::AreEqual("World (World)"s, world.ToString());
		}

		TEST_METHOD(Accessors)
		{
			World world;
			world.SetName("World");
			Assert::AreEqual("World"s, world.Name());
			Assert::AreEqual(0_z, world.PendingChildren().Size());
			
			Assert::AreEqual(&world, world.GetWorldState().World);

			Sector& sector1 = world.CreateSector("Sector1");
			Sector& sector2 = world.CreateSector("Sector2");

			Assert::AreEqual("Sector1"s, sector1.Name());
			Assert::AreEqual(world, *sector1.GetWorld());

			Assert::AreEqual("Sector2"s, sector2.Name());
			Assert::AreEqual(world, *sector2.GetWorld());
			
			Assert::AreEqual(2_z, world.Sectors().Size());
			Assert::AreEqual(sector1, *world.Sectors().Get<Scope*>(0)->As<Sector>());
			Assert::AreEqual(sector2, *world.Sectors().Get<Scope*>(1)->As<Sector>());

			const World copy = world;

			Assert::AreEqual(&world, copy.GetWorldState().World);
			Assert::AreEqual(2_z, copy.Sectors().Size());
			Assert::AreEqual(sector1, *copy.Sectors().Get<Scope*>(0)->As<Sector>());
			Assert::AreEqual(sector2, *copy.Sectors().Get<Scope*>(1)->As<Sector>());
			Assert::AreEqual(0_z, copy.PendingChildren().Size());
		}

		TEST_METHOD(Update)
		{
			World world;
			Sector& sector1 = world.CreateSector("Sector1");
			Sector& sector2 = world.CreateSector("Sector2");

			Entity* fooEntity1 = sector1.CreateEntity("FooEntity", "Foo1");
			Entity* fooEntity2 = sector2.CreateEntity("FooEntity", "Foo2");

			world.Update();

			Assert::IsTrue(fooEntity1->As<FooEntity>()->IsUpdated());
			Assert::IsTrue(fooEntity2->As<FooEntity>()->IsUpdated());
		}

		TEST_METHOD(Clone)
		{
 			World sector;
 			Attributed* sectorBasePtr = &sector;

			sectorBasePtr->AppendAuxiliaryAttribute("aux") = 10;

			Scope* sectorClone = sectorBasePtr->Clone();
			Assert::IsNotNull(sectorClone);

			bool isEntity = sectorClone->Is(World::TypeIdClass());

			World::DataType* aux = sectorClone->Find("aux");
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

	_CrtMemState WorldTest::sStartMemState;
}