#pragma once

#include "TypeManager.h"

#include "World.h"
#include "Sector.h"
#include "Entity.h"

#include "Action.h"
#include "ActionList.h"

#include "ActionIncrement.h"
#include "ActionCreate.h"
#include "ActionDestroy.h"
#include "ActionListWhile.h"
#include "ActionSpawnBomb.h"
#include "ActionStopKick.h"

#include "ReactionAttributed.h"
#include "ActionEvent.h"
#include "ActionListForEach.h"

#include "GameObject.h"
#include "Transform.h"
#include "ActionRender.h"
#include "RenderComponent.h"
#include "Menu.h"
#include "MenuDisplay.h"
#include "HUD.h"
#include "HUDNumber.h"

#include "ActionAnimate.h"
#include "ActionUpdateAnimationState.h"

#include "ActionPlaySound.h"

#include "Character.h"
#include "ActionPickupPowerUp.h"
#include "ActionCharacterMovementInput.h"

#include "MenuInputMessage.h"

#include "PlayerInputMessage.h"
#include "Player1InputMessage.h"
#include "Player2InputMessage.h"
#include "Player3InputMessage.h"
#include "Player4InputMessage.h"

#include "PlayerInputReaction.h"
#include "Player1InputReaction.h"
#include "Player2InputReaction.h"
#include "Player3InputReaction.h"
#include "Player4InputReaction.h"

#include "Bomb.h"
#include "PowerUp.h"

#include "ActionExpireEntity.h"
#include "DestructibleWall.h"
#include "Explosion.h"

using namespace Library;

namespace GameDesktop
{
	class GameTypes final
	{

	public:
		static void Create() { if (!sInstance) sInstance = new GameTypes(); }
		static void Destroy() { delete sInstance; sInstance = nullptr; };

	private:
		GameTypes()
		{
			if (!TypeManager::Instance())
			{
				throw std::runtime_error("TypeManager does not exist.");
			}
			
			RegisterType<World>();
			RegisterType<Sector>();
			RegisterType<Entity>();
			RegisterType<Map>();

			RegisterType<Action>();
			RegisterType<ActionList>();

			RegisterType<ActionIncrement>();
			RegisterType<ActionCreate>();
			RegisterType<ActionDestroy>();
			RegisterType<ActionListWhile>();
			RegisterType<ActionListForEach>();
			RegisterType<ActionSpawnBomb>();
			RegisterType<ActionStopKick>();

			RegisterType<Reaction>();
			RegisterType<ReactionAttributed>();
			RegisterType<ActionEvent>();
			RegisterType<EventMessageAttributed>();
			
			RegisterType<GameObject>();
			RegisterType<Transform>();
			RegisterType<ActionRender>();
			RegisterType<RenderComponent>();
			RegisterType<ActionAnimate>();
			RegisterType<ActionUpdateAnimationState>();
			RegisterType<ActionPlaySound>();
			RegisterType<Menu>();
			RegisterType<MenuDisplay>();
			RegisterType<HUD>();
			RegisterType<HUDNumber>();

			RegisterType<Bomb>();

			RegisterType<Character>();
			RegisterType<ActionPickupPowerUp>();
			RegisterType<ActionCharacterMovementInput>();

			RegisterType<MenuInputMessage>();

			RegisterType<PlayerInputMessage>();
			RegisterType<Player1InputMessage>();
			RegisterType<Player2InputMessage>();
			RegisterType<Player3InputMessage>();
			RegisterType<Player4InputMessage>();

			RegisterType<PlayerInputReaction>();
			RegisterType<Player1InputReaction>();
			RegisterType<Player2InputReaction>();
			RegisterType<Player3InputReaction>();
			RegisterType<Player4InputReaction>();

			RegisterType<ActionExpireEntity>();
			RegisterType<PowerUp>();
			RegisterType<DestructibleWall>();
			RegisterType<Explosion>();
		}
		
		~GameTypes()
		{
			TypeManager* typeManager = TypeManager::Instance();
			if (typeManager) typeManager->Clear();
		}

	public:
		GameTypes(const GameTypes&) = delete;
		GameTypes& operator=(const GameTypes&) = delete;
		GameTypes(GameTypes&&) = delete;
		GameTypes& operator=(GameTypes&&) = delete;

	private:
		inline static GameTypes* sInstance { nullptr };
		
#pragma region Factories
	private:
		SectorFactory SectorFactory;
		EntityFactory EntityFactory;

		ActionListFactory ActionListFactory;
		ActionIncrementFactory ActionIncrementFactory;
		ActionCreateFactory ActionCreateFactory;
		ActionDestroyFactory ActionDestroyFactory;
		ActionListWhileFactory ActionListWhileFactory;
		ActionListForEachFactory ActionListForEachFactory;

		ReactionAttributedFactory ReactionAttributedFactory;
		ActionEventFactory ActionEventFactory;

		GameObjectFactory GameObjectFactory;
		TransformFactory TransformFactory;
		ActionRenderFactory ActionRenderFactory;
		RenderComponentFactory RenderComponentFactory;
		MenuFactory MenuFactory;
		MenuDisplayFactory MenuDisplayFactory;
		HUDFactory HUDFactory;
		HUDNumberFactory HUDNumberFactory;
		ActionAnimateFactory ActionAnimateFactory;
		ActionUpdateAnimationStateFactory ActionUpdateAnimationStateFactory;
		ActionPlaySoundFactory ActionPlaySoundFactory;

		CharacterFactory CharacterFactory;
		ActionPickupPowerUpFactory ActionPickupPowerUpFactory;
		ActionCharacterMovementInputFactory ActionCharacterMovementInputFactory;

		Player1InputReactionFactory Player1InputReactionFactory;
		Player2InputReactionFactory Player2InputReactionFactory;
		Player3InputReactionFactory Player3InputReactionFactory;
		Player4InputReactionFactory Player4InputReactionFactory;
		ActionSpawnBombFactory actionSpawnBombFactory;
		PowerUpFactory PowerUpFactory;
		DestructibleWallFactory DestructibleWallFactory;
		ActionExpireEntityFactory ActionExpireEntityFactory;
		ExplosionFactory ExplosionFactory;
		ActionStopKickFactory actionStopKick;

#pragma endregion Factories
	};
}
