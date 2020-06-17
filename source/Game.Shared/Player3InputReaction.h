#pragma once
#include "PlayerInputReaction.h"
#include "Factory.h"

namespace GameDesktop
{
	class Player3InputReaction final : public PlayerInputReaction
	{
		RTTI_DECLARATIONS(Player3InputReaction, PlayerInputReaction)

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="name">Pointer to the World that will process this message.</param>
		explicit Player3InputReaction(const std::string& name = std::string());

		/// <summary>
		/// Default destructor.
		/// </summary>
		~Player3InputReaction() override;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">Player3InputReaction to be copied.</param>
		Player3InputReaction(const Player3InputReaction& rhs);

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">Player3InputReaction to be copied.</param>
		/// <returns>Newly copied into left hand side Player3InputReaction.</returns>
		Player3InputReaction& operator=(const Player3InputReaction& rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">Player3InputReaction to be moved.</param>
		Player3InputReaction(Player3InputReaction && rhs) noexcept;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">Player3InputReaction to be moved.</param>
		/// <returns>Newly moved into left hand side Player3InputReaction.</returns>
		Player3InputReaction& operator=(Player3InputReaction && rhs) noexcept = default;
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the Player3InputReaction.</returns>
		virtual gsl::owner<Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor
	};

#pragma region Factory
	ConcreteFactory(Player3InputReaction, Library::Scope)
#pragma endregion Factory
}

