#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "VertexDeclarations.h"

// First Party
#include "Mesh.h"
#pragma endregion Includes

namespace Library
{
	Buffer* VertexPosition::CreateVertexBuffer(RenderingManager* renderingManager, const Mesh& mesh)
	{		
		const Vector<glm::vec3>& sourceVertices = mesh.Vertices();

		Vector vertices(sourceVertices.Size(), Vector<VertexPosition>::EqualityFunctor());

		for (size_t i = 0; i < sourceVertices.Size(); i++)
		{
			const glm::vec3& position = sourceVertices.At(i);
			vertices.EmplaceBack(glm::vec4(position.x, position.y, position.z, 1.0f));
		}

		return VertexDeclaration::CreateVertexBuffer(renderingManager, gsl::make_span(vertices.Data(), vertices.Size()));
	}

	Buffer* VertexPositionColor::CreateVertexBuffer(RenderingManager* renderingManager, const Mesh& mesh)
	{
		const Vector<glm::vec3>& sourceVertices = mesh.Vertices();

		Vector vertices(sourceVertices.Size(), Vector<VertexPositionColor>::EqualityFunctor());

		assert(mesh.VertexColors().Size() > 0);
		const Vector<glm::vec4>& vertexColors = mesh.VertexColors().At(0);
		assert(vertexColors.Size() == sourceVertices.Size());

		for (size_t i = 0; i < sourceVertices.Size(); i++)
		{
			const glm::vec3& position = sourceVertices.At(i);
			const glm::vec4& color = vertexColors.At(i);
			vertices.EmplaceBack(glm::vec4(position.x, position.y, position.z, 1.0f), color);
		}

		return VertexDeclaration::CreateVertexBuffer(renderingManager, gsl::make_span(vertices.Data(), vertices.Size()));
	}

	Buffer* VertexPositionTexture::CreateVertexBuffer(RenderingManager* renderingManager, const Mesh& mesh)
	{
		const Vector<glm::vec3>& sourceVertices = mesh.Vertices();
		const Vector<glm::vec3>& textureCoordinates = mesh.TextureCoordinates().At(0);
		assert(textureCoordinates.Size() == sourceVertices.Size());

		Vector vertices(sourceVertices.Size(), Vector<VertexPositionTexture>::EqualityFunctor());

		for (size_t i = 0; i < sourceVertices.Size(); i++)
		{
			const glm::vec3& position = sourceVertices.At(i);
			const glm::vec3 &uv = textureCoordinates.At(i);
			vertices.EmplaceBack(glm::vec4(position.x, position.y, position.z, 1.0f), glm::vec2(uv.x, uv.y));
		}

		return VertexDeclaration::CreateVertexBuffer(renderingManager, gsl::make_span(vertices.Data(), vertices.Size()));
	}

	Buffer* VertexPositionNormal::CreateVertexBuffer(RenderingManager* renderingManager, const Mesh& mesh)
	{
		const Vector<glm::vec3>& sourceVertices = mesh.Vertices();
		const Vector<glm::vec3>& sourceNormals = mesh.Normals();
		assert(sourceNormals.Size() == sourceVertices.Size());

		Vector vertices(sourceVertices.Size(), Vector<VertexPositionNormal>::EqualityFunctor());

		for (size_t i = 0; i < sourceVertices.Size(); i++)
		{
			const glm::vec3& position = sourceVertices.At(i);
			const glm::vec3& normal = sourceNormals.At(i);
			vertices.EmplaceBack(glm::vec4(position.x, position.y, position.z, 1.0f), normal);
		}

		return VertexDeclaration::CreateVertexBuffer(renderingManager, gsl::make_span(vertices.Data(), vertices.Size()));
	}

	Buffer* VertexPositionTextureNormal::CreateVertexBuffer(RenderingManager* renderingManager, const Mesh& mesh)
	{
		const Vector<glm::vec3>& sourceVertices = mesh.Vertices();
		const auto& sourceUVs = mesh.TextureCoordinates().At(0);
		assert(sourceUVs.Size() == sourceVertices.Size());
		const auto& sourceNormals = mesh.Normals();
		assert(sourceNormals.Size() == sourceVertices.Size());

		Vector vertices(sourceVertices.Size(), Vector<VertexPositionTextureNormal>::EqualityFunctor());

		for (size_t i = 0; i < sourceVertices.Size(); i++)
		{
			const glm::vec3& position = sourceVertices.At(i);
			const glm::vec3& uv = sourceUVs.At(i);
			const glm::vec3& normal = sourceNormals.At(i);
			vertices.EmplaceBack(glm::vec4(position.x, position.y, position.z, 1.0f), glm::vec2(uv.x, uv.y), normal);
		}

		return VertexDeclaration::CreateVertexBuffer(renderingManager, gsl::make_span(vertices.Data(), vertices.Size()));
	}

	Buffer* VertexPositionTextureNormalTangent::CreateVertexBuffer(RenderingManager* renderingManager, const Mesh& mesh)
	{
		const Vector<glm::vec3>& sourceVertices = mesh.Vertices();
		const auto& sourceUVs = mesh.TextureCoordinates().At(0);
		assert(sourceUVs.Size() == sourceVertices.Size());
		const auto& sourceNormals = mesh.Normals();
		assert(sourceNormals.Size() == sourceVertices.Size());
		const auto& sourceTangents = mesh.Tangents();
		assert(sourceTangents.Size() == sourceVertices.Size());

		Vector vertices(sourceVertices.Size(), Vector<VertexPositionTextureNormalTangent>::EqualityFunctor());
		
		for (size_t i = 0; i < sourceVertices.Size(); i++)
		{
			const glm::vec3& position = sourceVertices.At(i);
			const glm::vec3& uv = sourceUVs.At(i);
			const glm::vec3& normal = sourceNormals.At(i);
			const glm::vec3& tangent = sourceTangents.At(i);
			vertices.EmplaceBack(glm::vec4(position.x, position.y, position.z, 1.0f), glm::vec2(uv.x, uv.y), normal, tangent);
		}

		return VertexDeclaration::CreateVertexBuffer(renderingManager, gsl::make_span(vertices.Data(), vertices.Size()));
	}

	Buffer* VertexSkinnedPositionTextureNormal::CreateVertexBuffer(RenderingManager* renderingManager, const Mesh& mesh)
	{
		const Vector<glm::vec3>& sourceVertices = mesh.Vertices();
		const auto& sourceUVs = mesh.TextureCoordinates().At(0);
		assert(sourceUVs.Size() == sourceVertices.Size());
		const auto& sourceNormals = mesh.Normals();
		assert(sourceNormals.Size() == sourceVertices.Size());
		const auto& boneWeights = mesh.BoneWeights();
		assert(boneWeights.Size() == sourceVertices.Size());

		Vector vertices(sourceVertices.Size(), Vector<VertexSkinnedPositionTextureNormal>::EqualityFunctor());

		for (size_t i = 0; i < sourceVertices.Size(); i++)
		{
			const glm::vec3& position = sourceVertices.At(i);
			const glm::vec3& uv = sourceUVs.At(i);
			const glm::vec3& normal = sourceNormals.At(i);
			const BoneVertexWeights& vertexWeights = boneWeights.At(i);

			float weights[BoneVertexWeights::MaxBoneWeightsPerVertex] = { 0 };
			uint32_t indices[BoneVertexWeights::MaxBoneWeightsPerVertex] = { 0 };
			
			for (size_t j = 0; j < vertexWeights.Weights().Size(); j++)
			{
				const BoneVertexWeights::VertexWeight& vertexWeight = vertexWeights.Weights().At(j);
				weights[j] = vertexWeight.Weight;
				indices[j] = vertexWeight.BoneIndex;
			}

			vertices.EmplaceBack(glm::vec4(position.x, position.y, position.z, 1.0f), glm::vec2(uv.x, uv.y), normal, glm::make_vec4(indices), glm::make_vec4(weights));
		}

		return VertexDeclaration::CreateVertexBuffer(renderingManager, gsl::make_span(vertices.Data(), vertices.Size()));
	}
}