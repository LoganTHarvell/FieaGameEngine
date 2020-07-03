#pragma once

#pragma region Includes
// First Party
#include "Entity.h"
#include "Transform.h"
#pragma endregion Includes

namespace Library
{
	/// <summary>
	/// Represents an Entity with a Transform.
	/// </summary>
	class Actor : public Entity
	{
		RTTI_DECLARATIONS(Actor, Entity)
		
#pragma region Special Members
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="name">Name of the Actor.</param>
		explicit Actor(std::string name=std::string());

		/// <summary>
		/// Default destructor.
		/// </summary>
		virtual ~Actor() override = default;

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="rhs">Actor to be copied.</param>
		Actor(const Actor& rhs) = default;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		/// <param name="rhs">Actor to be copied.</param>
		/// <returns>Newly copied into left hand side Actor.</returns>
		Actor& operator=(const Actor& rhs) = default;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">Actor to be moved.</param>
		Actor(Actor&& rhs) noexcept = default;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">Actor to be moved.</param>
		/// <returns>Newly moved into left hand side Actor.</returns>
		Actor& operator=(Actor&& rhs) noexcept = default;

	protected:
		/// <summary>
		/// Specialized constructor for use by derived classes to ensure correct Attribute population.
		/// </summary>
		/// <param name="typeId">Type ID of the derived class.</param>
		/// <param name="name">Actor name.</param>
		explicit Actor(const IdType typeId, std::string name=std::string());
#pragma endregion Special Members

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// </summary>
		/// <returns>Owning pointer to a newly heap allocated copy of the Actor.</returns>
		virtual gsl::owner<Scope*> Clone() const override;
#pragma endregion Virtual Copy Constructor

#pragma region Accessors
	public:
		/// <summary>
		/// Gets the Root Actor to which this Actor is relative.
		/// </summary>
		/// <returns>Pointer to the Root Actor.</returns>
		const Actor* RootActor() const;

		/// <summary>
		/// Sets the Root Actor to which this Actor is relative.
		/// </summary>
		void SetRootActor(Actor* actor);
		
		/// <summary>
		/// Gets the local Transform.
		/// </summary>
		/// <returns>Reference to the local Transform.</returns>
		const Transform& LocalTransform() const;
		
		/// <summary>
		/// Gets the world Transform.
		/// </summary>
		/// <returns>Reference to the world Transform.</returns>
		const Transform& WorldTransform() const;

		/// <summary>
		/// Sets the local Transform translation.
		/// </summary>
		/// <param name="translation">Translation vector.</param>
		void SetLocalTranslation(const glm::vec3& translation);

		/// <summary>
		/// Sets the world Transform translation.
		/// </summary>
		/// <param name="translation">Translation vector.</param>
		void SetWorldTranslation(const glm::vec3& translation);

		/// <summary>
		/// Sets the local Transform rotation.
		/// </summary>
		/// <param name="angle">Rotation angle in radians.</param>
		/// <param name="axis">Rotation axis vector, recommended to be normalized.</param>
		void SetLocalRotation(const float angle, const glm::vec3& axis);

		/// <summary>
		/// Sets the world Transform rotation.
		/// </summary>
		/// <param name="angle">Rotation angle in radians.</param>
		/// <param name="axis">Rotation axis vector, recommended to be normalized.</param>
		void SetWorldRotation(const float angle, const glm::vec3& axis);

		/// <summary>
		/// Sets the local Transform rotation.
		/// </summary>
		/// <param name="quaternion">Rotation as a quaternion.</param>
		void SetLocalRotation(const glm::quat& quaternion);

		/// <summary>
		/// Sets the world Transform rotation.
		/// </summary>
		/// <param name="quaternion">Rotation as a quaternion.</param>
		void SetWorldRotation(const glm::quat& quaternion);

		/// <summary>
		/// Sets the local Transform scale.
		/// </summary>
		/// <param name="scale">Scale vector.</param>
		void SetLocalScale(const glm::vec3& scale);

		/// <summary>
		/// Sets the world Transform scale.
		/// </summary>
		/// <param name="scale">Scale vector.</param>
		void SetWorldScale(const glm::vec3& scale);

		/// <summary>
		/// Sets the local Transform.
		/// </summary>
		/// <param name="transform">New Transform.</param>
		void SetLocalTransform(const Transform& transform);
		
		/// <summary>
		/// Sets the world Transform.
		/// </summary>
		/// <param name="transform">New Transform.</param>
		void SetWorldTransform(const Transform& transform);
#pragma endregion Accessors

#pragma region Modifiers
	public:		
		/// <summary>
		/// Translates the local Transform.
		/// </summary>
		/// <param name="translation">Vector used to translate the Transform.</param>
		void AddLocalTranslation(const glm::vec3& translation);

		/// <summary>
		/// Translates the world Transform.
		/// </summary>
		/// <param name="translation">Vector used to translate the Transform.</param>
		void AddWorldTranslation(const glm::vec3& translation);

		/// <summary>
		/// Rotates the local Transform.
		/// </summary>
		/// <param name="angle">Rotation angle in radians.</param>
		/// <param name="axis">Rotation axis vector, recommended to be normalized.</param>
		void AddLocalRotation(const float angle, const glm::vec3& axis);

		/// <summary>
		/// Rotates the world Transform.
		/// </summary>
		/// <param name="angle">Rotation angle to be applied in radians.</param>
		/// <param name="axis">Rotation axis vector, recommended to be normalized.</param>
		void AddWorldRotation(const float angle, const glm::vec3& axis);

		/// <summary>
		/// Rotates the local Transform.
		/// </summary>
		/// <param name="quaternion">Rotation as a quaternion.</param>
		void AddLocalRotation(const glm::quat& quaternion);

		/// <summary>
		/// Rotates the world Transform.
		/// </summary>
		/// <param name="quaternion">Rotation to be applied.</param>
		void AddWorldRotation(const glm::quat& quaternion);

		/// <summary>
		/// Transforms the local Transform.
		/// </summary>
		/// <param name="transformation">Transformation to be applied.</param>
		void LocalTransformation(const Transform& transformation);

		/// <summary>
		/// Transforms the world Transform.
		/// </summary>
		/// <param name="transformation">Transformation to be applied.</param>
		void WorldTransformation(const Transform& transformation);
#pragma endregion Modifiers

#pragma region Helpers
	private:
		/// <summary>
		/// Sets the world transform to the local transform relative to the RootActor world transform.
		/// </summary>
		void UpdateWorldTransform() const;
#pragma endregion Helpers
		
#pragma region Data Members
	private:
		/// <summary>
		/// Root Actor to which this Actor is relative.
		/// </summary>
		Actor* mRootActor{ nullptr };
		
		/// <summary>
		/// Local Transform of the Actor.
		/// </summary>
		Transform mLocalTransform{ Transform::Identity };
		
		/// <summary>
		/// World Transform of the Actor.
		/// </summary>
		mutable Transform mWorldTransform{ Transform::Identity };

		/// <summary>
		/// Flag for when the world transform needs updating.
		/// </summary>
		mutable bool mDirtyWorldTransform{ true };
#pragma endregion Data Members
	};
	
#pragma region Factory
		/// <summary>
		/// ActorFactory class declaration.
		/// </summary>
		ConcreteFactory(Actor, Entity)
#pragma endregion Factory
}

// Inline File
#include "Actor.inl"