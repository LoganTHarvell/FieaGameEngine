#pragma once

#pragma region Includes
// Standard
#include <optional>

// First Party
#include "TypeManager.h"
#include "Attributed.h"
#include "Factory.h"
#pragma endregion Includes

namespace Library
{
	// Forward Declarations
	struct WorldState;

	/// <summary>
	/// Represents a base object within the reflection system.
	/// </summary>
	class Entity : public Attributed
	{
		RTTI_DECLARATIONS(Entity, Attributed)

#pragma region Hidden Inheritance
	private:
		using Attributed::Adopt;
		using Attributed::Orphan;
#pragma endregion Hidden Inheritance
		
#pragma region Type Definitions
	private:
		/// <summary>
		/// Data structure for performing an action on a given Scope at the end of an Update call.
		/// </summary>
		struct PendingChild final
		{
			/// <summary>
			/// Child Scope pending an action.
			/// </summary>
			Entity& Child;

			/// <summary>
			/// Defines the state delimiting which action is to be performed on the PendingChild.
			/// </summary>
			enum class State
			{
				Invalid = -1,

				ToAdd,
				ToRemove,

				End
			} ChildState;
		};
#pragma endregion Type Definitions
		
#pragma region Static Members
	public:
		/// <summary>
		/// Getter for the class SignatureList, used for registration with the TypeManager.
		/// </summary>
		static const SignatureList& Signatures();
#pragma endregion Static Members

#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="name">Name of the Entity.</param>
		explicit Entity(std::string name=std::string());

		/// <summary>
		/// Default destructor.
		/// </summary>
		virtual ~Entity() override = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">Entity to be copied.</param>
		Entity(const Entity& rhs);

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">Entity to be copied.</param>
		/// <returns>Newly copied into left hand side Entity.</returns>
		Entity& operator=(const Entity& rhs);

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">Entity to be moved.</param>
		Entity(Entity&& rhs) noexcept;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">Entity to be moved.</param>
		/// <returns>Newly moved into left hand side Entity.</returns>
		Entity& operator=(Entity&& rhs) noexcept;

	protected:
		/// <summary>
		/// Specialized constructor for use by derived classes to ensure correct Attribute population.
		/// </summary>
		/// <param name="typeId">Type ID of the derived class.</param>
		/// <param name="name">Entity name.</param>
		explicit Entity(const IdType typeId, std::string name=std::string());
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the Entity.</returns>
		virtual gsl::owner<Scope*> Clone() const override;
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
		/// Gets the Entity that owns this Entity.
		/// </summary>
		/// <returns>Pointer to the Entity that owns this Entity.</returns>
		virtual Entity* GetParent() const override;

		/// <summary>
		/// Sets the Entity that owns this Entity.
		/// </summary>
		/// <param name="entity">Entity to be set as the parent.</param>
		virtual void SetParent(Entity* entity);

		/// <summary>
		/// Gets whether the Entity is enabled or disabled.
		/// </summary>
		/// <returns>True when enabled. Otherwise, false.</returns>
		bool Enabled() const;

		/// <summary>
		/// Enables or disables the Entity, determining whether Update is called.
		/// </summary>
		/// <param name="enabled">Boolean determining whether to enable or disable the Entity.</param>
		void SetEnabled(const bool enabled);
		
		/// <summary>
		/// Gets the number of child Entity objects.
		/// </summary>
		/// <returns>Number of child Entity objects.</returns>
		std::size_t ChildCount() const;

		/// <summary>
		/// Gets the child Entity with the given name.
		/// </summary>
		/// <param name="name">Name of the child Entity to be found.</param>
		/// <returns>Reference to the child Entity with the given name.</returns>
		template<typename T=Entity>
		T* FindChild(const std::string& name);

		/// <summary>
		/// Gets the child Entity with the given name.
		/// </summary>
		/// <param name="name">Name of the child Entity to be found.</param>
		/// <returns>Reference to the child Entity with the given name.</returns>
		template<typename T=Entity>
		const T* FindChild(const std::string& name) const;

		/// <summary>
		/// Gets the child Entity array with the given name.
		/// </summary>
		/// <param name="name">Name of the child Entity array to be found.</param>
		/// <returns>Span of the child Entity array with the given name.</returns>
		template<typename T=Entity>
		gsl::span<T*> FindChildArray(const std::string& name);

		/// <summary>
		/// Gets the child Entity array associated with the given name.
		/// </summary>
		/// <param name="name">Name of the child Entity array to be found.</param>
		/// <returns>Span of the child Entity array with the given name.</returns>
		template<typename T=Entity>
		gsl::span<const T* const> FindChildArray(const std::string& name) const;
		
		/// <summary>
		/// Performs the given function on each child Entity.
		/// </summary>
		/// <param name="functor">Function to be performed on each child Entity.</param>
		void ForEachChild(const std::function<void(Entity&)>& functor);

		/// <summary>
		/// Performs the given function on each child Entity.
		/// </summary>
		/// <param name="functor">Function to be performed on each child Entity.</param>
		void ForEachChild(const std::function<void(const Entity&)>& functor) const;
#pragma endregion Accessors

#pragma region Modifiers
		/// <summary>
		/// Adopts a child Entity.
		/// </summary>
		/// <param name="child">Child to be adopted.</param>
		/// <returns>Reference to the newly heap allocated Entity.</returns>
		/// <exception cref="std::runtime_error">Child already exists with that name.</exception>
		Entity& AddChild(Entity& child);
		
		/// <summary>
		/// Generates an Entity and adopts it.
		/// </summary>
		/// <param name="className">Class name of an Entity or Entity subclass to be instantiated.</param>
		/// <param name="name">Name of the newly created Entity.</param>
		/// <returns>Reference to the newly heap allocated Entity.</returns>
		Entity& CreateChild(const std::string& className, const std::string& name);

		/// <summary>
		/// Orphans a child Entity.
		/// </summary>
		/// <param name="child">Child to be removed.</param>
		void DestroyChild(Entity& child);
#pragma endregion Modifiers

#pragma region Game Loop
	public:
		/// <summary>
		/// Virtual initialization method meant for use by derived classes.
		/// </summary>
		virtual void Initialize(WorldState& worldState);
		
		/// <summary>
		/// Virtual update method to be called every frame.
		/// </summary>
		/// <param name="worldState">WorldState context for the current processing step.</param>
		virtual void Update(WorldState& worldState);

		/// <summary>
		/// Virtual shutdown method meant for use by derived classes.
		/// </summary>
		virtual void Shutdown(WorldState& worldState);
#pragma endregion Game Loop

#pragma region RTTI Overrides
	public:
		/// <summary>
		/// Virtual override for representing the Entity as a std::string.
		/// </summary>
		/// <returns></returns>
		virtual std::string ToString() const override;
#pragma endregion RTTI Overrides

#pragma region Helper Methods
	protected:
		/// <summary>
		/// Performs pending actions of the child Scopes.
		/// </summary>
		void UpdatePendingChildren();
#pragma endregion Helper Methods
		
#pragma region Data Members
	protected:
		/// <summary>
		/// Name of the Entity, reflected as a prescribed Attribute.
		/// </summary>
		std::string mName;

		/// <summary>
		/// Represents whether the Entity should be updated.
		/// </summary>
		bool mEnabled{ true };
		
		/// <summary>
		/// Collection of Entity objects within the Children prescribed Attribute.
		/// </summary>
		Vector<Entity*> mChildren;

	private:
		/// <summary>
		/// Pending children to have an action performed during the end of an Update call.
		/// </summary>
		Vector<PendingChild> mPendingChildren{ Vector<PendingChild>::EqualityFunctor() };

		/// <summary>
		/// Flag representing whether the Entity is currently updating children.
		/// </summary>
		bool mUpdatingChildren{ false };
#pragma endregion Data Members
	};

#pragma region Factory
	/// <summary>
	/// EntityFactory class declaration.
	/// </summary>
	ConcreteFactory(Entity, Entity)
#pragma endregion Factory
}

// Inline File
#include "Entity.inl"