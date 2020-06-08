#pragma once

#pragma region Includes
// First Party
#include "Vector.h"
#pragma endregion Includes

namespace Library
{
	// Forward Declarations
	class Transform;
	class Model;
	class Bone;
	class Keyframe;
	class OutputStreamHelper;
	class InputStreamHelper;

	struct BoneAnimationData final
	{
		std::uint32_t BoneIndex{ 0 };
		Vector<std::shared_ptr<Keyframe>> Keyframes;
	};

    class BoneAnimation final
    {
    public:
		BoneAnimation(Model& model, InputStreamHelper& streamHelper);
		BoneAnimation(Model& model, const BoneAnimationData& boneAnimationData);
		BoneAnimation(Model& model, BoneAnimationData&& boneAnimationData);
		BoneAnimation(const BoneAnimation&) = default;
		BoneAnimation(BoneAnimation&& rhs) = default;
		BoneAnimation& operator=(const BoneAnimation& rhs) = default;
		BoneAnimation& operator=(BoneAnimation&& rhs) = default;
		~BoneAnimation() = default;

		Bone& GetBone();
		const Bone& GetBone() const;
		Vector<std::shared_ptr<Keyframe>>& Keyframes();

		std::uint32_t GetTransform(const float time, Transform& transform) const;
		void GetTransformAtKeyframe(std::uint32_t keyframeIndex, Transform& transform) const;
		void GetInterpolatedTransform(const float time, Transform& transform) const;

		void Save(OutputStreamHelper& streamHelper) const;

    private:
		void Load(InputStreamHelper& streamHelper);
		std::uint32_t FindKeyframeIndex(const float time) const;

		Model* mModel;
		std::weak_ptr<Bone> mBone;
		Vector<std::shared_ptr<Keyframe>> mKeyframes;
    };
}
