#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "AssetImporter.h"

// ThirdParty
#pragma warning(disable : 26812)
#pragma warning(disable : 26451)
#include <assimp/anim.h>
#pragma warning(default : 26451)
#pragma warning(default : 26812)

// First Party
#include "AnimationClip.h"
#include "BoneAnimation.h"
#pragma endregion Includes

namespace Library
{
	std::shared_ptr<AnimationClip> AssetImporter::LoadAnimationClip(Library::Model& model, aiAnimation& animation)
	{
		AnimationClipData animationClipData(animation.mName.C_Str(), static_cast<float>(animation.mDuration), static_cast<float>(animation.mTicksPerSecond));

		assert(animation.mNumChannels > 0);

		if (animationClipData.TicksPerSecond <= 0.0f)
		{
			animationClipData.TicksPerSecond = 1.0f;
		}

		animationClipData.BoneAnimations.Reserve(animation.mNumChannels);
		for (unsigned int i = 0; i < animation.mNumChannels; i++)
		{
			std::shared_ptr<BoneAnimation> boneAnimation = AssetImporter::LoadBoneAnimation(model, *(animation.mChannels[i]));
			animationClipData.BoneAnimations.EmplaceBack(boneAnimation);

			assert(animationClipData.BoneAnimationsByBone.Find(&(boneAnimation->GetBone())) == animationClipData.BoneAnimationsByBone.end());
			animationClipData.BoneAnimationsByBone[&(boneAnimation->GetBone())] = boneAnimation;
		}

		for (auto& boneAnimation : animationClipData.BoneAnimations)
		{
			if (boneAnimation->Keyframes().Size() > animationClipData.KeyframeCount)
			{
				animationClipData.KeyframeCount = gsl::narrow_cast<uint32_t>(boneAnimation->Keyframes().Size());
			}
		}

		return std::make_shared<AnimationClip>(std::move(animationClipData));
	}
}
