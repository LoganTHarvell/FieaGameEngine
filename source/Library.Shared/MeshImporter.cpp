#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "AssetImporter.h"

// Third Party
#pragma warning(disable : 26812)
#pragma warning(disable : 26451)
#include <assimp/mesh.h>
#pragma warning(default : 26451)
#pragma warning(default : 26812)

// First Party
#include "Model.h"
#include "Mesh.h"
#pragma endregion Includes

namespace Library
{
	std::shared_ptr<Mesh> AssetImporter::LoadMesh(Model& model, aiMesh& mesh)
	{
		MeshData meshData;

		meshData.Material = model.Materials().At(mesh.mMaterialIndex);

		// Vertices
		{
			meshData.Vertices.Reserve(mesh.mNumVertices);
			for (unsigned int i = 0; i < mesh.mNumVertices; i++)
			{
				meshData.Vertices.EmplaceBack(glm::make_vec3(reinterpret_cast<const float*>(&mesh.mVertices[i])));
			}
		}

		// Normals
		if (mesh.HasNormals())
		{
			meshData.Normals.Reserve(mesh.mNumVertices);
			for (unsigned int i = 0; i < mesh.mNumVertices; i++)
			{
				meshData.Normals.EmplaceBack(glm::make_vec3(reinterpret_cast<const float*>(&mesh.mNormals[i])));
			}
		}

		// Tangents and Bi-Normals
		if (mesh.HasTangentsAndBitangents())
		{
			meshData.Tangents.Reserve(mesh.mNumVertices);
			meshData.BiNormals.Reserve(mesh.mNumVertices);
			for (unsigned int i = 0; i < mesh.mNumVertices; i++)
			{
				meshData.Tangents.EmplaceBack(glm::make_vec3(reinterpret_cast<const float*>(&mesh.mTangents[i])));
				meshData.BiNormals.EmplaceBack(glm::make_vec3(reinterpret_cast<const float*>(&mesh.mBitangents[i])));
			}
		}

		// Texture Coordinates
		{
			const unsigned int uvChannelCount = mesh.GetNumUVChannels();
			meshData.TextureCoordinates.Reserve(uvChannelCount);
			for (unsigned int i = 0; i < uvChannelCount; i++)
			{
				Vector<glm::vec3> textureCoordinates;
				textureCoordinates.Reserve(mesh.mNumVertices);
				aiVector3D* aiTextureCoordinates = mesh.mTextureCoords[i];
				for (unsigned int j = 0; j < mesh.mNumVertices; j++)
				{
					textureCoordinates.EmplaceBack(glm::make_vec3(reinterpret_cast<const float*>(&aiTextureCoordinates[j])));
				}

				meshData.TextureCoordinates.EmplaceBack(std::move(textureCoordinates));
			}
		}

		// Vertex Colors
		{
			const unsigned int colorChannelCount = mesh.GetNumColorChannels();
			meshData.VertexColors.Reserve(colorChannelCount);
			for (unsigned int i = 0; i < colorChannelCount; i++)
			{
				Vector<glm::vec4> vertexColors;
				vertexColors.Reserve(mesh.mNumVertices);
				aiColor4D* aiVertexColors = mesh.mColors[i];
				for (unsigned int j = 0; j < mesh.mNumVertices; j++)
				{
					vertexColors.EmplaceBack(glm::make_vec4(reinterpret_cast<const float*>(&aiVertexColors[j])));
				}
				meshData.VertexColors.EmplaceBack(std::move(vertexColors));
			}
		}

		// Faces (note: could pre-Reserve if we limit primitive types)
		if (mesh.HasFaces())
		{
			meshData.FaceCount = mesh.mNumFaces;
			for (uint32_t i = 0; i < meshData.FaceCount; i++)
			{
				aiFace* face = &mesh.mFaces[i];

				for (unsigned int j = 0; j < face->mNumIndices; j++)
				{
					meshData.Indices.EmplaceBack(face->mIndices[j]);
				}
			}
		}

		// Bones
		if (mesh.HasBones())
		{
			meshData.BoneWeights.Resize(mesh.mNumVertices);

			for (unsigned int i = 0; i < mesh.mNumBones; i++)
			{
				aiBone* meshBone = mesh.mBones[i];

				// Look up the bone in the model's hierarchy, or add it if not found.
				uint32_t boneIndex = 0U;
				std::string boneName = meshBone->mName.C_Str();
				auto boneMappingIterator = model.Data().BoneIndexMapping.Find(boneName);
				if (boneMappingIterator != model.Data().BoneIndexMapping.end())
				{
					boneIndex = boneMappingIterator->second;
				}
				else
				{
					boneIndex = gsl::narrow_cast<uint32_t>(model.Bones().Size());
					glm::mat4x4 offset = glm::make_mat4x4(&meshBone->mOffsetMatrix.a1);

					model.Data().Bones.EmplaceBack(std::make_shared<Bone>(boneName, boneIndex, offset));
					model.Data().BoneIndexMapping[boneName] = boneIndex;
				}

				for (unsigned int j = 0; j < meshBone->mNumWeights; j++)
				{
					const aiVertexWeight vertexWeight = meshBone->mWeights[j];
					meshData.BoneWeights[vertexWeight.mVertexId].AddWeight(vertexWeight.mWeight, boneIndex);
				}
			}
		}

		return std::make_shared<Mesh>(model, std::move(meshData));
	}
}
