#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "Entity.h"

//First Party
#include "Sector.h"
#pragma endregion Includes

namespace Library
{
	const TypeManager::TypeInfo& Entity::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ "Name", Types::String, false, 1, offsetof(Entity, mName) }
			},

			Attributed::TypeIdClass()
		};

		return typeInfo;
	}

	Entity::Entity() : Attributed(TypeIdClass())
	{
	}

	gsl::owner<Library::Scope*> Entity::Clone() const
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

	const Sector* Entity::GetSector() const
	{
		const Scope* parent = GetParent();
		if (!parent) return nullptr;

		assert(parent->Is(Sector::TypeIdClass()));
		return static_cast<const Sector*>(parent);
	}

	void Entity::SetSector(Sector& sector)
	{
		sector.Adopt(*this, mName);
	}

	void Entity::Update(WorldState& worldState)
	{
		worldState.Entity = this;

		// TODO Action Update calls...

		worldState.Entity = nullptr;
	}

	std::string Entity::ToString() const
	{
		std::ostringstream oss;
		oss << mName << " (Entity)";
		return oss.str();
	}
}