#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header File
#include "AssetImporter.h"

// Third Party
#pragma warning(disable : 26812)
#pragma warning(disable : 26451)
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#pragma warning(default : 26451)
#pragma warning(default : 26812)

// First Party
#include "Model.h"
#include "Bone.h"
#include "AnimationClip.h"
#pragma endregion Includes

using namespace std::string_literals;

namespace Library
{
	Model AssetImporter::LoadModel(const std::string& filename, const bool flipUVs)
	{
		Assimp::Importer importer;
		
		Model model;
		ModelData& modelData = model.Data();

		uint32_t flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_FlipWindingOrder;

		if (flipUVs)
		{
			flags |= aiProcess_FlipUVs;
		}

		const aiScene* scene = importer.ReadFile(filename, flags);
		
		if (scene == nullptr)
		{
			throw std::runtime_error(importer.GetErrorString());
		}

		if (scene->HasMaterials())
		{
			for (unsigned int i = 0; i < scene->mNumMaterials; i++)
			{
				std::shared_ptr<ModelMaterial> modelMaterial = LoadModelMaterial(model, *(scene->mMaterials[i]));
				modelData.Materials.EmplaceBack(modelMaterial);
			}
		}

		if (scene->HasMeshes())
		{
			for (unsigned int i = 0; i < scene->mNumMeshes; i++)
			{
				std::shared_ptr<Mesh> mesh = LoadMesh(model, *(scene->mMeshes[i]));
				modelData.Meshes.EmplaceBack(mesh);
			}
		}

		if (scene->HasAnimations())
		{
			if (scene->mRootNode == nullptr) throw std::runtime_error("Attempted to load animations, mesh root node is null."s);

			modelData.RootNode = BuildSkeleton(modelData, *scene->mRootNode, nullptr);

			modelData.Animations.Reserve(scene->mNumAnimations);
			for (unsigned int i = 0; i < scene->mNumAnimations; i++)
			{
				std::shared_ptr<AnimationClip> animation = LoadAnimationClip(model, *(scene->mAnimations[i]));
				modelData.Animations.EmplaceBack(animation);
				modelData.AnimationsByName.Emplace(std::pair<std::string, std::shared_ptr<AnimationClip>>(animation->Name(), animation));
			}
		}

		return model;
	}

	std::shared_ptr<SceneNode> AssetImporter::BuildSkeleton(ModelData& modelData, aiNode& node, const std::shared_ptr<SceneNode>& parentSceneNode)
	{
		std::shared_ptr<SceneNode> sceneNode;

		const auto boneMapping = modelData.BoneIndexMapping.Find(node.mName.C_Str());

		if (boneMapping == modelData.BoneIndexMapping.end())
		{
			sceneNode = std::make_shared<SceneNode>(node.mName.C_Str());
		}
		else
		{
			sceneNode = modelData.Bones[boneMapping->second];
		}

		const glm::mat4x4 transform = glm::make_mat4x4(&node.mTransformation.a1);
		sceneNode->SetTransform(transform);
		sceneNode->SetParent(parentSceneNode);

		auto& children = sceneNode->Children();
		children.Reserve(node.mNumChildren);
		
		for (unsigned int i = 0; i < node.mNumChildren; i++)
		{
			std::shared_ptr<SceneNode> childSceneNode = BuildSkeleton(modelData, *(node.mChildren[i]), sceneNode);
			children.EmplaceBack(childSceneNode);
		}

		return sceneNode;
	}
}
