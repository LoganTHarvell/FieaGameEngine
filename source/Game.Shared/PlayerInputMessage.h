#pragma once
#include "Attributed.h"
#include "World.h"

namespace GameDesktop
{
	class PlayerInputMessage : public Library::Attributed
	{
		RTTI_DECLARATIONS(PlayerInputMessage, Attributed)

#pragma region Type Definitions
	public:
		enum class MessageType
		{
			Unknown = -1,

			SpawnBomb,
			StopKick,
			Punch,
			Movement,
			
			End
		};
#pragma endregion Type Definitions

#pragma region Static Members
	public: 
		static const Library::TypeManager::TypeInfo& TypeInfo();
#pragma endregion Static Members

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="type">The enum type that represents the kind of player input.</param>
		explicit PlayerInputMessage(MessageType type = MessageType::Unknown);

		/// <summary>
		/// Default destructor.
		/// </summary>
		virtual ~PlayerInputMessage() = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">PlayerInputMessage to be copied.</param>
		PlayerInputMessage(const PlayerInputMessage& rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">PlayerInputMessage to be copied.</param>
		/// <returns>Newly copied into left hand side PlayerInputMessage.</returns>
		PlayerInputMessage& operator=(const PlayerInputMessage& rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">PlayerInputMessage to be moved.</param>
		PlayerInputMessage(PlayerInputMessage&& rhs) = default;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">PlayerInputMessage to be moved.</param>
		/// <returns>Newly moved into left hand side PlayerInputMessage.</returns>
		PlayerInputMessage& operator=(PlayerInputMessage&& rhs) = default;

#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the PlayerInputMessage</returns>
		virtual gsl::owner<Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor

#pragma region Accessors
	public:
		/// <summary>
		/// Gets the type of the message.
		/// </summary>
		/// <returns>A reference to the enum value that represents this message type.</returns>
		MessageType GetType();

		/// <summary>
		/// Gets the type of the message.
		/// </summary>
		/// <returns>A const reference to the enum value that represents this message type.</returns>
		const MessageType GetType() const;

		/// <summary>
		/// Sets the message type.
		/// </summary>
		/// <param name="type">The enum value that represents this messages type.</param>
		void SetType(MessageType type);

		/// <summary>
		/// Gets the World that will process this message.
		/// </summary>
		/// <returns>Pointer to the World that will process this message.</returns>
		Library::World* GetWorld() const;

		/// <summary>
		/// Gets the World that will process this message.
		/// </summary>
		/// <param name="world">World to be used to process this message.</param>
		void SetWorld(Library::World* world);
#pragma endregion Accessors

#pragma region Data Members
	private:
		/// <summary>
		/// Enum value used to determine what kind of event message this is.
		/// </summary>
		MessageType mType{ MessageType::Unknown };

		/// <summary>
		/// Pointer to the World object that will process this message.
		/// </summary>
		Library::World* mWorld{ nullptr };
#pragma endregion Data Members
	};
}

#include "PlayerInputMessage.inl"

