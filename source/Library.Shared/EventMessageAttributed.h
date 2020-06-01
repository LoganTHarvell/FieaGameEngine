#pragma once

#pragma region Includes
// First Party
#include "Attributed.h"
#pragma endregion Includes

namespace Library
{
	// Forwarded Classes
	class World;

	class EventMessageAttributed final : public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed)

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
		/// Key value for the Subtype Attribute.
		/// </summary>
		inline static const Key SubtypeKey = "Subtype";

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
		/// <param name="world">Pointer to the World that will process this message.</param>
		/// <param name="subtype">Subtype identifier to further distinguish instances.</param>
		explicit EventMessageAttributed(World* world=nullptr, Subtype subtype=Subtype());

		/// <summary>
		/// Default destructor.
		/// </summary>
		virtual ~EventMessageAttributed() override = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">EventMessageAttributed to be copied.</param>
		EventMessageAttributed(const EventMessageAttributed& rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">EventMessageAttributed to be copied.</param>
		/// <returns>Newly copied into left hand side EventMessageAttributed.</returns>
		EventMessageAttributed& operator=(const EventMessageAttributed& rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">EventMessageAttributed to be moved.</param>
		EventMessageAttributed(EventMessageAttributed&& rhs) noexcept = default;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">EventMessageAttributed to be moved.</param>
		/// <returns>Newly moved into left hand side EventMessageAttributed.</returns>
		EventMessageAttributed& operator=(EventMessageAttributed&& rhs) noexcept = default;
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the EventMessageAttributed.</returns>
		virtual gsl::owner<Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor

#pragma region Accessors
		/// <summary>
		/// Gets the World that will process this message.
		/// </summary>
		/// <returns>Pointer to the World that will process this message.</returns>
		World* GetWorld() const;

		/// <summary>
		/// Gets the World that will process this message.
		/// </summary>
		/// <param name="world">World to be used to process this message.</param>
		void SetWorld(World* world);

		/// <summary>
		/// Gets the Subtype value of this message.
		/// </summary>
		/// <returns>Pointer to the World that will process this message.</returns>
		std::string& GetSubtype();

		/// <summary>
		/// Gets the Subtype value of this message.
		/// </summary>
		/// <returns>Pointer to the World that will process this message.</returns>
		const std::string& GetSubtype() const;

		/// <summary>
		/// Gets the Subtype value this message.
		/// </summary>
		/// <param name="subtype">World to be used to process this message.</param>
		void SetSubtype(const Key& subtype);
#pragma endregion Accessors

#pragma region Data Members
	private:
		/// <summary>
		/// Pointer to the World object that will process this message.
		/// </summary>
		World* mWorld{ nullptr };

		/// <summary>
		/// String used to distinguish between EventMessageAttributed instances.
		/// </summary>
		Subtype mSubtype;
#pragma endregion Data Members
	};
}

#include "EventMessageAttributed.inl"