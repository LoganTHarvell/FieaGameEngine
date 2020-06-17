#include "pch.h"

#include "ToStringSpecialization.h"
#include "Entity.h"
#include "FooEntity.h"
#include "Sector.h"
#include "ActionIncrement.h"

using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;

namespace EntitySystemTests
{
	TEST_CLASS(EntityTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			TypeManager::Create();
			RegisterType<Sector>();
			RegisterType<Entity>();
			RegisterType<FooEntity>();
			RegisterType<Action>();
			RegisterType<ActionIncrement>();

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
			Entity entity("Entity");
			Assert::AreEqual("Entity"s, entity.Name());
			Assert::IsNotNull(entity.Find("Name"));
			Assert::AreEqual("Entity"s, entity.Find("Name")->Get<std::string>());

			FooEntity fooEntity("FooEntity");
			Assert::AreEqual("FooEntity"s, fooEntity.Name());
			Assert::IsNotNull(fooEntity.Find("Name"));
			Assert::AreEqual("FooEntity"s, fooEntity.Find("Name")->Get<std::string>());
			Assert::IsNotNull(fooEntity.Find("Data"));
			Assert::AreEqual(0, fooEntity.Find("Data")->Get<int>());
		}

		TEST_METHOD(RTTITest)
		{
			Entity a;
			Entity b;

			Assert::IsTrue(a.Is(Attributed::TypeIdClass()));
			Assert::IsTrue(a.Equals(&b));

			FooEntity aFoo;
			FooEntity bFoo;

			Assert::IsTrue(aFoo.Is(Entity::TypeIdClass()));
			Assert::IsTrue(aFoo.Equals(&bFoo));

			Entity* fooEntity = new FooEntity();
			bool isEntity = fooEntity->Is(Entity::TypeIdClass());

			Entity* createdFooEntity = isEntity ? createdFooEntity = fooEntity->CreateAs<Entity>() : nullptr;
			bool wasCreated = createdFooEntity != nullptr;

			bool isFooEntity = wasCreated ? createdFooEntity->Is(FooEntity::TypeIdClass()) : false;

			delete fooEntity;
			delete createdFooEntity;

			Assert::IsTrue(isEntity);
			Assert::IsTrue(wasCreated);
			Assert::IsTrue(isFooEntity);
		}

		TEST_METHOD(ToString)
		{
			Entity entity("BaseEntity");
			Assert::AreEqual("BaseEntity (Entity)"s, entity.ToString());

			FooEntity fooEntity("Foo");
			Assert::AreEqual("Foo (FooEntity)"s, fooEntity.ToString());
		}

		TEST_METHOD(Accessors)
		{
			Entity emptyEntity;
			emptyEntity.SetName("EmptyEntity");
			Assert::AreEqual("EmptyEntity"s, emptyEntity.Name());
			Assert::IsNull(emptyEntity.GetSector());

			Sector sector;
			Entity* entity = sector.CreateEntity("Entity", "BaseEntity");
			Assert::IsNotNull(entity);
			Assert::AreEqual("BaseEntity"s, entity->Name());
			Assert::AreEqual(sector, *entity->GetSector());

			Sector adopter;
			entity->SetSector(&adopter);
			Assert::IsNotNull(entity->GetSector());
			Assert::AreEqual(adopter, *entity->GetSector());
			Assert::IsNull(sector.FindScope(*entity).first);

			Assert::IsNull(entity->CreateAction("Invalid", "Name"));

			Action* entity1 = entity->CreateAction("ActionIncrement", "ActionIncrement1");
			Action* entity2 = entity->CreateAction("ActionIncrement", "ActionIncrement2");

			Assert::AreEqual(2_z, entity->Actions().Size());
			Assert::AreEqual(entity1, entity->Actions().Get<Scope*>(0)->As<Action>());
			Assert::AreEqual(entity2, entity->Actions().Get<Scope*>(1)->As<Action>());

			const Entity copy = *entity;

			Assert::AreEqual(2_z, copy.Actions().Size());
			Assert::AreEqual(entity1, copy.Actions().Get<Scope*>(0)->As<Action>());
			Assert::AreEqual(entity2, copy.Actions().Get<Scope*>(1)->As<Action>());

			entity->SetSector(nullptr);
			Assert::IsNull(entity->GetSector());
			delete entity;
		}

		TEST_METHOD(Update)
		{
			FooEntity fooEntity;
			Entity* entity = &fooEntity;

			WorldState worldState;

			entity->Update(worldState);

			Assert::IsTrue(fooEntity.IsUpdated());
		}

		TEST_METHOD(Clone)
		{
			Entity entity;
			Attributed* entityBasePtr = &entity;

			entityBasePtr->AppendAuxiliaryAttribute("aux") = 10;

			Scope* entityClone = entityBasePtr->Clone();
			Assert::IsNotNull(entityClone);

			bool isEntity = entityClone->Is(Entity::TypeIdClass());

			Entity::Data* aux = entityClone->Find("aux");
			int copiedAuxValue = aux ? aux->Get<int>() : 0;

			delete entityClone;

			Assert::IsTrue(isEntity);
			Assert::IsNotNull(aux);
			Assert::AreEqual(10, copiedAuxValue);

			FooEntity fooEntity;
			Entity* fooEntityBasePtr = &fooEntity;
			fooEntityBasePtr->AppendAuxiliaryAttribute("aux") = 10;

			Scope* fooEntityClone = fooEntityBasePtr->Clone();
			Assert::IsNotNull(fooEntityClone);

			bool isFooEntity = fooEntityClone->Is(FooEntity::TypeIdClass());

			aux = fooEntityClone->Find("aux");
			copiedAuxValue = aux ? aux->Get<int>() : 0;

			delete fooEntityClone;

			Assert::IsTrue(isFooEntity);
			Assert::IsNotNull(aux);
			Assert::AreEqual(10, copiedAuxValue);
		}

	private:
		static _CrtMemState sStartMemState;

		EntityFactory entityFactory;
		FooEntityFactory fooEntityFactory;
		SectorFactory sectorFactory;
		ActionIncrementFactory actionIncrementFactory;
	};

	_CrtMemState EntityTest::sStartMemState;
}