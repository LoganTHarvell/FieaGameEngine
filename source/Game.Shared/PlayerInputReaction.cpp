#include "pch.h"
#include "PlayerInputReaction.h"
#include "Event.h"
#include "PlayerInputMessage.h"
#include "Player1InputMessage.h"
#include "Player2InputMessage.h"
#include "Player3InputMessage.h"
#include "Player4InputMessage.h"
#include "Character.h"
#include "Sector.h"

using namespace Library;

namespace GameDesktop
{
#pragma region Static Members
	const TypeManager::TypeInfo& PlayerInputReaction::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ SpawnBombActionsKey, Types::Scope, true, 1, 0 },
				{ StopKickActionsKey, Types::Scope, true, 1, 0 },
				{ PunchActionsKey, Types::Scope, true, 1, 0 },
				{ MovementActionsKey, Types::Scope, true, 1, 0 },
			},

			ActionList::TypeIdClass()
		};

		return typeInfo;
	}
#pragma endregion Static Members

#pragma region Special Members
	PlayerInputReaction::PlayerInputReaction(const std::string& name) : Reaction(TypeIdClass(), name),
		mSpawnBombActions(mPairPtrs[SpawnBombActionsIndex]->second), mStopKickActions(mPairPtrs[StopKickActionsIndex]->second),
		mPunchActions(mPairPtrs[PunchActionsIndex]->second), mMovementActions(mPairPtrs[MovementActionsIndex]->second)
	{
	}

	PlayerInputReaction::PlayerInputReaction(const RTTI::IdType typeId, const std::string& name) : Reaction(typeId, name),
		mSpawnBombActions(mPairPtrs[SpawnBombActionsIndex]->second), mStopKickActions(mPairPtrs[StopKickActionsIndex]->second),
		mPunchActions(mPairPtrs[PunchActionsIndex]->second), mMovementActions(mPairPtrs[MovementActionsIndex]->second)
	{
	}
#pragma endregion Special Members

#pragma region Event Subscriber Overrrides
	void PlayerInputReaction::Notify(Library::EventPublisher& eventPublisher)
	{
		const PlayerInputMessage* message = nullptr;

		if (eventPublisher.Is(Event<Player1InputMessage>::TypeIdClass()))
		{
			Event<Player1InputMessage>* event = static_cast<Event<Player1InputMessage>*>(&eventPublisher);
			message = static_cast<const PlayerInputMessage*>(&event->Message);
		}
		else if (eventPublisher.Is(Event<Player2InputMessage>::TypeIdClass()))
		{
			Event<Player2InputMessage>* event = static_cast<Event<Player2InputMessage>*>(&eventPublisher);
			message = static_cast<const PlayerInputMessage*>(&event->Message);
		}
		else if (eventPublisher.Is(Event<Player3InputMessage>::TypeIdClass()))
		{
			Event<Player3InputMessage>* event = static_cast<Event<Player3InputMessage>*>(&eventPublisher);
			message = static_cast<const PlayerInputMessage*>(&event->Message);
		}
		else if (eventPublisher.Is(Event<Player4InputMessage>::TypeIdClass()))
		{
			Event<Player4InputMessage>* event = static_cast<Event<Player4InputMessage>*>(&eventPublisher);
			message = static_cast<const PlayerInputMessage*>(&event->Message);
		}

		if (message != nullptr)
		{

			Scope parameters;
			WorldState& worldState = message->GetWorld()->GetWorldState();

			assert(GetParent()->Is(Character::TypeIdClass()));
			worldState.Entity = static_cast<Character*>(GetParent());
			worldState.Sector = static_cast<Sector*>(GetParent()->GetParent());

			message->ForEachAuxiliary([&](const Attribute& attribute)
			{
				parameters[attribute.first] = attribute.second;
			});

			mParameterStack.Push(parameters);

			switch (message->GetType())
			{
			case PlayerInputMessage::MessageType::SpawnBomb:
				for (std::size_t i = 0; i < mSpawnBombActions.Size(); ++i)
				{
					assert(mSpawnBombActions[i].Is(Action::TypeIdClass()));
					worldState.Action = static_cast<Action*>(mSpawnBombActions.Get<Scope*>(i));
					worldState.Action->Update(worldState);
				}
				break;
			case PlayerInputMessage::MessageType::StopKick:
				for (std::size_t i = 0; i < mStopKickActions.Size(); ++i)
				{
					assert(mStopKickActions[i].Is(Action::TypeIdClass()));
					worldState.Action = static_cast<Action*>(mStopKickActions.Get<Scope*>(i));
					worldState.Action->Update(worldState);
				}
				break;
			case PlayerInputMessage::MessageType::Punch:
				for (std::size_t i = 0; i < mPunchActions.Size(); ++i)
				{
					assert(mPunchActions[i].Is(Action::TypeIdClass()));
					worldState.Action = static_cast<Action*>(mPunchActions.Get<Scope*>(i));
					worldState.Action->Update(worldState);
				}
				break;
			case PlayerInputMessage::MessageType::Movement:
				for (std::size_t i = 0; i < mMovementActions.Size(); ++i)
				{
					assert(mMovementActions[i].Is(Action::TypeIdClass()));
					worldState.Action = static_cast<Action*>(mMovementActions.Get<Scope*>(i));
					worldState.Action->Update(worldState);
				}
				break;
			}

			mParameterStack.Pop();
		}
	}
#pragma endregion Event Subscriber Overrrides
	
#pragma region Scope Overrides
	Library::Scope::Data* PlayerInputReaction::Find(const Key& key)
	{
		Data* result = nullptr;

		if (!mParameterStack.IsEmpty())
		{
			result = mParameterStack.Top().Find(key);
		}

		return result ? result : ActionList::Find(key);
	}
#pragma endregion Scope Overrides
}