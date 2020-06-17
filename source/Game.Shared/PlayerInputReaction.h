#pragma once
#include "Reaction.h"
#include "Factory.h"
#include "Stack.h"

namespace GameDesktop
{
	class PlayerInputReaction : public Library::Reaction
	{
		RTTI_DECLARATIONS_ABSTRACT(PlayerInputReaction, Library::Reaction)

#pragma region Static Members
	public:
		/// <summary>
		/// Key for the SpawnBombActions Attribute used when a SpawnBomb input event is received.
		/// </summary>
		inline static const std::string SpawnBombActionsKey = "SpawnBombActions";

		/// <summary>
		/// Index of the SpawnBombActions Attribute in the PlayerInputReaction.
		/// </summary>
		inline static const size_t SpawnBombActionsIndex = 3;

		/// <summary>
		/// Key for the StopKickActions Attribute used when a StopKick input event is received.
		/// </summary>
		inline static const std::string StopKickActionsKey = "StopKickActions";

		/// <summary>
		/// Index of the StopKickActions Attribute in the PlayerInputReaction.
		/// </summary>
		inline static const size_t StopKickActionsIndex = 4;

		/// <summary>
		/// Key for the PunchActions Attribute used when a Punch input event is received.
		/// </summary>
		inline static const std::string PunchActionsKey = "PunchActions";

		/// <summary>
		/// Index of the PunchActions Attribute in the PlayerInputReaction.
		/// </summary>
		inline static const size_t PunchActionsIndex = 5;

		/// <summary>
		/// Key for the MovementActions Attribute used when a Movement input event is received.
		/// </summary>
		inline static const std::string MovementActionsKey = "MovementActions";

		/// <summary>
		/// Index of the MovementActions Attribute in the PlayerInputReaction.
		/// </summary>
		inline static const size_t MovementActionsIndex = 6;

		/// <summary>
		/// Key for the X-Axis auxilary attribute.
		/// </summary>
		inline static const std::string XAxisKey = "X-Axis";

		/// <summary>
		/// Key for the Y-Axis auxilary attribute.
		/// </summary>
		inline static const std::string YAxisKey = "Y-Axis";

	public:
		/// <summary>
		/// Getter for the class TypeInfo, used for registration with the TypeManager.
		/// </summary>
		static const Library::TypeManager::TypeInfo& TypeInfo();
#pragma endregion Static Members

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="name">Pointer to the World that will process this message.</param>
		explicit PlayerInputReaction(const std::string& name = std::string());

		/// <summary>
		/// Default destructor.
		/// </summary>
		virtual ~PlayerInputReaction() override = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">PlayerInputReaction to be copied.</param>
		PlayerInputReaction(const PlayerInputReaction& rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">PlayerInputReaction to be copied.</param>
		/// <returns>Newly copied into left hand side PlayerInputReaction.</returns>
		PlayerInputReaction& operator=(const PlayerInputReaction& rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">PlayerInputReaction to be moved.</param>
		PlayerInputReaction(PlayerInputReaction&& rhs) noexcept = default;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">PlayerInputReaction to be moved.</param>
		/// <returns>Newly moved into left hand side PlayerInputReaction.</returns>
		PlayerInputReaction& operator=(PlayerInputReaction&& rhs) noexcept = default;

	protected:
		/// <summary>
		/// Specialized constructor for use by derived classes to ensure correct Attribute population.
		/// </summary>
		/// <param name="typeId">Type ID of the derived class.</param>
		explicit PlayerInputReaction(const RTTI::IdType typeId, const std::string& name = std::string());
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the PlayerInputReaction.</returns>
		virtual gsl::owner<Scope*> Clone() const override = 0;
#pragma endregion Virtual Copy Constructor

#pragma region Event Subscriber Overrides
	public:
		/// <summary>
		/// Interface method called by an EventPublisher during Publish to receive the Event.
		/// </summary>
		/// <param name="eventPublisher">Reference to an Event as an EventPublisher.</param>
		virtual void Notify(Library::EventPublisher& eventPublisher);
#pragma endregion Event Subscriber Overrides

#pragma region Scope Overrides
	public:
		/// <summary>
		/// Override for the Scope Find method to first look in the parameter stack for an Attribute.
		/// Finds the Data value associated with the given Key value, if it exists.
		/// </summary>
		/// <param name="key">Key value associated with the Data value to be found.</param>
		/// <returns>If found, a pointer to the Data value. Otherwise, nullptr.</returns>
		virtual Data* Find(const Key& key) override;
#pragma endregion Scope Overrides

#pragma region Data Members
	public: //Hack: temp
		/// <summary>
		/// Stack of Scopes maintaining the current set of auxiliary Attribute values from the 
		/// PlayerInputMessage instance passed by an Event at each call to Notify.
		/// These attributes act as a parameter list that is evaluated first during
		/// any Search call made during the ActionList Update call.
		/// </summary>
		Library::Stack<Scope> mParameterStack;

		/// <summary>
		/// Collection of Action objects within the Actions prescribed Attribute.
		/// </summary>
		Data& mSpawnBombActions;
		/// <summary>
		/// Collection of Action objects within the Actions prescribed Attribute.
		/// </summary>
		Data& mStopKickActions;
		/// <summary>
		/// Collection of Action objects within the Actions prescribed Attribute.
		/// </summary>
		Data& mPunchActions;
		/// <summary>
		/// Collection of Action objects within the Actions prescribed Attribute.
		/// </summary>
		Data& mMovementActions;
#pragma endregion Data Members
	};
}

