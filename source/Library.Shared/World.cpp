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
				{ NameKey, Types::String, false, 1, offsetof(World, mName) },
				{ SectorsKey, Types::Scope, true, 1, 0 }
			},

			Attributed::TypeIdClass()
		};

		return typeInfo;
	}

	World::World(const std::string& name) : Attributed(TypeIdClass()), 
		mName(name), mSectors(mPairPtrs[SectorsIndex]->second)
	{
		mWorldState.World = this;
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

	Sector& World::CreateSector(const std::string& name)
	{
		Sector* newSector = new Sector();
		assert(newSector);

		newSector->SetName(name);

		Adopt(*newSector, "Sectors");
		return *newSector;
	}

	void World::Update()
	{
		GameTime currentGameTime = mWorldState.GetGameTime();
		mGameClock.UpdateGameTime(currentGameTime);
		mWorldState.SetGameTime(currentGameTime);

		for (std::size_t i = 0; i < mSectors.Size(); ++i)
		{
			assert(mSectors[i].Is(Sector::TypeIdClass()));

			mWorldState.Sector = static_cast<Sector*>(mSectors.Get<Scope*>(i));
			mWorldState.Sector->Update(mWorldState);
		}

		mWorldState.Sector = nullptr;
	}

	std::string World::ToString() const
	{
		std::ostringstream oss;
		oss << mName << " (World)";
		return oss.str();
	}
}