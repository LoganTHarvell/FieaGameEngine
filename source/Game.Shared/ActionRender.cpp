#include "pch.h"
#include "ActionRender.h"

#include "Character.h"

namespace GameDesktop
{
	const Library::TypeManager::TypeInfo& ActionRender::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{IsEnabledKey, Types::Integer, false, 1, offsetof(ActionRender, mIsEnabled)},
				{PriorityKey, Types::Integer, false, 1, offsetof(ActionRender, mPriority)}
			}, 
			Action::TypeIdClass()
		};
		return typeInfo;
	}


	gsl::owner<ActionRender*> ActionRender::Clone() const
	{
		return new ActionRender(*this);
	}
	void ActionRender::Update(WorldState& worldState)
	{
		if (!mIsEnabled)
		{
			return;
		}
		RTTI* rtti = worldState.World->operator[](Game::GameKey).Get<RTTI*>();
		assert(rtti->Is(Game::TypeIdClass()));
		Game* game = static_cast<Game*>(rtti);

		const Entity* entity = GetEntity();
		assert(entity->Is(GameObject::TypeIdClass()));
		const GameObject* gameObject = static_cast<const GameObject*>(entity);

		if (gameObject->Is(Character::TypeIdClass()))
		{
			game->GetRenderQueue().Insert(gameObject, static_cast<int>(gameObject->GetTransform().GetLocation().y));
		}
		else
		{
			game->GetRenderQueue().Insert(gameObject, mPriority);
		}
	}

	std::string ActionRender::ToString() const
	{
		std::ostringstream oss;
		oss << Name() << " (ActionRender)";
		return oss.str();
	}

	ActionRender::ActionRender(const std::string& name /*= std::string()*/, int32_t priority) : Action(TypeIdClass(), name), mPriority(priority)
	{
	}

}