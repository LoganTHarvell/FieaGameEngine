#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "World.h"

// First Party
#include "Sector.h"
#pragma endregion Includes

namespace Library
{
	const TypeManager::TypeInfo& World::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ "Name", Types::String, false, 1, offsetof(World, mName) },
				{ "Sectors", Types::Scope, true, 0, 0 }
			},

			Attributed::TypeIdClass()
		};

		return typeInfo;
	}

	World::World() : Attributed(TypeIdClass()), mSectors(mPairPtrs[2]->second)
	{
	}

	gsl::owner<Library::Scope*> World::Clone() const
	{
		return new World(*this);
	}

	const std::string& World::Name() const
	{
		return mName;
	}

	void World::SetName(const std::string& name)
	{
		mName = name;
	}

	Sector::DataType& World::Sectors()
	{
		return mSectors;
	}

	const Sector::DataType& World::Sectors() const
	{
		return mSectors;
	}

	Sector* World::CreateSector(const std::string& className, const std::string& name)
	{
		Scope* newScope = Factory<Scope>::Create(className);

		if (newScope)
		{
			assert(newScope->Is(Sector::TypeIdClass()));

			Sector* newEntity = static_cast<Sector*>(newScope);
			newEntity->SetName(name);

			Adopt(*newScope, "Sectors");
			return newEntity;
		}

		return nullptr;
	}

	void World::Update(WorldState& worldState)
	{
		GameTime currentGameTime = worldState.GetGameTime();
		mGameClock.UpdateGameTime(currentGameTime);
		worldState.SetGameTime(currentGameTime);

		for (std::size_t i = 0; i < mSectors.Size(); ++i)
		{
			assert(mSectors[i].Is(Sector::TypeIdClass()));
			static_cast<Sector*>(mSectors.Get<Scope*>(i))->Update(worldState);
		}
	}

	std::string World::ToString() const
	{
		std::ostringstream oss;
		oss << mName << " (World)";
		return oss.str();
	}
}