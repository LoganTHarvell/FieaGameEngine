#pragma once
#include "PlayerInputReaction.h"
#include "Factory.h"

namespace GameDesktop
{
	class Player1InputReaction final : public PlayerInputReaction
	{
		RTTI_DECLARATIONS(Player1InputReaction, PlayerInputReaction)

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="name">Pointer to the World that will process this message.</param>
		explicit Player1InputReaction(const std::string& name = std::string());

		/// <summary>
		/// Default destructor.
		/// </summary>
		~Player1InputReaction() override;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">Player1InputReaction to be copied.</param>
		Player1InputReaction(const Player1InputReaction & rhs);

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">Player1InputReaction to be copied.</param>
		/// <returns>Newly copied into left hand side Player1InputReaction.</returns>
		Player1InputReaction& operator=(const Player1InputReaction & rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">Player1InputReaction to be moved.</param>
		Player1InputReaction(Player1InputReaction && rhs) noexcept;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">Player1InputReaction to be moved.</param>
		/// <returns>Newly moved into left hand side Player1InputReaction.</returns>
		Player1InputReaction& operator=(Player1InputReaction && rhs) noexcept = default;
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the Player1InputReaction.</returns>
		virtual gsl::owner<Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor
	};

#pragma region Factory
	ConcreteFactory(Player1InputReaction, Library::Scope)
#pragma endregion Factory
}

