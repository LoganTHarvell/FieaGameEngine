#pragma once

#pragma region Includes
// First Party
#include "SceneNode.h"
#pragma endregion Includes

namespace Library
{
	class BoneVertexWeights final
	{
	public:
		struct VertexWeight
		{
			float Weight;
			std::uint32_t BoneIndex;

			VertexWeight(const float weight, const std::uint32_t boneIndex) :
				Weight(weight), BoneIndex(boneIndex) { }

			bool operator==(const VertexWeight& rhs) const noexcept
			{
				if (this == &rhs) return true;
				return this->Weight == rhs.Weight && BoneIndex == rhs.BoneIndex;
			}
			
			bool operator!=(const VertexWeight& rhs) const noexcept
			{
				return !operator==(rhs);
			}
		};
		
		const Vector<VertexWeight>& Weights() const;
		void AddWeight(float weight, std::uint32_t boneIndex);

		static const std::uint32_t MaxBoneWeightsPerVertex = 4U;

		bool operator==(const BoneVertexWeights& rhs) const noexcept
		{
			if (this == &rhs) return true;
			return mWeights == rhs.mWeights;
		}

		bool operator!=(const BoneVertexWeights& rhs) const noexcept
		{
			return !operator==(rhs);
		}
		
	private:
		Vector<VertexWeight> mWeights;
	};

    class Bone final : public SceneNode
    {
		RTTI_DECLARATIONS_ABSTRACT(Bone, SceneNode)

    public:
		explicit Bone(InputStreamHelper& streamHelper);
		Bone(const std::string& name, const std::uint32_t index, const glm::mat4x4& offsetTransform);
		Bone(const Bone&) = default;
		Bone(Bone&&) = default;
		Bone& operator=(const Bone&) = default;
		Bone& operator=(Bone&&) = default;
		~Bone() = default;

		std::uint32_t Index() const;
		void SetIndex(const std::uint32_t index);

		const glm::mat4x4& OffsetTransform() const;

		virtual void Save(OutputStreamHelper& streamHelper) override;

    private:
		virtual void Load(InputStreamHelper& streamHelper) override;

		std::uint32_t mIndex{ 0 };		// Index into the model's bone container
		glm::mat4x4 mOffsetTransform;	// Transforms from mesh space to bone space
    };
}
