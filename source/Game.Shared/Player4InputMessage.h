#pragma once
#include "PlayerInputMessage.h"

namespace GameDesktop
{
	class Player4InputMessage : public PlayerInputMessage
	{
		RTTI_DECLARATIONS(Player4InputMessage, PlayerInputMessage)

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="type">The enum type that represents the kind of player input.</param>
		explicit Player4InputMessage(MessageType type = MessageType::Unknown);

		/// <summary>
		/// Default destructor.
		/// </summary>
		virtual ~Player4InputMessage() = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">Player4InputMessage to be copied.</param>
		Player4InputMessage(const Player4InputMessage & rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">Player4InputMessage to be copied.</param>
		/// <returns>Newly copied into left hand side Player4InputMessage.</returns>
		Player4InputMessage& operator=(const Player4InputMessage & rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">Player4InputMessage to be moved.</param>
		Player4InputMessage(Player4InputMessage && rhs) = default;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">Player4InputMessage to be moved.</param>
		/// <returns>Newly moved into left hand side Player4InputMessage.</returns>
		Player4InputMessage& operator=(Player4InputMessage && rhs) = default;

#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the Player4InputMessage</returns>
		virtual gsl::owner<Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor
	};
}

