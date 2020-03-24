#pragma once

#pragma region Includes
// First Party
#include "Attributed.h"
#include "WorldState.h"
#include "Factory.h"
#pragma endregion Includes

namespace Library
{
#pragma region Forward Class Declarations
	class Sector;
#pragma endregion Forward Class Declarations

	/// <summary>
	/// Represents a base object within the reflection system.
	/// </summary>
	class Entity : public Attributed
	{
		RTTI_DECLARATIONS(Entity, Attributed)

#pragma region TypeInfo
	public:
		/// <summary>
		/// Getter for the class TypeInfo, used for registration with the TypeManager.
		/// </summary>
		static const TypeManager::TypeInfo& TypeInfo();
#pragma endregion TypeInfo

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Entity();

		/// <summary>
		/// Default destructor.
		/// </summary>
		~Entity() = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">Entity to be copied.</param>
		Entity(const Entity& rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">Entity to be copied.</param>
		/// <returns>Newly copied into left hand side Entity.</returns>
		Entity& operator=(const Entity& rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">Entity to be moved.</param>
		Entity(Entity&& rhs) = default;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">Entity to be moved.</param>
		/// <returns>Newly moved into left hand side Entity.</returns>
		Entity& operator=(Entity&& rhs) = default;

	protected:
		/// <summary>
		/// Specialized constructor for use by derived classes to ensure correct attribute population.
		/// </summary>
		/// <param name="typeId">Type ID of the derived class.</param>
		explicit Entity(const RTTI::IdType typeId);
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the Entity.</returns>
		virtual gsl::owner<Library::Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor

#pragma region Accessors
	public:
		/// <summary>
		/// Name of the Entity.
		/// </summary>
		/// <returns>Name of the Entity as a std::string.</returns>
		const std::string& Name() const;

		/// <summary>
		/// Sets the name of the Entity.
		/// </summary>
		/// <param name="name">String to use as the name of the Entity.</param>
		void SetName(const std::string& name);

		/// <summary>
		/// Gets the Sector that owns this Entity.
		/// </summary>
		/// <returns>Pointer to the Sector that owns this Entity.</returns>
		const Sector* GetSector() const;

		/// <summary>
		/// Sets the Sector that owns this Entity.
		/// </summary>
		/// <param name="sector">Sector to adopt this Entity.</param>
		void SetSector(Sector& sector);
#pragma endregion Accessors

#pragma region Game Loop
	public:
		/// <summary>
		/// Virtual update method to be called every frame.
		/// </summary>
		/// <param name="worldState">WorldState context for the current processing step.</param>
		virtual void Update(WorldState& worldState);
#pragma endregion Game Loop

#pragma region RTTI Overrides
	public:
		/// <summary>
		/// Virtual override for representing the Entity as a std::string.
		/// </summary>
		/// <returns></returns>
		virtual std::string ToString() const override;
#pragma endregion RTTI Overrides

#pragma region Data Members
	private:
		/// <summary>
		/// Name of the Entity, reflected as a prescribed attribute.
		/// </summary>
		std::string mName;
#pragma endregion Data Members
	};

#pragma region Factory
	/// <summary>
	/// EntityFactory class declaration.
	/// </summary>
	ConcreteFactory(Entity, Scope)
#pragma endregion Factory
}