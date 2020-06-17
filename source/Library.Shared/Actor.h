#pragma once

#pragma region Includes
// First Party
#include "Entity.h"
#include "Transform.h"
#pragma endregion Includes

namespace Library
{	
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
		/// Gets the Transform instance of the Actor.
		/// </summary>
		/// <returns>Reference to the Transform instance of the Actor.</returns>
		Transform& GetTransform();

		/// <summary>
		/// Gets the Transform instance of the Actor.
		/// </summary>
		/// <returns>Reference to the Transform instance of the Actor.</returns>
		const Transform& GetTransform() const;
#pragma endregion Accessors
		
#pragma region Data Members
	private:
		/// <summary>
		/// Transform of the Actor.
		/// </summary>
		Transform mTransform{ Transform::Identity };
#pragma endregion Data Members

	};
	
#pragma region Factory
		/// <summary>
		/// ActorFactory class declaration.
		/// </summary>
		ConcreteFactory(Actor, Entity)
#pragma endregion Factory
}