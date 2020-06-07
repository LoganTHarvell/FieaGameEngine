#pragma once

#pragma region Includes
// First Party
#include "Entity.h"
#include "Transform.h"
#pragma endregion Includes

namespace Library
{
	// Forwarded Classes
	class Model;
	class SceneNode;
	class AnimationClip;

	/// <summary>
	/// A entity component that enables animation of a model.
	/// </summary>
    class AnimatorComponent final : public Entity
    {
		RTTI_DECLARATIONS_ABSTRACT(AnimatorComponent, Entity)
    	
    public:
		AnimatorComponent() = delete;
		~AnimatorComponent() = default;
		AnimatorComponent(const AnimatorComponent& rhs) = default;
		AnimatorComponent& operator=(const AnimatorComponent& rhs) = default;
		AnimatorComponent(AnimatorComponent&& rhs) = default;
		AnimatorComponent& operator=(AnimatorComponent&& rhs) = default;

    	AnimatorComponent(std::shared_ptr<Model> model, std::string name=std::string(), const bool interpolationEnabled=true);

		const std::shared_ptr<Model>& GetModel() const;
		const std::shared_ptr<AnimationClip>& CurrentClip() const;
		float CurrentTime() const;
		std::uint32_t CurrentKeyframe() const;
		const Vector<Transform>& BoneTransforms() const;
		
		bool InterpolationEnabled() const;
		void SetInterpolationEnabled(const bool enabled);

		bool IsPlayingClip() const;
		bool IsClipLooped() const;

		void StartClip(const std::shared_ptr<AnimationClip>& clip);
		void PauseClip();
		void ResumeClip();
		void RestartClip();
		void SetCurrentKeyFrame(std::uint32_t keyframe);
		void GetBindPose();

		virtual void Update(WorldState& worldState) override;

    private:
		void GetBindPose(const SceneNode& sceneNode);
		void GetBindPoseBottomUp(const SceneNode& sceneNode);
		void GetPose(const float time, const SceneNode& sceneNode);
		void GetPoseAtKeyframe(std::uint32_t keyframe, const SceneNode& sceneNode);
		void GetInterpolatedPose(const float time, const SceneNode& sceneNode);		

		std::shared_ptr<Model> mModel;
		std::shared_ptr<AnimationClip> mCurrentClip;
		float mCurrentTime{ 0.0f };
		std::uint32_t mCurrentKeyframe{ 0 };
		HashMap<const SceneNode*, Transform> mToRootTransforms;
		Vector<Transform> mFinalTransforms;
		Transform mInverseRootTransform{ Transform::Identity };
		bool mInterpolationEnabled;
		bool mIsPlayingClip{ false };
		bool mIsClipLooped{ true };
    };
}
