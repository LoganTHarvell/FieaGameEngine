#include "pch.h"

#include "ToStringSpecialization.h"
#include "JsonParseMaster.h"
#include "JsonScopeParseHelper.h"
#include "Entity.h"
#include "Sector.h"
#include "World.h"
#include "ActionListForEach.h"
#include "ActionIncrement.h"

using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace EntitySystemTests::ActionTests
{
	TEST_CLASS(ActionListForEachTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			TypeManager::Create();

			RegisterType<World>();
			RegisterType<Sector>();
			RegisterType<Entity>();

			RegisterType<Action>();
			RegisterType<ActionList>();
			RegisterType<ActionListForEach>();
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
		TEST_METHOD(Update)
		{
			World world;

			JsonScopeParseHelper::SharedData sharedData;
			sharedData.SetScope(world);

			JsonScopeParseHelper helper;
			JsonParseMaster parser;

			parser.AddHelper(helper);
			parser.SetSharedData(sharedData);

			parser.ParseFromFile("Content/ActionListForEach.json");
			Entity* entity = world["Sectors"][0]["Entities"][0].As<Entity>();
			Assert::IsNotNull(entity);
			Assert::AreEqual(0, (*entity)["AuxIndex"].Get<int32_t>());
			for (size_t i = 0; i < 5; ++i)
			{
				Assert::AreEqual(static_cast<int32_t>(i+1), (*entity)["AuxOperand"].Get<int32_t>(i));
			}

			world.Update();
			Assert::AreEqual(4, (*entity)["AuxIndex"].Get<int32_t>());
			for (size_t i = 0; i < 5; ++i)
			{
				Assert::AreEqual(static_cast<int32_t>(i), (*entity)["AuxOperand"].Get<int32_t>(i));
			}
		}

	private:
		static _CrtMemState sStartMemState;

		ActionListForEachFactory actionListForEachFactory;
		ActionIncrementFactory actionIncrementFactory;
		ActionListFactory actionListFactory;
		SectorFactory sectorFactory;
		EntityFactory entityFactory;
	};
	_CrtMemState ActionListForEachTest::sStartMemState;
}