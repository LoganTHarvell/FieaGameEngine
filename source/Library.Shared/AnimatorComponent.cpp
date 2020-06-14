#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "AnimatorComponent.h"

// First Party
#include "Model.h"
#include "Bone.h"
#include "AnimationClip.h"
#include "WorldState.h"
#include "GameTime.h"
#include "Transform.h"
#pragma endregion Includes

namespace Library
{
	AnimatorComponent::AnimatorComponent(std::shared_ptr<Model> model, std::string name, const bool interpolationEnabled) : Entity(AnimatorComponent::TypeIdClass(), std::move(name)),
		mModel(std::move(model)), mInterpolationEnabled(interpolationEnabled)
	{
		mFinalTransforms.Resize(mModel->Bones().Size());
	}

	const std::shared_ptr<Model>& AnimatorComponent::GetModel() const
	{
		return mModel;
	}

	const std::shared_ptr<AnimationClip>& AnimatorComponent::CurrentClip() const
	{
		return mCurrentClip;
	}

	float AnimatorComponent::CurrentTime() const
	{
		return mCurrentTime;
	}

	std::uint32_t AnimatorComponent::CurrentKeyframe() const
	{
		return mCurrentKeyframe;
	}
	
	const Vector<glm::mat4x4>& AnimatorComponent::BoneTransforms() const
	{
		return mFinalTransforms;
	}

	bool AnimatorComponent::InterpolationEnabled() const
	{
		return mInterpolationEnabled;
	}

	bool AnimatorComponent::IsPlayingClip() const
	{
		return mIsPlayingClip;
	}

	bool AnimatorComponent::IsClipLooped() const
	{
		return mIsClipLooped;
	}

	void AnimatorComponent::SetInterpolationEnabled(const bool enabled)
	{
		mInterpolationEnabled = enabled;
	}

	void AnimatorComponent::StartClip(const std::shared_ptr<AnimationClip>& clip)
	{
		mCurrentClip = clip;
		mCurrentTime = 0.0f;
		mCurrentKeyframe = 0;
		mIsPlayingClip = true;

		mInverseRootTransform = glm::inverse(mModel->RootNode()->GetTransform());
		GetPose(mCurrentTime, *(mModel->RootNode()));
	}

	void AnimatorComponent::PauseClip()
	{
		mIsPlayingClip = false;
	}

	void AnimatorComponent::ResumeClip()
	{
		if (mCurrentClip != nullptr)
		{
			mIsPlayingClip = true;
		}
	}

	void AnimatorComponent::RestartClip()
	{
		if (mCurrentClip != nullptr)
		{
			StartClip(mCurrentClip);
		}
	}

	void AnimatorComponent::Update(WorldState& worldState)
	{
		if (mIsPlayingClip)
		{
			assert(mCurrentClip != nullptr);

			mCurrentTime += worldState.GameTime->ElapsedGameTimeSeconds().count() * mCurrentClip->TicksPerSecond();
			if (mCurrentTime >= mCurrentClip->Duration())
			{
				if (mIsClipLooped)
				{
					mCurrentTime = 0.0f;
				}
				else
				{
					mIsPlayingClip = false;
					return;
				}
			}

			const auto& rootNode = *(mModel->RootNode());
			
			if (mInterpolationEnabled)
			{
				GetInterpolatedPose(mCurrentTime, rootNode);
			}
			else
			{
				GetPose(mCurrentTime, rootNode);
			}
		}
	}

	void AnimatorComponent::SetCurrentKeyFrame(const std::uint32_t keyframe)
	{
		mCurrentKeyframe = keyframe;
		GetPoseAtKeyframe(mCurrentKeyframe, *(mModel->RootNode()));
	}

	void AnimatorComponent::GetBindPoseBottomUp(const SceneNode& sceneNode)
	{
		glm::mat4x4 toRootTransform = sceneNode.GetTransform();

		auto parentNode = sceneNode.GetParent().lock();
		while (parentNode != nullptr)
		{
			toRootTransform = toRootTransform * parentNode->GetTransform();
			parentNode = parentNode->GetParent().lock();
		}

		const Bone* bone = sceneNode.As<Bone>();

		if (bone != nullptr)
		{
			mFinalTransforms[bone->Index()] = bone->OffsetTransform() * toRootTransform * mInverseRootTransform;
		}

		for (const auto& childNode : sceneNode.Children())
		{
			GetBindPoseBottomUp(*childNode);
		}
	}

	void AnimatorComponent::GetBindPose()
	{
		GetBindPose(*(mModel->RootNode()));
	}

	void AnimatorComponent::GetBindPose(const SceneNode& sceneNode)
	{
		const glm::mat4x4 toParentTransform = sceneNode.GetTransform();
		const auto parentNode = sceneNode.GetParent().lock();
		const glm::mat4x4 toRootTransform = (parentNode != nullptr ? toParentTransform * mToRootTransforms.At(parentNode.get()) : toParentTransform);
		mToRootTransforms[&sceneNode] = toRootTransform;		

		const Bone* bone = sceneNode.As<Bone>();

		if (bone != nullptr)
		{
			mFinalTransforms[bone->Index()] = bone->OffsetTransform() * toRootTransform * mInverseRootTransform;
		}

		for (const auto& childNode : sceneNode.Children())
		{
			GetBindPose(*childNode);
		}
	}

	void AnimatorComponent::GetPose(const float time, const SceneNode& sceneNode)
	{
		Transform toParentTransform;
		glm::mat4x4 toParentTransformMatrix;
		const Bone* bone = sceneNode.As<Bone>();

		if (bone != nullptr)
		{
			const auto keyframe = mCurrentClip->GetTransform(time, *bone, toParentTransform);
			
			if (keyframe != std::numeric_limits<std::uint32_t>::max())
			{
				mCurrentKeyframe = keyframe;
			}

			toParentTransformMatrix = toParentTransform.Matrix();
		}
		else
		{
			toParentTransformMatrix = sceneNode.GetTransform();
		}

		const auto parentNode = sceneNode.GetParent().lock();
		const glm::mat4x4 toRootTransform = (parentNode != nullptr ? toParentTransformMatrix * mToRootTransforms.At(parentNode.get()) : toParentTransformMatrix);
		mToRootTransforms[&sceneNode] = toRootTransform;

		if (bone != nullptr)
		{
			mFinalTransforms[bone->Index()] = bone->OffsetTransform() * toRootTransform * mInverseRootTransform;
		}

		for (const auto& childNode : sceneNode.Children())
		{
			GetPose(time, *childNode);
		}
	}

	void AnimatorComponent::GetPoseAtKeyframe(const std::uint32_t keyframe, const SceneNode& sceneNode)
	{
		Transform toParentTransform;
		glm::mat4x4 toParentTransformMatrix;
		const Bone* bone = sceneNode.As<Bone>();

		if (bone != nullptr)
		{
			mCurrentClip->GetTransformAtKeyframe(keyframe, *bone, toParentTransform);
			toParentTransformMatrix = toParentTransform.Matrix();
		}
		else
		{
			toParentTransformMatrix = sceneNode.GetTransform();
		}

		const auto& parentNode = sceneNode.GetParent().lock();
		const glm::mat4x4 toRootTransform = (parentNode != nullptr ? toParentTransformMatrix * mToRootTransforms.At(parentNode.get()) : toParentTransformMatrix);
		mToRootTransforms[&sceneNode] = toRootTransform;

		if (bone != nullptr)
		{
			mFinalTransforms[bone->Index()] = bone->OffsetTransform() * toRootTransform * mInverseRootTransform;
		}

		for (const auto& childNode : sceneNode.Children())
		{
			GetPoseAtKeyframe(keyframe, *childNode);
		}
	}

	void AnimatorComponent::GetInterpolatedPose(const float time, const SceneNode& sceneNode)
	{
		Transform toParentTransform;
		glm::mat4x4 toParentTransformMatrix;
		const Bone* bone = sceneNode.As<Bone>();

		if (bone != nullptr)
		{
			mCurrentClip->GetInterpolatedTransform(time, *bone, toParentTransform);
			toParentTransformMatrix = toParentTransform.Matrix();
		}
		else
		{
			toParentTransformMatrix = sceneNode.GetTransform();
		}

		const auto& parentNode = sceneNode.GetParent().lock();
		const glm::mat4x4 toRootTransform = (parentNode != nullptr ? toParentTransformMatrix * mToRootTransforms.At(parentNode.get()) : toParentTransformMatrix);
		mToRootTransforms[&sceneNode] = toRootTransform;

		if (bone != nullptr)
		{
			mFinalTransforms[bone->Index()] = bone->OffsetTransform() * toRootTransform * mInverseRootTransform;
		}

		for (const auto& childNode : sceneNode.Children())
		{
			GetInterpolatedPose(time, *childNode);
		}
	}
}
