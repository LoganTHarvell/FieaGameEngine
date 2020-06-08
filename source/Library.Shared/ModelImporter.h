#pragma once

// Standard
#include <memory>
#include <string>

// ASSIMP Forward Declarations
struct aiNode;
struct aiMaterial;

namespace Library
{
	// Forward Declarations
	struct ModelData;
	class Model;
	class ModelMaterial;
	class SceneNode;
	class TextureType;
	template<typename Key, typename Data> class HashMap;
	
	class ModelImporter final
	{
	public:
		ModelImporter() = delete;
		~ModelImporter() = delete;
		ModelImporter(const ModelImporter& rhs) = delete;
		ModelImporter& operator=(const ModelImporter& rhs) = delete;
		ModelImporter(ModelImporter&& rhs) = delete;
		ModelImporter& operator=(ModelImporter&& rhs) = delete;

	public:
		static class Model Load(const std::string& filename, const bool flipUVs=false);
		static std::shared_ptr<ModelMaterial> LoadModelMaterial(Model& model, aiMaterial& material);

	private:
		static std::shared_ptr<SceneNode> BuildSkeleton(ModelData& modelData, aiNode& node, const std::shared_ptr<SceneNode>& parentSceneNode);
		
	private:
		static const HashMap<TextureType, std::uint32_t> sTextureTypeMappings;
	};
}
