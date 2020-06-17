#pragma once

#pragma region Includes
// First Party
#include "Bone.h"
#pragma endregion Includes

namespace Library
{
	class Model;
    class Material;
    class ModelMaterial;
	class OutputStreamHelper;
	class InputStreamHelper;

	struct MeshData final
	{
		std::shared_ptr<ModelMaterial> Material;
		std::string Name;
		Vector<glm::vec3> Vertices;
		Vector<glm::vec3> Normals;
		Vector<glm::vec3> Tangents;
		Vector<glm::vec3> BiNormals;
		Vector<Vector<glm::vec3>> TextureCoordinates;
		Vector<Vector<glm::vec4>> VertexColors;
		std::uint32_t FaceCount{ 0 };
		Vector<std::uint32_t> Indices;
		Vector<BoneVertexWeights> BoneWeights;
	};

    class Mesh final
    {
    public:
		Mesh(Model& model, InputStreamHelper& streamHelper);
		Mesh(Model& model, MeshData&& meshData);
		Mesh(const Mesh&) = default;
		Mesh(Mesh&&) = default;
		Mesh& operator=(const Mesh&) = default;
		Mesh& operator=(Mesh&&) = default;
		~Mesh() = default;

		Model& GetModel();
		const Model& GetModel() const;
        std::shared_ptr<ModelMaterial> GetMaterial();
        std::shared_ptr<const ModelMaterial> GetMaterial() const;
        const std::string& Name() const;

		const Vector<glm::vec3>& Vertices() const;
		const Vector<glm::vec3>& Normals() const;
		const Vector<glm::vec3>& Tangents() const;
		const Vector<glm::vec3>& BiNormals() const;
		const Vector<Vector<glm::vec3>>& TextureCoordinates() const;
		const Vector<Vector<glm::vec4>>& VertexColors() const;
		std::uint32_t FaceCount() const;
		const Vector<std::uint32_t>& Indices() const;
		const Vector<BoneVertexWeights>& BoneWeights() const;

		void Save(OutputStreamHelper& streamHelper) const;

    private:
		void Load(InputStreamHelper& streamHelper);

        gsl::not_null<Model*> mModel;
		MeshData mData;
    };
}