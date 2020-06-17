#pragma once
#include "PlayerInputMessage.h"

namespace GameDesktop
{
	class Player2InputMessage : public PlayerInputMessage
	{
		RTTI_DECLARATIONS(Player2InputMessage, PlayerInputMessage)

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="type">The enum type that represents the kind of player input.</param>
		explicit Player2InputMessage(MessageType type = MessageType::Unknown);

		/// <summary>
		/// Default destructor.
		/// </summary>
		virtual ~Player2InputMessage() = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">Player2InputMessage to be copied.</param>
		Player2InputMessage(const Player2InputMessage & rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">Player2InputMessage to be copied.</param>
		/// <returns>Newly copied into left hand side Player2InputMessage.</returns>
		Player2InputMessage& operator=(const Player2InputMessage & rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">Player2InputMessage to be moved.</param>
		Player2InputMessage(Player2InputMessage && rhs) = default;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">Player2InputMessage to be moved.</param>
		/// <returns>Newly moved into left hand side Player2InputMessage.</returns>
		Player2InputMessage& operator=(Player2InputMessage && rhs) = default;

#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the Player2InputMessage</returns>
		virtual gsl::owner<Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor
	};
}

