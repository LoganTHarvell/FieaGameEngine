#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "AssetImporter.h"

// Third Party
#pragma warning(disable : 26812)
#pragma warning(disable : 26451)
#include <assimp/anim.h>
#pragma warning(default : 26451)
#pragma warning(default : 26812)

// First Party
#include "Model.h"
#include "BoneAnimation.h"
#include "Keyframe.h"
#pragma endregion Includes

namespace Library
{
	std::shared_ptr<BoneAnimation> AssetImporter::LoadBoneAnimation(Model& model, aiNodeAnim& nodeAnim)
	{
		assert(nodeAnim.mNumPositionKeys == nodeAnim.mNumRotationKeys);
		assert(nodeAnim.mNumPositionKeys == nodeAnim.mNumScalingKeys);

		BoneAnimationData boneAnimationData;
		boneAnimationData.BoneIndex = model.BoneIndexMapping().At(nodeAnim.mNodeName.C_Str());

		for (unsigned int i = 0; i < nodeAnim.mNumPositionKeys; i++)
		{
			const aiVectorKey positionKey = nodeAnim.mPositionKeys[i];
			const aiQuatKey rotationKey = nodeAnim.mRotationKeys[i];
			const aiVectorKey scaleKey = nodeAnim.mScalingKeys[i];

			assert(positionKey.mTime == rotationKey.mTime);
			assert(positionKey.mTime == scaleKey.mTime);

			boneAnimationData.Keyframes.EmplaceBack(std::make_shared<const Keyframe>(static_cast<float>(positionKey.mTime), glm::vec3(positionKey.mValue.x, positionKey.mValue.y, positionKey.mValue.z), glm::quat(rotationKey.mValue.x, rotationKey.mValue.y, rotationKey.mValue.z, rotationKey.mValue.w), glm::vec3(scaleKey.mValue.x, scaleKey.mValue.y, scaleKey.mValue.z)));
		}

		return std::make_shared<BoneAnimation>(model, std::move(boneAnimationData));
	}
}
