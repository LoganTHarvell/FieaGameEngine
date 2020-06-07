#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "Keyframe.h"

// First Party
#include "Transform.h"
#include "StreamHelper.h"
#pragma endregion Includes

namespace Library
{
	Keyframe::Keyframe(InputStreamHelper& streamHelper)
	{
		Load(streamHelper);
	}

	Keyframe::Keyframe(const float time, const glm::vec3& translation, const glm::quat& rotationQuaternion, const glm::vec3& scale) :
		mTime(time), mTranslation(translation), mRotationQuaternion(rotationQuaternion), mScale(scale)
    {
    }

	float Keyframe::Time() const
	{
		return mTime;
	}

	const glm::vec3& Keyframe::Translation() const
	{	
		return mTranslation;
	}

	const glm::quat& Keyframe::RotationQuaternion() const
	{
		return mRotationQuaternion;
	}

	const glm::vec3& Keyframe::Scale() const
	{
		return mScale;
	}

	glm::vec3 Keyframe::TranslationVector() const
	{
		return mTranslation;
	}

	glm::quat Keyframe::RotationQuaternionVector() const
	{
		return mRotationQuaternion;
	}

	glm::vec3 Keyframe::ScaleVector() const
	{
		return mScale;
	}

	Transform Keyframe::TransformationMatrix() const
	{	
		return Transform(mTranslation, mRotationQuaternion, mScale);
	}

	void Keyframe::Save(OutputStreamHelper& streamHelper) const
	{
		streamHelper << mTime;
		streamHelper << mTranslation.x << mTranslation.y << mTranslation.z;
		streamHelper << mRotationQuaternion.x << mRotationQuaternion.y << mRotationQuaternion.z << mRotationQuaternion.w;
		streamHelper << mScale.x << mScale.y << mScale.z;
	}

	void Keyframe::Load(InputStreamHelper& streamHelper)
	{
		streamHelper >> mTime;
		streamHelper >> mTranslation.x >> mTranslation.y >> mTranslation.z;
		streamHelper >> mRotationQuaternion.x >> mRotationQuaternion.y >> mRotationQuaternion.z >> mRotationQuaternion.w;
		streamHelper >> mScale.x >> mScale.y >> mScale.z;
	}
}
