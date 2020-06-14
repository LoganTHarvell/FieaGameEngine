#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "Model.h"

// First Party
#include "StreamHelper.h"
#include "Mesh.h"
#include "SceneNode.h"
#include "ModelMaterial.h"
#include "AnimationClip.h"
#pragma endregion Includes

namespace Library
{
	Model::Model(std::string name) : Entity(Model::TypeIdClass(), std::move(name))
	{
	}

	Model::Model(const std::string& filename, std::string name) : Entity(Model::TypeIdClass(), std::move(name))
	{
		Load(filename);
	}

	Model::Model(std::ifstream& file, std::string name) : Entity(Model::TypeIdClass(), std::move(name))
	{
		Load(file);
	}

	Model::Model(ModelData&& modelData, std::string name) : Entity(Model::TypeIdClass(), std::move(name)),
		mData(std::move(modelData))
	{
	}

	bool Model::HasMeshes() const
	{
		return (mData.Meshes.Size() > 0);
	}

	bool Model::HasMaterials() const
	{
		return (mData.Materials.Size() > 0);
	}

	bool Model::HasAnimations() const
	{
		return (mData.Animations.Size() > 0);
	}

	const Vector<std::shared_ptr<Mesh>>& Model::Meshes() const
	{
		return mData.Meshes;
	}

	const Vector<std::shared_ptr<ModelMaterial>>& Model::Materials() const
	{
		return mData.Materials;
	}

	const Vector<std::shared_ptr<AnimationClip>>& Model::Animations() const
	{
		return mData.Animations;
	}

	const HashMap<std::string, std::shared_ptr<AnimationClip>>& Model::AnimationsByName() const
	{
		return mData.AnimationsByName;
	}

	const Vector<std::shared_ptr<Bone>>& Model::Bones() const
	{
		return mData.Bones;
	}

	const HashMap<std::string, std::uint32_t>& Model::BoneIndexMapping() const
	{
		return mData.BoneIndexMapping;
	}

	std::shared_ptr<const SceneNode> Model::RootNode() const
	{
		return mData.RootNode;
	}

	ModelData& Model::Data()
	{
		return mData;
	}

	const ModelData& Model::Data() const
	{
		return mData;
	}

	void Model::Save(const std::string& filename) const
	{
		std::ofstream file(filename.c_str(), std::ios::binary);
		if (!file.is_open())
		{
			throw std::runtime_error("Could not open file.");
		}

		Save(file);
	}

	void Model::Save(std::ofstream& file) const
	{
		OutputStreamHelper streamHelper(file);

		// Serialize materials
		streamHelper << gsl::narrow_cast<std::uint32_t>(mData.Materials.Size());
		for (const auto& material : mData.Materials)
		{
			material->Save(streamHelper);
		}

		// Serialize meshes
		streamHelper << gsl::narrow_cast<std::uint32_t>(mData.Meshes.Size());
		for (const auto& mesh : mData.Meshes)
		{
			mesh->Save(streamHelper);
		}

		// Serialize bones
		streamHelper << gsl::narrow_cast<std::uint32_t>(mData.Bones.Size());
		for (const auto& bone : mData.Bones)
		{
			bone->Save(streamHelper);
		}

		// Serialize skeleton hierarchy
		const bool hasSkeleton = mData.RootNode != nullptr;
		streamHelper << hasSkeleton;
		if (hasSkeleton)
		{
			SaveSkeleton(streamHelper, mData.RootNode);
		}

		// Serialize animations
		streamHelper << gsl::narrow_cast<std::uint32_t>(mData.Animations.Size());
		for (const auto& animation : mData.Animations)
		{
			animation->Save(streamHelper);
		}
	}

	void Model::Load(const std::string& filename)
	{
		std::ifstream file(filename.c_str(), std::ios::binary);
		if (!file.is_open())
		{
			throw std::runtime_error("Could not open file.");
		}

		Load(file);
	}

	void Model::Load(std::ifstream& file)
	{
		InputStreamHelper streamHelper(file);

		// Deserialize materials
		std::uint32_t materialCount;
		streamHelper >> materialCount;
		mData.Materials.Reserve(materialCount);
		for (std::uint32_t i = 0; i < materialCount; i++)
		{
			mData.Materials.EmplaceBack(std::make_shared<ModelMaterial>(*this, streamHelper));
		}

		// Deserialize meshes
		std::uint32_t meshCount;
		streamHelper >> meshCount;
		mData.Meshes.Reserve(meshCount);
		for (std::uint32_t i = 0; i < meshCount; i++)
		{
			mData.Meshes.EmplaceBack(std::make_shared<Mesh>(*this, streamHelper));
		}

		// Deserialize bones
		std::uint32_t boneCount;
		streamHelper >> boneCount;
		mData.Bones.Reserve(boneCount);
		for (std::uint32_t index = 0; index < boneCount; index++)
		{
			const auto& bone = mData.Bones.EmplaceBack(std::make_shared<Bone>(streamHelper));
			mData.BoneIndexMapping[bone->Name()] = index;
		}

		// Deserialize skeleton hierarchy
		bool hasSkeleton;
		streamHelper >> hasSkeleton;
		if (hasSkeleton)
		{
			mData.RootNode = LoadSkeleton(streamHelper, nullptr);
		}

		// Deserialize animations
		std::uint32_t animationCount;
		streamHelper >> animationCount;
		mData.Animations.Reserve(animationCount);
		for (std::uint32_t i = 0; i < animationCount; i++)
		{
			const auto& animation = mData.Animations.EmplaceBack(std::make_shared<AnimationClip>(*this, streamHelper));
			mData.AnimationsByName[animation->Name()] = animation;
		}
	}

	void Model::SaveSkeleton(OutputStreamHelper& streamHelper, const std::shared_ptr<const SceneNode>& sceneNode) const
	{
		streamHelper << sceneNode->Name();

		const bool isBone = sceneNode->Is(Bone::TypeIdClass());
		streamHelper << isBone;
		if (isBone == false)
		{
			streamHelper << sceneNode->GetTransform();
		}

		streamHelper << gsl::narrow_cast<std::uint32_t>(sceneNode->Children().Size());
		for (const auto& childNode : sceneNode->Children())
		{
			SaveSkeleton(streamHelper, childNode);
		}
	}

	std::shared_ptr<SceneNode> Model::LoadSkeleton(InputStreamHelper& streamHelper, std::shared_ptr<SceneNode> parentSceneNode)
	{
		std::shared_ptr<SceneNode> sceneNode;

		// Deserialize bone's name for lookup
		std::string name;
		streamHelper >> name;

		bool isBone;
		streamHelper >> isBone;

		if (isBone)
		{
			const std::uint32_t boneIndex = mData.BoneIndexMapping.At(name);
			sceneNode = mData.Bones[boneIndex];
		}
		else
		{
			glm::mat4x4 transform;
			streamHelper >> transform;
			sceneNode = std::make_shared<SceneNode>(name, transform);
		}

		sceneNode->SetParent(std::move(parentSceneNode));

		// Deserialize children
		std::uint32_t childCount;
		streamHelper >> childCount;
		sceneNode->Children().Reserve(childCount);
		for (std::uint32_t i = 0; i < childCount; i++)
		{
			auto childSceneNode = LoadSkeleton(streamHelper, sceneNode);
			sceneNode->Children().EmplaceBack(childSceneNode);
		}

		return sceneNode;
	}
}
