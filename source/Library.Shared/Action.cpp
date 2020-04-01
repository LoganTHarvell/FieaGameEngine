#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "Action.h"

//First Party
#include "Entity.h"
#pragma endregion Includes

namespace Library
{
	const TypeManager::TypeInfo& Action::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ NameKey, Types::String, false, 1, offsetof(Action, mName) }
			},

			Attributed::TypeIdClass()
		};

		return typeInfo;
	}

	Action::Action(const RTTI::IdType typeId, const std::string& name) : Attributed(typeId), mName(name)
	{
	}

	const std::string& Action::Name() const
	{
		return mName;
	}

	void Action::SetName(const std::string& name)
	{
		mName = name;
	}

	Entity* Action::GetEntity() const
	{
		Scope* parent = GetParent();
		if (!parent) return nullptr;

		assert(parent->Is(Entity::TypeIdClass()));
		return static_cast<Entity*>(parent);
	}

	void Action::SetEntity(Entity* sector)
	{
		if (sector == nullptr)
		{
			Entity* parent = GetEntity();
			if (parent) parent->Orphan(*this);
		}
		else
		{
			sector->Adopt(*this, mName);
		}
	}

	std::string Action::ToString() const
	{
		std::ostringstream oss;
		oss << mName << " (Action)";
		return oss.str();
	}
}