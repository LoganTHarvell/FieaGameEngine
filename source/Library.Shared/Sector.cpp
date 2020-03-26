#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "Sector.h"

// First Party
#include "World.h"
#include "Entity.h"
#pragma endregion Includes

namespace Library
{
	const TypeManager::TypeInfo& Sector::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ NameKey, Types::String, false, 1, offsetof(Sector, mName) },
				{ EntitiesKey, Types::Scope, true, 1, 0 }
			},

			Attributed::TypeIdClass()
		};

		return typeInfo;
	}

	Sector::Sector(const std::string& name) : Attributed(TypeIdClass()), 
		mName(name), mEntities(mPairPtrs[EntitiesIndex]->second)
	{
	}

	gsl::owner<Library::Scope*> Sector::Clone() const
	{
		return new Sector(*this);
	}

	const std::string& Sector::Name() const
	{
		return mName;
	}

	void Sector::SetName(const std::string& name)
	{
		mName = name;
	}

	World* Sector::GetWorld() const
	{
		Scope* parent = GetParent();
		if (!parent) return nullptr;

		assert(parent->Is(World::TypeIdClass()));
		return static_cast<World*>(parent);
	}

	void Sector::SetWorld(World* world)
	{
		if (world == nullptr)
		{
			World* parent = GetWorld();
			if (parent) parent->Orphan(*this);
		}
		else
		{
			world->Adopt(*this, mName);
		}
	}

	Entity::DataType& Sector::Entities()
	{
		return mEntities;
	}

	const Entity::DataType& Sector::Entities() const
	{
		return mEntities;
	}

	Entity* Sector::CreateEntity(const std::string& className, const std::string& name)
	{
		Scope* newScope = Factory<Scope>::Create(className);		
		
		if (newScope)
		{
			assert(newScope->Is(Entity::TypeIdClass()));

			Entity* newEntity = static_cast<Entity*>(newScope);
			newEntity->SetName(name);
			
			Adopt(*newScope, "Entities");
			return newEntity;
		}

		return nullptr;
	}

	void Sector::Update(WorldState& worldState)
	{
		worldState.Sector = this;

		for (std::size_t i = 0; i < mEntities.Size(); ++i)
		{
			assert(mEntities[i].Is(Entity::TypeIdClass()));
			static_cast<Entity*>(mEntities.Get<Scope*>(i))->Update(worldState);
		}

		worldState.Sector = nullptr;
	}

	std::string Sector::ToString() const
	{
		std::ostringstream oss;
		oss << mName << " (Sector)";
		return oss.str();
	}
}