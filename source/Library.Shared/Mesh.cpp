#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "Mesh.h"

// First Party
#include "StreamHelper.h"
#include "ModelMaterial.h"
#include "Model.h"
#include "Bone.h"
#pragma endregion Includes

namespace Library
{
	Mesh::Mesh(Model& model, InputStreamHelper& streamHelper) :
		mModel(&model)
	{
		Load(streamHelper);
	}

	Mesh::Mesh(Model& model, MeshData&& meshData) :
		mModel(&model), mData(std::move(meshData))
	{
	}

	Model& Mesh::GetModel()
	{
		return *mModel;
	}

	const Library::Model& Mesh::GetModel() const
	{
		return *mModel;
	}

	std::shared_ptr<ModelMaterial> Mesh::GetMaterial()
	{
		return mData.Material;
	}

	std::shared_ptr<const ModelMaterial> Mesh::GetMaterial() const
	{
		return mData.Material;
	}

	const std::string& Mesh::Name() const
	{
		return mData.Name;
	}

	const Vector<glm::vec3>& Mesh::Vertices() const
	{
		return mData.Vertices;
	}

	const Vector<glm::vec3>& Mesh::Normals() const
	{
		return mData.Normals;
	}

	const Vector<glm::vec3>& Mesh::Tangents() const
	{
		return mData.Tangents;
	}

	const Vector<glm::vec3>& Mesh::BiNormals() const
	{
		return mData.BiNormals;
	}

	const Vector<Vector<glm::vec3>>& Mesh::TextureCoordinates() const
	{
		return mData.TextureCoordinates;
	}

	const Vector<Vector<glm::vec4>>& Mesh::VertexColors() const
	{
		return mData.VertexColors;
	}

	uint32_t Mesh::FaceCount() const
	{
		return mData.FaceCount;
	}

	const Vector<uint32_t>& Mesh::Indices() const
	{
		return mData.Indices;
	}

	const Vector<BoneVertexWeights>& Mesh::BoneWeights() const
	{
		return mData.BoneWeights;
	}

	void Mesh::Save(OutputStreamHelper& streamHelper) const
	{
		const std::string materialName = (mData.Material != nullptr ? mData.Material->Name() : "");
		streamHelper << materialName;

		// Serialize name
		streamHelper << mData.Name;

		// Serialize vertices
		streamHelper << gsl::narrow_cast<uint32_t>(mData.Vertices.Size());
		for (const glm::vec3& vertex : mData.Vertices)
		{
			streamHelper << vertex.x << vertex.y << vertex.z;
		}

		// Serialize normals
		streamHelper << gsl::narrow_cast<uint32_t>(mData.Normals.Size());
		for (const glm::vec3& normal : mData.Normals)
		{
			streamHelper << normal.x << normal.y << normal.z;
		}

		// Serialize tangents
		streamHelper << gsl::narrow_cast<uint32_t>(mData.Tangents.Size());
		for (const glm::vec3& tangents : mData.Tangents)
		{
			streamHelper << tangents.x << tangents.y << tangents.z;
		}

		// Serialize bi-normals
		streamHelper << gsl::narrow_cast<uint32_t>(mData.BiNormals.Size());
		for (const glm::vec3& biNormal : mData.BiNormals)
		{
			streamHelper << biNormal.x << biNormal.y << biNormal.z;
		}

		// Serialize texture coordinates
		streamHelper << gsl::narrow_cast<uint32_t>(mData.TextureCoordinates.Size());
		for (const auto& uvList : mData.TextureCoordinates)
		{
			streamHelper << gsl::narrow_cast<uint32_t>(uvList.Size());
			for (const glm::vec3& uv : uvList)
			{
				streamHelper << uv.x << uv.y << uv.z;
			}
		}

		// Serialize vertex colors
		streamHelper << gsl::narrow_cast<uint32_t>(mData.VertexColors.Size());
		for (const auto& vertexColorList : mData.VertexColors)
		{
			streamHelper << vertexColorList.Size();
			for (const glm::vec4& vertexColor : vertexColorList)
			{
				streamHelper << vertexColor.x << vertexColor.y << vertexColor.z << vertexColor.w;
			}
		}

		// Serialize indices
		streamHelper << mData.FaceCount;
		streamHelper << gsl::narrow_cast<uint32_t>(mData.Indices.Size());
		for (const uint32_t& index : mData.Indices)
		{
			streamHelper << index;
		}

		// Serialize bone weights
		streamHelper << gsl::narrow_cast<uint32_t>(mData.BoneWeights.Size());
		for (const BoneVertexWeights& boneVertexWeight : mData.BoneWeights)
		{
			streamHelper << gsl::narrow_cast<uint32_t>(boneVertexWeight.Weights().Size());
			for (const BoneVertexWeights::VertexWeight& weight : boneVertexWeight.Weights())
			{
				streamHelper << weight.Weight << weight.BoneIndex;
			}
		}
	}

	void Mesh::Load(InputStreamHelper& streamHelper)
	{
		// Deserialize material reference
		{
			std::string materialName;
			streamHelper >> materialName;

			for (const auto& material : mModel->Materials())
			{
				if (material->Name() == materialName)
				{
					mData.Material = material;
					break;
				}
			}
		}

		// Deserialize name
		streamHelper >> mData.Name;

		// Deserialize vertices
		{
			uint32_t vertexCount;
			streamHelper >> vertexCount;
			mData.Vertices.Reserve(vertexCount);
			for (uint32_t i = 0; i < vertexCount; i++)
			{
				glm::vec3 vertex;
				streamHelper >> vertex.x >> vertex.y >> vertex.z;
				mData.Vertices.EmplaceBack(vertex);
			}
		}

		// Deserialize normals
		{
			uint32_t normalCount;
			streamHelper >> normalCount;
			mData.Normals.Reserve(normalCount);
			for (uint32_t i = 0; i < normalCount; i++)
			{
				glm::vec3 normal;
				streamHelper >> normal.x >> normal.y >> normal.z;
				mData.Normals.EmplaceBack(normal);
			}
		}

		// Deserialize tangents
		{
			uint32_t tangentCount;
			streamHelper >> tangentCount;
			mData.Tangents.Reserve(tangentCount);
			for (uint32_t i = 0; i < tangentCount; i++)
			{
				glm::vec3 tangent;
				streamHelper >> tangent.x >> tangent.y >> tangent.z;
				mData.Tangents.EmplaceBack(tangent);
			}
		}

		// Deserialize bi-normals
		{
			uint32_t biNormalCount;
			streamHelper >> biNormalCount;
			mData.BiNormals.Reserve(biNormalCount);
			for (uint32_t i = 0; i < biNormalCount; i++)
			{
				glm::vec3 biNormal;
				streamHelper >> biNormal.x >> biNormal.y >> biNormal.z;
				mData.BiNormals.EmplaceBack(biNormal);
			}
		}

		// Deserialize texture coordinates
		{
			uint32_t textureCoordinateCount;
			streamHelper >> textureCoordinateCount;
			mData.TextureCoordinates.Reserve(textureCoordinateCount);
			for (uint32_t i = 0; i < textureCoordinateCount; i++)
			{
				uint32_t uvListCount;
				streamHelper >> uvListCount;
				if (uvListCount > 0)
				{
					Vector<glm::vec3> uvs;
					uvs.Reserve(uvListCount);
					for (uint32_t j = 0; j < uvListCount; j++)
					{
						glm::vec3 uv;
						streamHelper >> uv.x >> uv.y >> uv.z;
						uvs.EmplaceBack(uv);
					}

					mData.TextureCoordinates.EmplaceBack(std::move(uvs));
				}
			}
		}

		// Deserialize vertex colors
		{
			uint32_t vertexColorCount;
			streamHelper >> vertexColorCount;
			mData.VertexColors.Reserve(vertexColorCount);
			for (uint32_t i = 0; i < vertexColorCount; i++)
			{
				uint32_t vertexColorListCount;
				streamHelper >> vertexColorListCount;
				if (vertexColorListCount > 0)
				{
					Vector<glm::vec4> vertexColors;
					vertexColors.Reserve(vertexColorListCount);
					for (uint32_t j = 0; j < vertexColorListCount; j++)
					{
						glm::vec4 vertexColor;
						streamHelper >> vertexColor.x >> vertexColor.y >> vertexColor.z >> vertexColor.w;
						vertexColors.EmplaceBack(vertexColor);
					}

					mData.VertexColors.EmplaceBack(std::move(vertexColors));
				}
			}
		}

		// Deserialize indexes	
		{
			streamHelper >> mData.FaceCount;
			uint32_t indexCount;
			streamHelper >> indexCount;
			mData.Indices.Reserve(indexCount);
			for (uint32_t i = 0; i < indexCount; i++)
			{
				uint32_t index;
				streamHelper >> index;
				mData.Indices.EmplaceBack(index);
			}
		}

		// Deserialize bone weights
		{
			uint32_t boneVertexWeightCount;
			streamHelper >> boneVertexWeightCount;
			mData.BoneWeights.Resize(boneVertexWeightCount);
			for (uint32_t i = 0; i < boneVertexWeightCount; i++)
			{
				uint32_t weightCount;
				streamHelper >> weightCount;
				mData.BoneWeights.Reserve(weightCount);
				for (uint32_t j = 0; j < weightCount; j++)
				{
					float weight;
					uint32_t boneIndex;
					streamHelper >> weight >> boneIndex;

					mData.BoneWeights[i].AddWeight(weight, boneIndex);
				}
			}
		}
	}
}