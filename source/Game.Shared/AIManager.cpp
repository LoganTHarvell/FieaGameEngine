#include "pch.h"
#include "AIManager.h"

#include "Event.h"
#include "EventQueue.h"
#include "Player1InputMessage.h"
#include "Player2InputMessage.h"
#include "Player3InputMessage.h"
#include "Player4InputMessage.h"

#include <chrono>
#include <ctime>

namespace GameDesktop
{
	GameDesktop::AIManager::AIManager(EInitState state, GameTime time)
		: mPlayersState(state)
	{
		ResetAITimeDelay(time);
	}

	void AIManager::PakRandomMoveInput(PlayerInputMessage* message)
	{
		bool YAxis = std::rand() % 2;
		if (YAxis)
		{
			(*message)["Y-Axis"] = (std::rand() % 3 - 1.0f);
			(*message)["X-Axis"] = 0.0f;
		}
		else
		{
			(*message)["X-Axis"] = (std::rand() % 3 - 1.0f);
			(*message)["Y-Axis"] = 0.0f;
		}
	}

	void AIManager::PakBomb(PlayerInputMessage* message)
	{
		message->SetType(PlayerInputMessage::MessageType::SpawnBomb);
	}

	void AIManager::UpdatePlayer1AI(WorldState& worldState)
	{
		{
			// Player 1
			Player1InputMessage message;
			message.SetWorld(worldState.World);
			EventQueue& eventQueue = *worldState.EventQueue;

			//Bombs
			{
				if (std::rand() % mBombSpawnRate == 0) // TODO: refactor, odds, real limits, timing, percent chance
				{
					std::shared_ptr<Event<Player1InputMessage>> event;
					message.SetType(Player1InputMessage::MessageType::SpawnBomb);
					event = std::make_shared<Event<Player1InputMessage>>(Event<Player1InputMessage>(message));
					eventQueue.Enqueue(event);
					return;
				}
			}

			// Movement
			{
				std::shared_ptr<Event<Player1InputMessage>> event;
				message.SetType(Player1InputMessage::MessageType::Movement);
				PakRandomMoveInput(&message);
				event = std::make_shared<Event<Player1InputMessage>>(Event<Player1InputMessage>(message));
				eventQueue.Enqueue(event);
				return;
			}
		}
	}

	void AIManager::UpdatePlayer2AI(WorldState& worldState)
	{
		{
			// Player 2
			Player2InputMessage message;
			message.SetWorld(worldState.World);
			EventQueue& eventQueue = *worldState.EventQueue;

			//Bombs
			{
				if (std::rand() % mBombSpawnRate == 0) // TODO: refactor, odds, real limits, timing, percent chance
				{
					std::shared_ptr<Event<Player2InputMessage>> event;
					message.SetType(Player2InputMessage::MessageType::SpawnBomb);
					event = std::make_shared<Event<Player2InputMessage>>(Event<Player2InputMessage>(message));
					eventQueue.Enqueue(event);
					return;
				}
			}

			// Movement
			{
				std::shared_ptr<Event<Player2InputMessage>> event;
				message.SetType(Player2InputMessage::MessageType::Movement);
				PakRandomMoveInput(&message);
				event = std::make_shared<Event<Player2InputMessage>>(Event<Player2InputMessage>(message));
				eventQueue.Enqueue(event);
				return;
			}
		}
	}

	void AIManager::UpdatePlayer3AI(WorldState& worldState)
	{
		{
			// Player 3
			Player3InputMessage message;
			message.SetWorld(worldState.World);
			EventQueue& eventQueue = *worldState.EventQueue;

			//Bombs
			{
				if (std::rand() % mBombSpawnRate == 0) // TODO: refactor, odds, real limits, timing, percent chance
				{
					std::shared_ptr<Event<Player3InputMessage>> event;
					message.SetType(Player3InputMessage::MessageType::SpawnBomb);
					event = std::make_shared<Event<Player3InputMessage>>(Event<Player3InputMessage>(message));
					eventQueue.Enqueue(event);
					return;
				}
			}

			// Movement
			{
				std::shared_ptr<Event<Player3InputMessage>> event;
				message.SetType(Player3InputMessage::MessageType::Movement);
				PakRandomMoveInput(&message);
				event = std::make_shared<Event<Player3InputMessage>>(Event<Player3InputMessage>(message));
				eventQueue.Enqueue(event);
				return;
			}
		}
	}
	
	void AIManager::UpdatePlayer4AI(WorldState& worldState)
	{
		{
			// Player 4
			Player4InputMessage message;
			message.SetWorld(worldState.World);
			EventQueue& eventQueue = *worldState.EventQueue;

			//Bombs
			{
				if (std::rand() % mBombSpawnRate == 0) // TODO: refactor, odds, real limits, timing, percent chance
				{
					std::shared_ptr<Event<Player4InputMessage>> event;
					message.SetType(Player4InputMessage::MessageType::SpawnBomb);
					event = std::make_shared<Event<Player4InputMessage>>(Event<Player4InputMessage>(message));
					eventQueue.Enqueue(event);
					return;
				}
			}

			// Movement
			{
				std::shared_ptr<Event<Player4InputMessage>> event;
				message.SetType(Player4InputMessage::MessageType::Movement);
				PakRandomMoveInput(&message);
				event = std::make_shared<Event<Player4InputMessage>>(Event<Player4InputMessage>(message));
				eventQueue.Enqueue(event);
				return;
			}
		}
	}

	void AIManager::UpdateAI(WorldState& worldState)
	{
		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		if (worldState.GameTime->CurrentTime() < (mStartTime + mInitialDelay))
			return;

		switch (mPlayersState)
		{
		case EInitState::NonePlayer:
			// Make four ai
			UpdatePlayer1AI(worldState);
			UpdatePlayer2AI(worldState);
			UpdatePlayer3AI(worldState);
			UpdatePlayer4AI(worldState);
			break;

		case EInitState::OnePlayer:
			// Make three ai
			UpdatePlayer2AI(worldState);
			UpdatePlayer3AI(worldState);
			UpdatePlayer4AI(worldState);
			break;

		case EInitState::TwoPlayer:
			// Make two ai
			UpdatePlayer3AI(worldState);
			UpdatePlayer4AI(worldState);
			break;

		case EInitState::ThreePlayer:
			// Make one ai
			UpdatePlayer4AI(worldState);
			break;

		default:
			break;
		}
	}

	 void AIManager::ResetAITimeDelay(GameTime gameTime)
	{
		mStartTime = gameTime.CurrentTime();
	}
}
