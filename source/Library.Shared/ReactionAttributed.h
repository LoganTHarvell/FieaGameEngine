#pragma once

#pragma region Includes
// First Party
#include "Reaction.h"
#include "Stack.h"
#pragma endregion Includes

namespace Library
{
	class ReactionAttributed final : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction)

#pragma region Type Definitions
	public:
		/// <summary>
		/// Type used to distinguish EventMessageAttributed instances.
		/// </summary>
		using Subtype = std::string;
#pragma endregion Type Definitions

#pragma region Static Members
	public:
		/// <summary>
		/// Key for the Subtype Attribute used to set the created EventMessageAttributed Subtype.
		/// </summary>
		inline static const std::string SubtypeKey = "Subtype";

	public:
		/// <summary>
		/// Getter for the class TypeInfo, used for registration with the TypeManager.
		/// </summary>
		static const TypeManager::TypeInfo& TypeInfo();
#pragma endregion Static Members

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="name">Pointer to the World that will process this message.</param>
		/// <param name="subtype">Tag specifying an EventMessageAttribute subtype to which an instance responds.</param>
		explicit ReactionAttributed(const std::string& name=std::string(), const Subtype& subtype=Subtype());

		/// <summary>
		/// Default destructor.
		/// </summary>
		virtual ~ReactionAttributed() override;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">ReactionAttributed to be copied.</param>
		ReactionAttributed(const ReactionAttributed& rhs);

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">ReactionAttributed to be copied.</param>
		/// <returns>Newly copied into left hand side ReactionAttributed.</returns>
		ReactionAttributed& operator=(const ReactionAttributed& rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">ReactionAttributed to be moved.</param>
		ReactionAttributed(ReactionAttributed&& rhs) noexcept;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">ReactionAttributed to be moved.</param>
		/// <returns>Newly moved into left hand side ReactionAttributed.</returns>
		ReactionAttributed& operator=(ReactionAttributed&& rhs) noexcept = default;
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the ReactionAttributed.</returns>
		virtual gsl::owner<Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor

#pragma region Event Subscriber Overrides
	public:
		/// <summary>
		/// Interface method called by an EventPublisher during Publish to receive the Event.
		/// </summary>
		/// <param name="eventPublisher">Reference to an Event as an EventPublisher.</param>
		virtual void Notify(EventPublisher& eventPublisher) override;
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
	private:
		/// <summary>
		/// Stack of Scopes maintaining the current set of auxiliary Attribute values from the 
		/// EventMessageAttributed instance passed by an Event at each call to Notify.
		/// These attributes act as a parameter list that is evaluated first during
		/// any Search call made during the ActionList Update call.
		/// </summary>
		Stack<Scope> mParameterStack;

		/// <summary>
		/// String to specify a EventMessageAttributed subtype to which this Reaction should respond.
		/// </summary>
		std::string mSubtype;

		/// <summary>
		/// Mutex controlling thread access to the ReactionAttributed.
		/// </summary>
		std::mutex mMutex;
#pragma endregion Data Members
	};

#pragma region Factory
	/// <summary>
	/// ReactionAttributedFactory class declaration.
	/// </summary>
	ConcreteFactory(ReactionAttributed, Scope)
#pragma endregion Factory
}