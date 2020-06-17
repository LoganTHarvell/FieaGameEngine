#pragma once

// Standard
#include <memory>
#include <string>

// ASSIMP Forward Declarations
struct aiMaterial;
struct aiMesh;
struct aiAnimation;
struct aiNode;
struct aiNodeAnim;

namespace Library
{
	// Forward Declarations
	template<typename Key, typename Data> class HashMap;
	struct ModelData;
	class Model;
	class ModelMaterial;
	enum class TextureType;
	class Mesh;
	class AnimationClip;
	class SceneNode;
	class BoneAnimation;
	
	class AssetImporter final
	{
	public:
		AssetImporter() = delete;
		~AssetImporter() = delete;
		AssetImporter(const AssetImporter& rhs) = delete;
		AssetImporter& operator=(const AssetImporter& rhs) = delete;
		AssetImporter(AssetImporter&& rhs) = delete;
		AssetImporter& operator=(AssetImporter&& rhs) = delete;

	public:
		static Model LoadModel(const std::string& filename, const bool flipUVs=false);

	private:
		static std::shared_ptr<ModelMaterial> LoadModelMaterial(Model& model, aiMaterial& material);
		static std::shared_ptr<Mesh> LoadMesh(Model& model, aiMesh& mesh);
		static std::shared_ptr<SceneNode> BuildSkeleton(ModelData& modelData, aiNode& node, const std::shared_ptr<SceneNode>& parentSceneNode);
		static std::shared_ptr<AnimationClip> LoadAnimationClip(Model& model, aiAnimation& animation);
		static std::shared_ptr<BoneAnimation> LoadBoneAnimation(Model& model, aiNodeAnim& nodeAnim);
		
	private:
		static const HashMap<TextureType, std::uint32_t> sTextureTypeMappings;
	};
}
