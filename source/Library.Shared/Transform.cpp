#include "pch.h"

#include "Transform.h"

namespace Library
{
#pragma region Constants
	const Transform Transform::Identity{ glm::vec3(0), glm::quat(0, 0, 0, 1), glm::vec3(1) };
	const Transform Transform::Zero{ glm::vec3(0), glm::quat(0, 0, 0, 0), glm::vec3(0) };
#pragma endregion Constants
}
