#pragma once

#pragma region Includes
// First Party
#include "Entity.h"
#pragma endregion Includes

namespace Library
{
	// Forward Declarations
	class Mesh;
	class ModelMaterial;
	class SceneNode;
	class Bone;
	class AnimationClip;
	class OutputStreamHelper;
	class InputStreamHelper;

	/// <summary>
	/// Represents the data encapsulated within a Model.
	/// </summary>
	struct ModelData final
	{
		Vector<std::shared_ptr<Mesh>> Meshes;
		Vector<std::shared_ptr<ModelMaterial>> Materials;
		Vector<std::shared_ptr<AnimationClip>> Animations;
		HashMap<std::string, std::shared_ptr<AnimationClip>> AnimationsByName;
		Vector<std::shared_ptr<Bone>> Bones;
		HashMap<std::string, std::uint32_t> BoneIndexMapping;
		std::shared_ptr<SceneNode> RootNode;
	};

	/// <summary>
	/// Component for providing a physical representation of an Entity as a 3D model.
	/// </summary>
	class Model final : public Entity
	{
		RTTI_DECLARATIONS(Model, Entity)

#pragma region Special Members
	public:
		explicit Model(std::string name=std::string());
		~Model() = default;
		Model(const Model& rhs) = default;
		Model& operator=(const Model& rhs) = default;
		Model(Model&& rhs) = default;
		Model& operator=(Model&& rhs) = default;

		Model(const std::string& filename, std::string name);
		explicit Model(std::ifstream& file, std::string name=std::string());
		explicit Model(ModelData&& modelData, std::string name=std::string());
#pragma endregion Special Members

#pragma region Acessors
	public:
		bool HasMeshes() const;
		bool HasMaterials() const;
		bool HasAnimations() const;

		const Vector<std::shared_ptr<Mesh>>& Meshes() const;
		const Vector<std::shared_ptr<ModelMaterial>>& Materials() const;
		const Vector<std::shared_ptr<AnimationClip>>& Animations() const;
		const HashMap<std::string, std::shared_ptr<AnimationClip>>& AnimationsByName() const;
		const Vector<std::shared_ptr<Bone>>& Bones() const;
		const HashMap<std::string, std::uint32_t>& BoneIndexMapping() const;
		std::shared_ptr<const SceneNode> RootNode() const;

		ModelData& Data();
		const ModelData& Data() const;
#pragma endregion Acessors

#pragma region Asset Management
	public:
		void Save(const std::string& filename) const;
		void Save(std::ofstream& file) const;

	private:
		void Load(const std::string& filename);
		void Load(std::ifstream& file);

		void SaveSkeleton(OutputStreamHelper & streamHelper, const std::shared_ptr<const SceneNode> & sceneNode) const;
		std::shared_ptr<SceneNode> LoadSkeleton(InputStreamHelper & streamHelper, std::shared_ptr<SceneNode> parentSceneNode);
#pragma endregion Asset Management

		ModelData mData;
	};

#pragma region ModelFactory Declaration
	ConcreteFactory(Model, Entity)
#pragma endregion ModelFactory Declaration
}
