#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "Entity.h"

//First Party
#include "Sector.h"
#include "Action.h"
#pragma endregion Includes

namespace Library
{
	const TypeManager::TypeInfo& Entity::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ NameKey, Types::String, false, 1, offsetof(Entity, mName) },
				{ ActionsKey, Types::Scope, true, 1, 0 }
			},

			Attributed::TypeIdClass()
		};

		return typeInfo;
	}

	Entity::Entity(const std::string& name) : Attributed(TypeIdClass()), 
		mName(name), mActions(mPairPtrs[ActionsIndex]->second)
	{
	}

	Entity::Entity(const RTTI::IdType typeId, const std::string& name) : Attributed(typeId), 
		mName(name), mActions(mPairPtrs[ActionsIndex]->second)
	{
	}

	gsl::owner<Scope*> Entity::Clone() const
	{
		return new Entity(*this);
	}

	const std::string& Entity::Name() const
	{
		return mName;
	}

	void Entity::SetName(const std::string& name)
	{
		mName = name;
	}

	Sector* Entity::GetSector() const
	{
		Scope* parent = GetParent();
		if (!parent) return nullptr;

		assert(parent->Is(Sector::TypeIdClass()));
		return static_cast<Sector*>(parent);
	}

	void Entity::SetSector(Sector* sector)
	{
		if (sector == nullptr)
		{
			Sector* parent = GetSector();
			if (parent) parent->Orphan(*this);
		}
		else
		{
			sector->Adopt(*this, mName);
		}
	}

	Entity::DataType& Entity::Actions()
	{
		return mActions;
	}

	const Entity::DataType& Entity::Actions() const
	{
		return mActions;
	}

	Action* Entity::CreateAction(const std::string& className, const std::string& name)
	{
		Scope* newScope = Factory<Scope>::Create(className);

		if (newScope)
		{
			assert(newScope->Is(Action::TypeIdClass()));

			Action* newAction = static_cast<Action*>(newScope);
			newAction->SetName(name);

			Adopt(*newScope, ActionsKey);
			return newAction;
		}

		return nullptr;
	}

	void Entity::Update(WorldState& worldState)
	{
		for (std::size_t i = 0; i < mActions.Size(); ++i)
		{
			assert(mActions[i].Is(Action::TypeIdClass()));

			worldState.Action = static_cast<Action*>(mActions.Get<Scope*>(i));
			worldState.Action->Update(worldState);
		}

		worldState.Action = nullptr;
	}

	std::string Entity::ToString() const
	{
		std::ostringstream oss;
		oss << mName << " (Entity)";
		return oss.str();
	}
}