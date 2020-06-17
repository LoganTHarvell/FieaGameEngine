#pragma once
#include "PlayerInputReaction.h"
#include "Factory.h"

namespace GameDesktop
{
	class Player2InputReaction final: public PlayerInputReaction
	{
		RTTI_DECLARATIONS(Player2InputReaction, PlayerInputReaction)

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="name">Pointer to the World that will process this message.</param>
		explicit Player2InputReaction(const std::string& name = std::string());

		/// <summary>
		/// Default destructor.
		/// </summary>
		~Player2InputReaction() override;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">Player2InputReaction to be copied.</param>
		Player2InputReaction(const Player2InputReaction& rhs);

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">Player2InputReaction to be copied.</param>
		/// <returns>Newly copied into left hand side Player2InputReaction.</returns>
		Player2InputReaction& operator=(const Player2InputReaction& rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">Player2InputReaction to be moved.</param>
		Player2InputReaction(Player2InputReaction && rhs) noexcept;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">Player2InputReaction to be moved.</param>
		/// <returns>Newly moved into left hand side Player2InputReaction.</returns>
		Player2InputReaction& operator=(Player2InputReaction && rhs) noexcept = default;
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the Player2InputReaction.</returns>
		virtual gsl::owner<Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor
	};

#pragma region Factory
	ConcreteFactory(Player2InputReaction, Library::Scope)
#pragma endregion Factory
}

