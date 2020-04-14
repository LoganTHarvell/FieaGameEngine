#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "World.h"

// First Party
#include "Sector.h"
#include "EventQueue.h"
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

	World::World(const std::string& name, GameTime* gameTime, EventQueue* eventQueue) : Attributed(TypeIdClass()), 
		mName(name), mSectors(mPairPtrs[SectorsIndex]->second)
	{
		mWorldState.World = this;
		mWorldState.GameTime = gameTime;
		mWorldState.EventQueue = eventQueue;
	}

	World::World(World&& rhs) noexcept : Attributed(std::move(rhs)),
		mName(rhs.mName), mSectors(mPairPtrs[SectorsIndex]->second)
	{
		mWorldState.World = this;
		mWorldState.GameTime = rhs.mWorldState.GameTime;
		mWorldState.EventQueue = rhs.mWorldState.EventQueue;

		mName = std::move(rhs.mName);

		rhs.mWorldState.GameTime = nullptr;
		rhs.mWorldState.EventQueue = nullptr;
	}

	World& World::operator=(World&& rhs) noexcept
	{
		Attributed::operator=(std::move(rhs));
		
		mWorldState.World = this;
		mWorldState.GameTime = rhs.mWorldState.GameTime;
		mWorldState.EventQueue = rhs.mWorldState.EventQueue;

		mName = std::move(rhs.mName);

		rhs.mName.clear();
		rhs.mWorldState.GameTime = nullptr;
		rhs.mWorldState.EventQueue = nullptr;

		return *this;
	}

	gsl::owner<Scope*> World::Clone() const
	{
		return new World(*this);
	}

	WorldState& World::GetWorldState()
	{
		return mWorldState;
	}

	const WorldState& World::GetWorldState() const
	{
		return mWorldState;
	}

	World::PendingChildList& World::PendingChildren()
	{
		return mPendingChildren;
	}

	const World::PendingChildList& World::PendingChildren() const
	{
		return mPendingChildren;
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

		Adopt(*newSector, SectorsKey);
		return *newSector;
	}

	void World::Update()
	{
		if (mWorldState.GameTime)
		{
			mGameClock.UpdateGameTime(*mWorldState.GameTime);
		
			if (mWorldState.EventQueue)
			{
				mWorldState.EventQueue->Update(*mWorldState.GameTime);
			}
		}

		for (std::size_t i = 0; i < mSectors.Size(); ++i)
		{
			assert(mSectors[i].Is(Sector::TypeIdClass()));

			mWorldState.Sector = static_cast<Sector*>(mSectors.Get<Scope*>(i));
			mWorldState.Sector->Update(mWorldState);
		}

		mWorldState.Sector = nullptr;

		UpdatePendingChildren();
	}

	std::string World::ToString() const
	{
		std::ostringstream oss;
		oss << mName << " (World)";
		return oss.str();
	}

	void World::UpdatePendingChildren()
	{
		for (PendingChild& pendingChild : mPendingChildren)
		{
			switch (pendingChild.ChildState)
			{
			case PendingChild::State::ToAdd:
				pendingChild.Target.Adopt(pendingChild.Child, *pendingChild.AttributeKey);
				break;

			case PendingChild::State::ToRemove:
				delete pendingChild.Target.Orphan(pendingChild.Child);
				break;

			default:
				break;
			}
		}

		mPendingChildren.Clear();
	}
}