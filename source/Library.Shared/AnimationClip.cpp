#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "AnimationClip.h"

// First Party
#include "BoneAnimation.h"
#include "Bone.h"
#include "StreamHelper.h"
#pragma endregion Includes

namespace Library
{
#pragma region AnimationClipData

	AnimationClipData::AnimationClipData(std::string name, const float duration, const float ticksPerSecond) :
		Name(std::move(name)), Duration(duration), TicksPerSecond(ticksPerSecond)
	{
	}

#pragma endregion

	AnimationClip::AnimationClip(Model& model, InputStreamHelper& streamHelper)
	{
		Load(model, streamHelper);
	}

	AnimationClip::AnimationClip(AnimationClipData&& animationClipData) :
		mData(std::move(animationClipData))
	{		
	}

	const std::string& AnimationClip::Name() const
	{
		return mData.Name;
	}

	float AnimationClip::Duration() const
	{
		return mData.Duration;
	}

	float AnimationClip::TicksPerSecond() const
	{
		return mData.TicksPerSecond;
	}

	const Vector<std::shared_ptr<const BoneAnimation>>& AnimationClip::BoneAnimations() const
	{
		return mData.BoneAnimations;
	}

	const HashMap<const Bone*, std::shared_ptr<const BoneAnimation>>& AnimationClip::BoneAnimationsByBone() const
	{
		return mData.BoneAnimationsByBone;
	}

	std::uint32_t AnimationClip::KeyframeCount() const
	{
		return mData.KeyframeCount;
	}

	std::uint32_t AnimationClip::GetTransform(const float time, const Bone& bone, Transform& transform) const
	{
		const auto& foundBoneAnimation = mData.BoneAnimationsByBone.Find(&bone);
		if (foundBoneAnimation != mData.BoneAnimationsByBone.end())
		{
			return foundBoneAnimation->second->GetTransform(time, transform);
		}
		else
		{
			transform = Transform::Identity;
			return std::numeric_limits<std::uint32_t>::max();
		}
	}

	void AnimationClip::GetTransforms(const float time, Vector<Transform>& boneTransforms) const
	{
		for (const auto& boneAnimation : mData.BoneAnimations)
		{
			boneAnimation->GetTransform(time, boneTransforms[boneAnimation->GetBone().Index()]);
		}
	}

	void AnimationClip::GetTransformAtKeyframe(const std::uint32_t keyframe, const Bone& bone, Transform& transform) const
	{
		const auto& foundBoneAnimation = mData.BoneAnimationsByBone.Find(&bone);
		if (foundBoneAnimation != mData.BoneAnimationsByBone.end())
		{
			foundBoneAnimation->second->GetTransformAtKeyframe(keyframe, transform);
		}
		else
		{
			transform = Transform::Identity;
		}
	}

	void AnimationClip::GetTransformsAtKeyframe(const std::uint32_t keyframe, Vector<Transform>& boneTransforms) const
	{
		for (const auto& boneAnimation : mData.BoneAnimations)
		{
			boneAnimation->GetTransformAtKeyframe(keyframe, boneTransforms[boneAnimation->GetBone().Index()]);
		}
	}

	void AnimationClip::GetInterpolatedTransform(const float time, const Bone& bone, Transform& transform) const
	{
		const auto& foundBoneAnimation = mData.BoneAnimationsByBone.Find(&bone);

		if (foundBoneAnimation != mData.BoneAnimationsByBone.end())
		{
			foundBoneAnimation->second->GetInterpolatedTransform(time, transform);
		}
		else
		{
			transform = Transform::Identity;
		}
	}

	void AnimationClip::GetInterpolatedTransforms(const float time, Vector<Transform>& boneTransforms) const
	{
		for (const auto& boneAnimation : mData.BoneAnimations)
		{
			boneAnimation->GetInterpolatedTransform(time, boneTransforms[boneAnimation->GetBone().Index()]);
		}
	}

	void AnimationClip::Save(OutputStreamHelper& streamHelper) const
	{
		streamHelper << mData.Name << mData.Duration << mData.TicksPerSecond;
		
		// Serialize bone animations
		streamHelper << gsl::narrow_cast<std::uint32_t>(mData.BoneAnimations.Size());

		for (const auto& boneAnimation: mData.BoneAnimations)
		{
			boneAnimation->Save(streamHelper);
		}

		streamHelper << mData.KeyframeCount;
	}

	void AnimationClip::Load(Model& model, InputStreamHelper& streamHelper)
	{
		streamHelper >> mData.Name >> mData.Duration >> mData.TicksPerSecond;

		// Deserialize bone animations
		std::uint32_t boneAnimationCount;
		streamHelper >> boneAnimationCount;
		mData.BoneAnimations.Reserve(boneAnimationCount);
		for (std::uint32_t i = 0; i < boneAnimationCount; i++)
		{
			std::shared_ptr<BoneAnimation> boneAnimation = std::make_shared<BoneAnimation>(model, streamHelper);
			mData.BoneAnimations.EmplaceBack(boneAnimation);
			mData.BoneAnimationsByBone[&(boneAnimation->GetBone())] = boneAnimation;
		}

		streamHelper >> mData.KeyframeCount;
	}
}
