#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "World.h"

// Standard
#include <utility>

// First Party
#include "Entity.h"
#include "EventQueue.h"
#pragma endregion Includes

namespace Library
{
	World::World(std::string name, GameTime* gameTime, EventQueue* eventQueue) : Entity(TypeIdClass(), std::move(name))
	{
		mWorldState.World = this;
		mWorldState.GameTime = gameTime;
		mWorldState.EventQueue = eventQueue;
	}

	World::World(const World& rhs) : Entity(rhs),
		mGameClock(rhs.mGameClock)
	{
			mWorldState.World = this;
			mWorldState.GameTime = rhs.mWorldState.GameTime;
			mWorldState.EventQueue = rhs.mWorldState.EventQueue;
	}

	World& World::operator=(const World& rhs)
	{
		if (this != &rhs)
		{
			Entity::operator=(rhs);

			mGameClock = rhs.mGameClock;
			mWorldState.GameTime = rhs.mWorldState.GameTime;
			mWorldState.EventQueue = rhs.mWorldState.EventQueue;
		}
		
		return *this;
	}
	
	World::World(World&& rhs) noexcept : Entity(std::move(rhs))
	{
		mWorldState.World = this;
		mWorldState.GameTime = rhs.mWorldState.GameTime;
		mWorldState.EventQueue = rhs.mWorldState.EventQueue;
		rhs.mWorldState.GameTime = nullptr;
		rhs.mWorldState.EventQueue = nullptr;
	}

	World& World::operator=(World&& rhs) noexcept
	{
		mWorldState.GameTime = rhs.mWorldState.GameTime;
		mWorldState.EventQueue = rhs.mWorldState.EventQueue;

		rhs.mWorldState.GameTime = nullptr;
		rhs.mWorldState.EventQueue = nullptr;

		Entity::operator=(std::move(rhs));

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

	ConstWorldState World::GetWorldState() const
	{
		return
		{
			mWorldState.ContentManager,
			mWorldState.RenderingManager,
			mWorldState.GameTime,
			mWorldState.EventQueue,
			mWorldState.World,
			mWorldState.Sector,
			mWorldState.Entity
		};
	}

	void World::Run()
	{
		IsRunning = true;
		
		while (IsRunning)
		{
			Update();
		}
	}

	void World::Stop()
	{
		IsRunning = false;
	}

	void World::Initialize()
	{
		mGameClock.Reset();
		
		ForEachChild([this](Entity& sector)
		{
			mWorldState.Sector = &sector;
			mWorldState.Sector->Initialize(mWorldState);
		});

		mWorldState.Sector = nullptr;

		UpdatePendingChildren();
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

		ForEachChild([this](Entity& sector)
		{
			mWorldState.Sector = &sector;
			mWorldState.Sector->Update(mWorldState);
		});

		mWorldState.Sector = nullptr;

		UpdatePendingChildren();
	}

	void World::Shutdown()
	{		
		if (mWorldState.EventQueue)
		{
			mWorldState.EventQueue->Clear();
		}
		
		ForEachChild([this](Entity& sector)
		{
			mWorldState.Sector = &sector;
			mWorldState.Sector->Shutdown(mWorldState);
		});

		mWorldState.Sector = nullptr;

		UpdatePendingChildren();
	}

	std::string World::ToString() const
	{
		std::ostringstream oss;
		oss << Name() << " (World)";
		return oss.str();
	}
}