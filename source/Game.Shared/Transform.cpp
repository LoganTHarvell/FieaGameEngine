#include "pch.h"
#include "Transform.h"

namespace GameDesktop
{
	const TypeManager::TypeInfo& Transform::TypeInfo()
	{
		static const TypeManager::TypeInfo typeInfo
		{
			{
				{ LocationKey, Types::Vector, false, 1, offsetof(Transform, mLocation)},
				{ RotationKey, Types::Float, false, 1, offsetof(Transform, mRotation)},
				{ ScaleKey, Types::Vector, false, 1, offsetof(Transform, mScale)}
			},

			Attributed::TypeIdClass()
		};
		return typeInfo;
	}
	Transform::Transform(const glm::vec4& location, float rotation, const glm::vec4& scale) :
		Attributed(TypeIdClass()), mLocation(location), mRotation(rotation), mScale(scale)
	{
	}
	glm::vec3 Transform::GetLocation() const
	{
		return glm::vec3(mLocation.x, mLocation.y, mLocation.z);
	}
	glm::vec4& Transform::GetLocation()
	{
		return mLocation;
	}
	void Transform::SetLocation(const glm::vec2& location)
	{
		mLocation.x = location.x;
		mLocation.y = location.y;
	}
	void Transform::SetLocation(const glm::vec4& location)
	{
		mLocation = location;
	}
	float Transform::GetRotation() const
	{
		return mRotation;
	}
	void Transform::SetRotation(float rotation)
	{
		mRotation = rotation;
	}
	glm::vec2 Transform::GetScale() const
	{
		return glm::vec2(mScale.x, mScale.y);
	}
	void Transform::SetScale(const glm::vec2& scale)
	{
		mScale.x = scale.x;
		mScale.y = scale.y;
	}
	glm::vec2 Transform::Forward() const
	{
		return glm::vec2(cosf(mRotation), sinf(mRotation));
	}
	glm::vec2 Transform::Right() const
	{
		return glm::vec2(cosf(mRotation + 90), sinf(mRotation + 90));
	}
	gsl::owner<Transform*> Transform::Clone() const
	{
		return new Transform(*this);
	}
}