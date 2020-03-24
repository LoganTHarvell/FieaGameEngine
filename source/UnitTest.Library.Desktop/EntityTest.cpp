 #include "pch.h"

#include "ToStringSpecialization.h"
#include "Entity.h"
#include "FooEntity.h"
#include "Sector.h"

using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;


namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EntityTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			TypeManager::Create();
			RegisterType<Entity>();
			RegisterType<FooEntity>();
			RegisterType<Sector>();

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
			Entity entity;
			Assert::IsNotNull(entity.Find("Name"));

			FooEntity fooEntity;
			Assert::IsNotNull(fooEntity.Find("Name"));
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
			Entity entity;
			entity.SetName("BaseEntity");
			Assert::AreEqual("BaseEntity (Entity)"s, entity.ToString());

			FooEntity fooEntity;
			fooEntity.SetName("Foo");
			Assert::AreEqual("Foo (FooEntity)"s, fooEntity.ToString());
		}

		TEST_METHOD(Accessors)
		{
			Entity emptyEntity;
			Assert::AreEqual(std::string(), emptyEntity.Name());
			Assert::IsNull(emptyEntity.GetSector());

			Sector sector;
			Entity* entity = sector.CreateEntity("Entity", "BaseEntity");

			entity->SetName("BaseEntity");
			Assert::AreEqual("BaseEntity"s, entity->Name());
			Assert::AreEqual(sector, *entity->GetSector());

			Sector adopter;
			entity->SetSector(adopter);
			Assert::IsNotNull(entity->GetSector());
			Assert::AreEqual(adopter, *entity->GetSector());
			Assert::IsNull(sector.FindScope(*entity).first);
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

			Entity::DataType* aux = entityClone->Find("aux");
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
	};

	_CrtMemState EntityTest::sStartMemState;
}