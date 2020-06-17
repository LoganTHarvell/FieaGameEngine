#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "Actor.h"
#pragma endregion Includes

namespace Library
{	
	Actor::Actor(std::string name) : Entity(TypeIdClass(), std::move(name))
	{
	}

	Actor::Actor(const IdType typeId, std::string name) : Entity(typeId, std::move(name))
	{
	}

	gsl::owner<Scope*> Actor::Clone() const
	{
		return new Actor(*this);
	}

	Transform& Actor::GetTransform()
	{
		return mTransform;
	}

	const Transform& Actor::GetTransform() const
	{
		return mTransform;
	}
}
