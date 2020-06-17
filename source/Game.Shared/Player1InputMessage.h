#pragma once
#include "PlayerInputMessage.h"

namespace GameDesktop
{
	class Player1InputMessage : public PlayerInputMessage
	{
		RTTI_DECLARATIONS(Player1InputMessage, PlayerInputMessage)

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="type">The enum type that represents the kind of player input.</param>
		explicit Player1InputMessage(MessageType type = MessageType::Unknown);

		/// <summary>
		/// Default destructor.
		/// </summary>
		virtual ~Player1InputMessage() = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">Player1InputMessage to be copied.</param>
		Player1InputMessage(const Player1InputMessage & rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">Player1InputMessage to be copied.</param>
		/// <returns>Newly copied into left hand side Player1InputMessage.</returns>
		Player1InputMessage& operator=(const Player1InputMessage & rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">Player1InputMessage to be moved.</param>
		Player1InputMessage(Player1InputMessage && rhs) = default;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">Player1InputMessage to be moved.</param>
		/// <returns>Newly moved into left hand side Player1InputMessage.</returns>
		Player1InputMessage& operator=(Player1InputMessage && rhs) = default;

#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the Player1InputMessage</returns>
		virtual gsl::owner<Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor
	};
}

