 #include "pch.h"

#include "ToStringSpecialization.h"
#include "World.h"
#include "GameTime.h"
#include "EventQueue.h"
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
			const World world("World");
			Assert::AreEqual(0_z, world.PendingChildren().Size());
			Assert::AreEqual("World"s, world.Name());
		}

		TEST_METHOD(Copy)
		{
			const auto lhs = Signature();
			const auto rhs = Signature();
			Assert::IsTrue(lhs == rhs);
			
			GameTime gameTime;
			EventQueue eventQueue;
			const World world("World", &gameTime, &eventQueue);

			World copyConstructed(world);
			Assert::AreEqual("World"s, copyConstructed.Name());
			Assert::IsNotNull(copyConstructed.GetWorldState().World);
			Assert::AreEqual(&copyConstructed, copyConstructed.GetWorldState().World);
			Assert::IsNotNull(copyConstructed.GetWorldState().GameTime);
			Assert::AreEqual(&gameTime, copyConstructed.GetWorldState().GameTime);
			Assert::IsNotNull(copyConstructed.GetWorldState().EventQueue);
			Assert::AreEqual(&eventQueue, copyConstructed.GetWorldState().EventQueue);

			World copyAssigned;
			copyAssigned = copyConstructed;

			Assert::AreEqual("World"s, copyAssigned.Name());
			Assert::IsNotNull(copyAssigned.GetWorldState().World);
			Assert::AreEqual(&copyAssigned, copyAssigned.GetWorldState().World);
			Assert::IsNotNull(copyAssigned.GetWorldState().GameTime);
			Assert::AreEqual(&gameTime, copyAssigned.GetWorldState().GameTime);
			Assert::IsNotNull(copyAssigned.GetWorldState().EventQueue);
			Assert::AreEqual(&eventQueue, copyAssigned.GetWorldState().EventQueue);
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
			const World a;
			World b;

			Assert::IsTrue(a.Is(Attributed::TypeIdClass()));
			Assert::IsTrue(a.Equals(&b));

			World* world = new World();
			const bool isEntity = world->Is(World::TypeIdClass());

			Attributed* createdWorld = isEntity ? createdWorld = world->CreateAs<World>() : nullptr;
			const bool wasCreated = createdWorld != nullptr;

			const bool isFooEntity = wasCreated ? createdWorld->Is(World::TypeIdClass()) : false;
			
			delete world;
			delete createdWorld;

			Assert::IsTrue(isEntity);
			Assert::IsTrue(wasCreated);
			Assert::IsTrue(isFooEntity);
		}

		TEST_METHOD(ToString)
		{
			const World world("World");
			Assert::AreEqual("World (World)"s, world.ToString());
		}

		TEST_METHOD(Accessors)
		{
			World world;
			world.SetName("World");
			Assert::AreEqual("World"s, world.Name());
			
			Assert::AreEqual(&world, world.GetWorldState().World);

			Entity& sector1 = world.CreateChild("Entity"s, "Sector1"s);
			Entity& sector2 = world.CreateChild("Entity"s, "Sector2"s);

			Assert::AreEqual("Sector1"s, sector1.Name());
			Assert::AreEqual(world, *sector1.GetParent()->As<World>());

			Assert::AreEqual("Sector2"s, sector2.Name());
			Assert::AreEqual(world, *sector2.GetParent()->As<World>());
			
			Assert::AreEqual(2_z, world.ChildCount());
			Assert::AreEqual(sector1, *world.FindChild("Sector1"));
			Assert::AreEqual(sector2, *world.FindChild("Sector2"));

			const World copy = world;

			Assert::IsNull(copy.GetWorldState().GameTime);
			Assert::IsNull(copy.GetWorldState().EventQueue);
			Assert::AreEqual(&copy, copy.GetWorldState().World);
			Assert::IsNull(copy.GetWorldState().Entity);
			Assert::IsNull(copy.GetWorldState().Entity);
			
			Assert::AreEqual(2_z, copy.ChildCount());
			Assert::AreEqual(sector1, *copy.FindChild("Sector1"));
			Assert::AreEqual(sector2, *copy.FindChild("Sector2"));
			Assert::AreEqual(0_z, copy.PendingChildren().Size());
		}

		TEST_METHOD(Update)
		{
			GameTime gameTime;
			EventQueue queue;
			
			World world("World", &gameTime, &queue);
			Entity& sector1 = world.CreateChild("Entity", "Sector1");
			Entity& sector2 = world.CreateChild("Entity", "Sector2");

			Entity& fooEntity1 = sector1.CreateChild("FooEntity", "Foo1");
			Entity& fooEntity2 = sector2.CreateChild("FooEntity", "Foo2");

			world.Update();

			Assert::IsTrue(fooEntity1.As<FooEntity>()->IsUpdated());
			Assert::IsTrue(fooEntity2.As<FooEntity>()->IsUpdated());
		}

		TEST_METHOD(Clone)
		{
 			World sector;
 			Attributed* sectorBasePtr = &sector;

			sectorBasePtr->AppendAuxiliaryAttribute("aux") = 10;

			Scope* sectorClone = sectorBasePtr->Clone();
			Assert::IsNotNull(sectorClone);

			const bool isEntity = sectorClone->Is(World::TypeIdClass());

			Entity::Data* aux = sectorClone->Find("aux");
			const int copiedAuxValue = aux ? aux->Get<int>() : 0;

			delete sectorClone;

			Assert::IsTrue(isEntity);
			Assert::IsNotNull(aux);
			Assert::AreEqual(10, copiedAuxValue);
		}

	private:
		static _CrtMemState sStartMemState;

		EntityFactory entityFactory;
		FooEntityFactory fooEntityFactory;
	};

	_CrtMemState WorldTest::sStartMemState;
}