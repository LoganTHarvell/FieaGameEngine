#include "pch.h"

#include "ToStringSpecialization.h"

#include "GameClock.h"
#include "GameTime.h"

using namespace std::string_literals;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace UnitTests;
using namespace Library;

namespace UtilityTests
{
	TEST_CLASS(GameClockAndTimeTest)
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

		TEST_METHOD(GameClockAndTime)
		{
			GameClock gameClock;
			GameTime gameTime;

			Assert::AreEqual(std::chrono::milliseconds(0), gameTime.TotalGameTime());
			Assert::AreEqual(std::chrono::milliseconds(0), gameTime.ElapsedGameTime());
			
			gameClock.UpdateGameTime(gameTime);
			gameClock.UpdateGameTime(gameTime);

			Assert::AreEqual(std::chrono::duration_cast<std::chrono::milliseconds>(gameClock.CurrentTime() - gameClock.StartTime()).count(), gameTime.TotalGameTime().count());
			Assert::AreEqual(std::chrono::duration_cast<std::chrono::milliseconds>(gameClock.CurrentTime() - gameClock.LastTime()).count(), gameTime.ElapsedGameTime().count());

			Assert::IsTrue(std::chrono::duration_cast<std::chrono::duration<float>>(gameClock.CurrentTime() - gameClock.StartTime()).count() - gameTime.TotalGameTimeSeconds().count() < 0.001);
			Assert::IsTrue(std::chrono::duration_cast<std::chrono::duration<float>>(gameClock.CurrentTime() - gameClock.LastTime()).count() - gameTime.ElapsedGameTimeSeconds().count() < 0.001);

			const GameClock constGameClock = gameClock;
			const GameTime constGameTime = gameTime;

			Assert::AreEqual(std::chrono::duration_cast<std::chrono::milliseconds>(constGameClock.CurrentTime()-constGameClock.StartTime()).count(), constGameTime.TotalGameTime().count());
			Assert::AreEqual(std::chrono::duration_cast<std::chrono::milliseconds>(constGameClock.CurrentTime()-constGameClock.LastTime()).count(), constGameTime.ElapsedGameTime().count());
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState GameClockAndTimeTest::sStartMemState;
}