#pragma once
#include "PlayerInputReaction.h"
#include "Factory.h"

namespace GameDesktop
{
	class Player4InputReaction final : public PlayerInputReaction
	{
		RTTI_DECLARATIONS(Player4InputReaction, PlayerInputReaction)

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="name">Pointer to the World that will process this message.</param>
		explicit Player4InputReaction(const std::string& name = std::string());

		/// <summary>
		/// Default destructor.
		/// </summary>
		~Player4InputReaction() override;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">Player4InputReaction to be copied.</param>
		Player4InputReaction(const Player4InputReaction& rhs);

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">Player4InputReaction to be copied.</param>
		/// <returns>Newly copied into left hand side Player4InputReaction.</returns>
		Player4InputReaction& operator=(const Player4InputReaction& rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">Player4InputReaction to be moved.</param>
		Player4InputReaction(Player4InputReaction && rhs) noexcept;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">Player4InputReaction to be moved.</param>
		/// <returns>Newly moved into left hand side Player4InputReaction.</returns>
		Player4InputReaction& operator=(Player4InputReaction && rhs) noexcept = default;
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the Player4InputReaction.</returns>
		virtual gsl::owner<Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor
	};

#pragma region Factory
	ConcreteFactory(Player4InputReaction, Library::Scope)
#pragma endregion Factory
}

