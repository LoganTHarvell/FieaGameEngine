#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "BoneAnimation.h"

// First Party
#include "Model.h"
#include "Bone.h"
#include "Keyframe.h"
#include "StreamHelper.h"
#pragma endregion Includes

namespace Library
{
	BoneAnimation::BoneAnimation(Model& model, InputStreamHelper& streamHelper) :
		mModel(&model)
	{
		Load(streamHelper);
	}

	BoneAnimation::BoneAnimation(Model& model, const BoneAnimationData& boneAnimationData) :
		mModel(&model), mBone(model.Bones().At(boneAnimationData.BoneIndex)), mKeyframes(boneAnimationData.Keyframes)
	{
	}

	BoneAnimation::BoneAnimation(Model& model, BoneAnimationData&& boneAnimationData) :
		mModel(&model), mBone(model.Bones().At(boneAnimationData.BoneIndex)), mKeyframes(move(boneAnimationData.Keyframes))
	{
		boneAnimationData.BoneIndex = 0U;
	}

	Bone& BoneAnimation::GetBone()
	{
		const auto& bone = mBone.lock();
		assert(bone != nullptr);
		
		return *bone;
	}

	const Bone& BoneAnimation::GetBone() const
	{
		const auto& bone = mBone.lock();
		assert(bone != nullptr);
		
		return *bone;
	}
	
	Vector<std::shared_ptr<Keyframe>>& BoneAnimation::Keyframes()
	{
		return mKeyframes;
	}

	uint32_t BoneAnimation::GetTransform(const float time, Transform& transform) const
	{
		const std::uint32_t keyframeIndex = FindKeyframeIndex(time);
		const std::shared_ptr<Keyframe>& keyframe = mKeyframes[keyframeIndex];
		transform = keyframe->TransformationMatrix();

		return keyframeIndex;
	}

	void BoneAnimation::GetTransformAtKeyframe(std::uint32_t keyframeIndex, Transform& transform) const
	{
		// Clamp the keyframe
		if (keyframeIndex >= mKeyframes.Size())
		{
			keyframeIndex = gsl::narrow_cast<std::uint32_t>(mKeyframes.Size() - 1);
		}
		
		const std::shared_ptr<Keyframe>& keyframe = mKeyframes[keyframeIndex];
		transform = keyframe->TransformationMatrix();
	}

	void BoneAnimation::GetInterpolatedTransform(const float time, Transform& transform) const
	{
		const std::shared_ptr<Keyframe>& firstKeyframe = mKeyframes.Front();
		const std::shared_ptr<Keyframe>& lastKeyframe = mKeyframes.Back();

		if (time <= firstKeyframe->Time())
		{
			// Specified time is before the start time of the animation, so return the first keyframe
			transform = firstKeyframe->TransformationMatrix();
		}
		else if (time >= lastKeyframe->Time())
		{
			// Specified time is after the end time of the animation, so return the last keyframe
			transform = lastKeyframe->TransformationMatrix();
		}
		else
		{
			// Interpolate the transform between keyframes
			const uint32_t keyframeIndex = FindKeyframeIndex(time);
			const std::shared_ptr<Keyframe>& keyframeOne = mKeyframes[keyframeIndex];
			const std::shared_ptr<Keyframe>& keyframeTwo = mKeyframes[keyframeIndex + 1];

			const glm::vec3 translationOne = keyframeOne->TranslationVector();
			const glm::quat rotationQuaternionOne = keyframeOne->RotationQuaternionVector();
			const glm::vec3 scaleOne = keyframeOne->ScaleVector();
			
			const glm::vec3 translationTwo = keyframeTwo->TranslationVector();
			const glm::quat rotationQuaternionTwo = keyframeTwo->RotationQuaternionVector();
			const glm::vec3 scaleTwo = keyframeTwo->ScaleVector();

			const float lerpValue = ((time - keyframeOne->Time()) / (keyframeTwo->Time() - keyframeOne->Time()));
			const glm::vec3 translation = glm::lerp(translationOne, translationTwo, lerpValue);
			const glm::quat rotationQuaternion = glm::slerp(rotationQuaternionOne, rotationQuaternionTwo, lerpValue);
			const glm::vec3 scale = glm::lerp(scaleOne, scaleTwo, lerpValue);

			transform = Transform(scale, rotationQuaternion, translation);
		}
	}

	void BoneAnimation::Save(OutputStreamHelper& streamHelper) const
	{
		const Bone& bone = GetBone();
		streamHelper << bone.Name();

		streamHelper << gsl::narrow_cast<uint32_t>(mKeyframes.Size());
		for (const auto& keyframe: mKeyframes)
		{
			keyframe->Save(streamHelper);
		}
	}

	void BoneAnimation::Load(InputStreamHelper& streamHelper)
	{
		// Deserialize the referenced bone
		std::string name;
		streamHelper >> name;
		const uint32_t boneIndex = mModel->BoneIndexMapping().At(name);		
		mBone = mModel->Bones().At(boneIndex);

		// Deserialize the keyframes
		uint32_t keyframeCount;
		streamHelper >> keyframeCount;
		mKeyframes.Reserve(keyframeCount);
		for (uint32_t i = 0; i < keyframeCount; i++)
		{
			mKeyframes.EmplaceBack(std::make_shared<Keyframe>(streamHelper));
		}
	}

	uint32_t BoneAnimation::FindKeyframeIndex(const float time) const
	{
		const std::shared_ptr<Keyframe>& firstKeyframe = mKeyframes.Front();
		if (time <= firstKeyframe->Time())
		{
			return 0;
		}

		const std::shared_ptr<Keyframe>& lastKeyframe = mKeyframes.Back();
		if (time >= lastKeyframe->Time())
		{
			return gsl::narrow_cast<uint32_t>(mKeyframes.Size() - 1);
		}

		uint32_t keyframeIndex = 1;

		for (; keyframeIndex < mKeyframes.Size() - 1 && time >= mKeyframes[keyframeIndex]->Time(); keyframeIndex++);

		return keyframeIndex - 1;
	}
}
