#pragma once

#pragma region Includes
// First Party
#include "Vector.h"
#include "HashMap.h"
#pragma endregion Includes

namespace Library
{
	// Forwarded Classes
	class Transform;
	class Model;
	class Bone;
	class BoneAnimation;
	class OutputStreamHelper;
	class InputStreamHelper;

	struct AnimationClipData final
	{
		AnimationClipData() = default;
		AnimationClipData(std::string name, const float duration, const float ticksPerSecond);
		AnimationClipData(const AnimationClipData&) = default;
		AnimationClipData(AnimationClipData&&) = default;
		AnimationClipData& operator=(const AnimationClipData&) = default;
		AnimationClipData& operator=(AnimationClipData&&) = default;
		~AnimationClipData() = default;

		std::string Name;
		float Duration{ 0.0f };
		float TicksPerSecond{ 0.0f };
		Vector<std::shared_ptr<const BoneAnimation>> BoneAnimations;
		HashMap<const Bone*, std::shared_ptr<const BoneAnimation>> BoneAnimationsByBone;
		std::uint32_t KeyframeCount{ 0 };
	};

    class AnimationClip final
    {
    public:
		AnimationClip(Model& model, InputStreamHelper& streamHelper);
		explicit AnimationClip(AnimationClipData&& animationClipData);
		AnimationClip(const AnimationClip&) = default;
		AnimationClip(AnimationClip&&) = default;
		AnimationClip& operator=(const AnimationClip&) = default;
		AnimationClip& operator=(AnimationClip&&) = default;
		~AnimationClip() = default;

		const std::string& Name() const;
		float Duration() const;
		float TicksPerSecond() const;
		const Vector<std::shared_ptr<const BoneAnimation>>& BoneAnimations() const;
		const HashMap<const Bone*, std::shared_ptr<const BoneAnimation>>& BoneAnimationsByBone() const;
		std::uint32_t KeyframeCount() const;

		std::uint32_t GetTransform(const float time, const Bone& bone, Transform& transform) const;
		void GetTransforms(const float time, Vector<Transform>& boneTransforms) const;
		
		void GetTransformAtKeyframe(const std::uint32_t keyframe, const Bone& bone, Transform& transform) const;
		void GetTransformsAtKeyframe(const std::uint32_t keyframe, Vector<Transform>& boneTransforms) const;

		void GetInterpolatedTransform(const float time, const Bone& bone, Transform& transform) const;
		void GetInterpolatedTransforms(const float time, Vector<Transform>& boneTransforms) const;

		void Save(OutputStreamHelper& streamHelper) const;

    private:
		void Load(Model& model, InputStreamHelper& streamHelper);

		AnimationClipData mData;
    };
}
