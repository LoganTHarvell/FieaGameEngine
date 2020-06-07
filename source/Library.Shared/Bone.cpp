#include "pch.h"

#include "Bone.h"

#include "StreamHelper.h"

namespace Library
{
#pragma region BoneVertexWeights

	const Vector<BoneVertexWeights::VertexWeight>& BoneVertexWeights::Weights() const
	{
		return mWeights;
	}

	void BoneVertexWeights::AddWeight(float weight, uint32_t boneIndex)
	{
		if (mWeights.Size() == MaxBoneWeightsPerVertex)
		{
			throw std::runtime_error("Maximum number of bone weights per vertex exceeded.");
		}

		mWeights.EmplaceBack(weight, boneIndex);
	}
#pragma endregion

	Bone::Bone(InputStreamHelper& streamHelper)
	{
		Load(streamHelper);
	}

	Bone::Bone(const std::string& name, const uint32_t index, const Transform& offsetTransform) :
		SceneNode(name), mIndex(index), mOffsetTransform(offsetTransform)
    {
    }

	uint32_t Bone::Index() const
	{
		return mIndex;
	}

	void Bone::SetIndex(const uint32_t index)
	{
		mIndex = index;
	}

	const Transform& Bone::OffsetTransform() const
	{
		return mOffsetTransform;
	}

	void Bone::Save(OutputStreamHelper& streamHelper)
	{
		SceneNode::Save(streamHelper);

		streamHelper << mIndex << mOffsetTransform;
	}

	void Bone::Load(InputStreamHelper& streamHelper)
	{
		SceneNode::Load(streamHelper);

		streamHelper >> mIndex >> mOffsetTransform;
	}
}
