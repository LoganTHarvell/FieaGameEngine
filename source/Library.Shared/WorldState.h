#pragma once

namespace Library
{
	/// <summary>
	/// Handle for data that makes up the current state of a World during processing.
	/// </summary>
	struct WorldState final
	{	
		/// <summary>
		/// Handle to the current GameTime. May be null.
		/// </summary>
		class GameTime* GameTime{ nullptr };

		/// <summary>
		/// Handle to the current EventQueue. May be null.
		/// </summary>
		class EventQueue* EventQueue{ nullptr };

		/// <summary>
		/// Handle to the current World. May be null.
		/// </summary>
		class World* World{ nullptr };

		/// <summary>
		/// Handle to the current Sector. May be null.
		/// </summary>
		class Sector* Sector{ nullptr };

		/// <summary>
		/// Handle to the current Sector. May be null.
		/// </summary>
		class Entity* Entity{ nullptr };

		/// <summary>
		/// Handle to the current Action. May be null.
		/// </summary>
		class Action* Action{ nullptr };
	};
	
	/// <summary>
	/// Handle for data that makes up the current state of a World during processing.
	/// </summary>
	struct ConstWorldState final
	{	
		/// <summary>
		/// Handle to the current GameTime. May be null.
		/// </summary>
		const class GameTime* GameTime{ nullptr };

		/// <summary>
		/// Handle to the current EventQueue. May be null.
		/// </summary>
		const class EventQueue* EventQueue{ nullptr };

		/// <summary>
		/// Handle to the current World. May be null.
		/// </summary>
		const class World* World{ nullptr };

		/// <summary>
		/// Handle to the current Sector. May be null.
		/// </summary>
		const class Sector* Sector{ nullptr };

		/// <summary>
		/// Handle to the current Sector. May be null.
		/// </summary>
		const class Entity* Entity{ nullptr };

		/// <summary>
		/// Handle to the current Action. May be null.
		/// </summary>
		const class Action* Action{ nullptr };
	};
}

