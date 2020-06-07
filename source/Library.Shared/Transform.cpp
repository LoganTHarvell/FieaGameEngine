#include "pch.h"

#include "Transform.h"

namespace Library
{
#pragma region Constants
	const Transform Transform::Identity{ glm::identity<glm::mat4x4>() };
	const Transform Transform::Zero{ 0 };
#pragma endregion Constants
}
