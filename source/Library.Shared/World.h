#pragma once

#pragma region Includes
// First Party
#include "Entity.h"
#include "GameClock.h"
#include "WorldState.h"
#pragma endregion Includes

namespace Library
{
	/// <summary>
	/// Represents the global scope for a simulation, containing sectors and their entities.
	/// </summary>
	class World final : public Entity
	{
		RTTI_DECLARATIONS(World, Entity)

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="name">Name of the World.</param>
		/// <param name="gameTime">GameTime to be used in Update.</param>
		/// <param name="eventQueue">EventQueue to be used in Update.</param>
		explicit World(std::string name=std::string(), GameTime* gameTime=nullptr, class EventQueue* eventQueue=nullptr);

		/// <summary>
		/// Default destructor.
		/// </summary>
		~World() = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">World to be copied.</param>
		World(const World& rhs);

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">World to be copied.</param>
		/// <returns>Newly copied into left hand side World.</returns>
		World& operator=(const World& rhs);

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">World to be moved.</param>
		World(World&& rhs) noexcept;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">World to be moved.</param>
		/// <returns>Newly moved into left hand side World.</returns>
		World& operator=(World&& rhs) noexcept;
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the World.</returns>
		virtual gsl::owner<Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor

#pragma region Accessors
	public:
		/// <summary>
		/// Gets the WorldState associated with the World.
		/// </summary>
		/// <returns>Reference to the WorldState associated with the World.</returns>
		WorldState& GetWorldState();
		
		/// <summary>
		/// Gets the WorldState associated with the World.
		/// </summary>
		/// <returns>Reference to the WorldState associated with the World.</returns>
		ConstWorldState GetWorldState() const;
#pragma endregion Accessors

#pragma region Game Loop
	public:
		/// <summary>
		/// Runs the game loop of the World until Stop is called.
		/// </summary>
		void Run();

		/// <summary>
		/// Stops the game loop.
		/// </summary>
		void Stop();
	
		/// <summary>
		/// World initialize method to be called before running, hides inherited Entity Initialize.
		/// </summary>
		void Initialize();
	
		/// <summary>
		/// World update method to be called every frame, hides inherited Entity Update.
		/// </summary>
		void Update();
	
		/// <summary>
		/// World shutdown method to be called after running, hides inherited Entity Shutdown.
		/// </summary>
		void Shutdown();

	private:
		/// <summary>
		/// Hides unused Entity Update method.
		/// </summary>
		using Entity::Initialize;
		
		/// <summary>
		/// Hides unused Entity Update method.
		/// </summary>
		using Entity::Update;

		/// <summary>
		/// Hides unused Entity Update method.
		/// </summary>
		using Entity::Shutdown;
#pragma endregion Game Loop

#pragma region RTTI Overrides
	public:
		/// <summary>
		/// Virtual override for representing the World as a std::string.
		/// </summary>
		/// <returns>String representation of the World.</returns>
		virtual std::string ToString() const override;
#pragma endregion RTTI Overrides

#pragma region Data Members
	private:
		/// <summary>
		/// Clock that tracks the game start time, current time, and update delta time.
		/// </summary>
		GameClock mGameClock;

		/// <summary>
		/// Convenience struct for passing the WorldState data in cascaded Update calls.
		/// </summary>
		struct WorldState mWorldState;

		/// <summary>
		/// Represents whether the game loop is running.
		/// </summary>
		bool IsRunning{ false };
#pragma endregion Data Members
	};
}

