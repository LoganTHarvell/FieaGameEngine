 #include "pch.h"

#include "ToStringSpecialization.h"
#include "Entity.h"
#include "FooEntity.h"
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
			RegisterType<Entity>();
			RegisterType<FooEntity>();
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
			const Entity entity("Entity");
			Assert::AreEqual("Entity"s, entity.Name());

			FooEntity fooEntity("FooEntity");
			Assert::AreEqual("FooEntity"s, fooEntity.Name());
			Assert::IsNotNull(fooEntity.Find("Data"));
			Assert::AreEqual(0.0f, fooEntity.Find("Data")->Get<float>());
		}

		TEST_METHOD(RTTITest)
		{
			const Entity a;
			Entity b;

			Assert::IsTrue(a.Is(Attributed::TypeIdClass()));
			Assert::IsTrue(a.Equals(&b));

			const FooEntity aFoo;
			FooEntity bFoo;

			Assert::IsTrue(aFoo.Is(Entity::TypeIdClass()));
			Assert::IsTrue(aFoo.Equals(&bFoo));

			Entity* fooEntity = new FooEntity();
			const bool isEntity = fooEntity->Is(Entity::TypeIdClass());

			Entity* createdFooEntity = isEntity ? createdFooEntity = fooEntity->CreateAs<Entity>() : nullptr;
			const bool wasCreated = createdFooEntity != nullptr;

			const bool isFooEntity = wasCreated ? createdFooEntity->Is(FooEntity::TypeIdClass()) : false;
			
			delete fooEntity;
			delete createdFooEntity;

			Assert::IsTrue(isEntity);
			Assert::IsTrue(wasCreated);
			Assert::IsTrue(isFooEntity);
		}

		TEST_METHOD(ToString)
		{
			const Entity entity("BaseEntity");
			Assert::AreEqual("BaseEntity (Entity)"s, entity.ToString());

			const FooEntity fooEntity("Foo");
			Assert::AreEqual("Foo (FooEntity)"s, fooEntity.ToString());
		}

		TEST_METHOD(Accessors)
		{
			Entity emptyEntity;
			emptyEntity.SetName("EmptyEntity");
			Assert::AreEqual("EmptyEntity"s, emptyEntity.Name());
			Assert::IsNull(emptyEntity.GetParent());

			Entity sectorRoot;
			Entity& entity = sectorRoot.CreateChild("Entity", "BaseEntity");
			Assert::AreEqual("BaseEntity"s, entity.Name());
			Assert::AreEqual(sectorRoot, *entity.GetParent());

			Entity adopter;
			entity.SetParent(&adopter);
			Assert::IsNotNull(entity.GetParent());
			Assert::AreEqual(adopter, *entity.GetParent());
			Assert::IsNull(sectorRoot.FindScope(entity).first);

			Assert::ExpectException<std::runtime_error>([&entity] { entity.CreateChild("Invalid", "Name"); });

			Entity& entity1 = entity.CreateChild("ActionIncrement", "ActionIncrement1");
			Entity& entity2 = entity.CreateChild("ActionIncrement", "ActionIncrement2");

			Assert::AreEqual(2_z, entity.ChildCount());
			Assert::AreEqual(&entity1, entity.FindChild("ActionIncrement1"));
			Assert::AreEqual(&entity2, entity.FindChild("ActionIncrement2"));
			Assert::AreEqual(&entity2, entity.FindChildArray("ActionIncrement2")[0]);

			const Entity copy = entity;

			Assert::AreEqual(2_z, copy.ChildCount());
			Assert::AreEqual(entity1, *copy.FindChild("ActionIncrement1"));
			Assert::AreEqual(entity2, *copy.FindChild("ActionIncrement2"));
			Assert::AreEqual(entity2, *copy.FindChildArray("ActionIncrement2")[0]);

			entity.SetParent(nullptr);
			Assert::IsNull(entity.GetParent());
			delete &entity;
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

			const bool isEntity = entityClone->Is(Entity::TypeIdClass());

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

			const bool isFooEntity = fooEntityClone->Is(FooEntity::TypeIdClass());

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
		ActionIncrementFactory actionIncrementFactory;
	};

	_CrtMemState EntityTest::sStartMemState;
}