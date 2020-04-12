#pragma once

#pragma region Includes
// First Party
#include "Attributed.h"
#include "WorldState.h"
#include "Factory.h"
#pragma endregion Includes

namespace Library
{
	// Forwarded Classes
	class World;
	class Entity;

	/// <summary>
	/// Represents a contained collection of Entity objects, analogous to a package or level.
	/// </summary>
	class Sector final : public Attributed
	{
		RTTI_DECLARATIONS(Sector, Attributed)

#pragma region Static Members
	public:
		/// <summary>
		/// Key for the Name Attribute in the Sector.
		/// </summary>
		inline static const KeyType NameKey = "Name";

		/// <summary>
		/// Key for the Entities Attribute in the Sector.
		/// </summary>
		inline static const KeyType EntitiesKey = "Entities";

		/// <summary>
		/// Index of the Entities Attribute in the Sector.
		/// </summary>
		inline static const std::size_t EntitiesIndex = 2;

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
		explicit Sector(const std::string& name=std::string());

		/// <summary>
		/// Default destructor.
		/// </summary>
		~Sector() = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">Sector to be copied.</param>
		Sector(const Sector & rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">Sector to be copied.</param>
		/// <returns>Newly copied into left hand side Sector.</returns>
		Sector& operator=(const Sector & rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">Sector to be moved.</param>
		Sector(Sector && rhs) noexcept = default;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">Sector to be moved.</param>
		/// <returns>Newly moved into left hand side Sector.</returns>
		Sector& operator=(Sector && rhs) noexcept = default;
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the Sector.</returns>
		virtual gsl::owner<Library::Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor

#pragma region Accessors
	public:
		/// <summary>
		/// Name of the Sector.
		/// </summary>
		/// <returns>Name of the Sector as a std::string.</returns>
		const std::string& Name() const;

		/// <summary>
		/// Sets the name of the Sector.
		/// </summary>
		/// <param name="name">String to use as the name of the Sector.</param>
		void SetName(const std::string& name);

		/// <summary>
		/// Gets the World that owns this Sector.
		/// </summary>
		/// <returns>Pointer to the World that owns this Sector.</returns>
		World* GetWorld() const;

		/// <summary>
		/// Sets the World that owns this Sector.
		/// </summary>
		/// <param name="world">World to adopt this Sector.</param>
		void SetWorld(World* world);

		/// <summary>
		/// Gets the data handle to the Entity objects contained in this Sector.
		/// </summary>
		/// <returns>Reference to the Entity objects.</returns>
		DataType& Entities();

		/// <summary>
		/// Gets the data handle to the Entity objects contained in this Sector.
		/// </summary>
		/// <returns>Reference to the Entity objects.</returns>
		const DataType& Entities() const;

		/// <summary>
		/// Generates an Entity class and adopts it into this Sector.
		/// </summary>
		/// <param name="className">Class name of an Entity or Entity subclass to be instantiated.</param>
		/// <param name="name">Name of the newly created Entity.</param>
		/// <returns>Reference to the newly heap allocated Entity.</returns>
		Entity* CreateEntity(const std::string& className, const std::string& name);
#pragma endregion Accessors

#pragma region Game Loop
	public:
		/// <summary>
		/// Virtual update method to be called every frame.
		/// </summary>
		/// <param name="worldState">WorldState context for the current processing step.</param>
		void Update(WorldState& worldState);
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
		/// Name of the Sector, reflected as a prescribed Attribute.
		/// </summary>
		std::string mName;

		/// <summary>
		/// Collection of Entity objects within the Entities prescribed Attribute.
		/// </summary>
		DataType& mEntities;
#pragma endregion Data Members
	};

#pragma region Factory
	/// <summary>
	/// SectorFactory class declaration.
	/// </summary>
	ConcreteFactory(Sector, Scope)
#pragma endregion Factory
}

