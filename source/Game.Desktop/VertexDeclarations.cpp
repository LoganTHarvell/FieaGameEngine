#include "pch.h"
#include "VertexDeclarations.h"
#include "GameException.h"
#include "Mesh.h"

using namespace std;
using namespace gsl;
using namespace DirectX;

namespace Library
{
	void VertexPosition::CreateVertexBuffer(not_null<ID3D11Device*> device, const Mesh& mesh, not_null<ID3D11Buffer**> vertexBuffer)
	{
		const Vector<glm::vec3>& sourceVertices = mesh.Vertices();

		vector<VertexPosition> vertices;
		vertices.reserve(sourceVertices.Size());

		for (size_t i = 0; i < sourceVertices.Size(); i++)
		{
			const glm::vec3& position = sourceVertices.At(i);
			vertices.emplace_back(XMFLOAT4(position.x, position.y, position.z, 1.0f));
		}

		VertexDeclaration::CreateVertexBuffer(device, vertices, vertexBuffer);
	}

	void VertexPositionColor::CreateVertexBuffer(not_null<ID3D11Device*> device, const Mesh& mesh, not_null<ID3D11Buffer**> vertexBuffer)
	{
		const Vector<glm::vec3>& sourceVertices = mesh.Vertices();

		vector<VertexPositionColor> vertices;
		vertices.reserve(sourceVertices.Size());

		assert(mesh.VertexColors().Size() > 0);
		const Vector<glm::vec4>& vertexColors = mesh.VertexColors().At(0);
		assert(vertexColors.Size() == sourceVertices.Size());

		for (size_t i = 0; i < sourceVertices.Size(); i++)
		{
			const glm::vec3& position = sourceVertices.At(i);
			const glm::vec4& color = vertexColors.At(i);
			vertices.emplace_back(XMFLOAT4(position.x, position.y, position.z, 1.0f), XMFLOAT4(&color[0]));
		}

		VertexDeclaration::CreateVertexBuffer(device, vertices, vertexBuffer);
	}

	void VertexPositionTexture::CreateVertexBuffer(not_null<ID3D11Device*> device, const Mesh& mesh, not_null<ID3D11Buffer**> vertexBuffer)
	{
		const Vector<glm::vec3>& sourceVertices = mesh.Vertices();
		const Vector<glm::vec3>& textureCoordinates = mesh.TextureCoordinates().At(0);
		assert(textureCoordinates.Size() == sourceVertices.Size());

		vector<VertexPositionTexture> vertices;
		vertices.reserve(sourceVertices.Size());
		for (size_t i = 0; i < sourceVertices.Size(); i++)
		{
			const glm::vec3& position = sourceVertices.At(i);
			const glm::vec3 &uv = textureCoordinates.At(i);
			vertices.emplace_back(XMFLOAT4(position.x, position.y, position.z, 1.0f), XMFLOAT2(uv.x, uv.y));
		}

		VertexDeclaration::CreateVertexBuffer(device, vertices, vertexBuffer);
	}

	void VertexPositionNormal::CreateVertexBuffer(not_null<ID3D11Device*> device, const Mesh& mesh, not_null<ID3D11Buffer**> vertexBuffer)
	{
		const Vector<glm::vec3>& sourceVertices = mesh.Vertices();
		const Vector<glm::vec3>& sourceNormals = mesh.Normals();
		assert(sourceNormals.Size() == sourceVertices.Size());

		vector<VertexPositionNormal> vertices;
		vertices.reserve(sourceVertices.Size());
		for (size_t i = 0; i < sourceVertices.Size(); i++)
		{
			const glm::vec3& position = sourceVertices.At(i);
			const glm::vec3& normal = sourceNormals.At(i);
			vertices.emplace_back(XMFLOAT4(position.x, position.y, position.z, 1.0f), XMFLOAT3(&normal[0]));
		}

		VertexDeclaration::CreateVertexBuffer(device, vertices, vertexBuffer);
	}

	void VertexPositionTextureNormal::CreateVertexBuffer(not_null<ID3D11Device*> device, const Mesh& mesh, not_null<ID3D11Buffer**> vertexBuffer)
	{
		const Vector<glm::vec3>& sourceVertices = mesh.Vertices();
		const auto& sourceUVs = mesh.TextureCoordinates().At(0);
		assert(sourceUVs.Size() == sourceVertices.Size());
		const auto& sourceNormals = mesh.Normals();
		assert(sourceNormals.Size() == sourceVertices.Size());

		vector<VertexPositionTextureNormal> vertices;
		vertices.reserve(sourceVertices.Size());
		for (size_t i = 0; i < sourceVertices.Size(); i++)
		{
			const glm::vec3& position = sourceVertices.At(i);
			const glm::vec3& uv = sourceUVs.At(i);
			const glm::vec3& normal = sourceNormals.At(i);
			vertices.emplace_back(XMFLOAT4(position.x, position.y, position.z, 1.0f), XMFLOAT2(uv.x, uv.y), XMFLOAT3(&normal[0]));
		}

		VertexDeclaration::CreateVertexBuffer(device, vertices, vertexBuffer);
	}

	void VertexPositionTextureNormalTangent::CreateVertexBuffer(not_null<ID3D11Device*> device, const Mesh& mesh, not_null<ID3D11Buffer**> vertexBuffer)
	{
		const Vector<glm::vec3>& sourceVertices = mesh.Vertices();
		const auto& sourceUVs = mesh.TextureCoordinates().At(0);
		assert(sourceUVs.Size() == sourceVertices.Size());
		const auto& sourceNormals = mesh.Normals();
		assert(sourceNormals.Size() == sourceVertices.Size());
		const auto& sourceTangents = mesh.Tangents();
		assert(sourceTangents.Size() == sourceVertices.Size());

		vector<VertexPositionTextureNormalTangent> vertices;
		vertices.reserve(sourceVertices.Size());
		for (size_t i = 0; i < sourceVertices.Size(); i++)
		{
			const glm::vec3& position = sourceVertices.At(i);
			const glm::vec3& uv = sourceUVs.At(i);
			const glm::vec3& normal = sourceNormals.At(i);
			const glm::vec3& tangent = sourceTangents.At(i);
			vertices.emplace_back(XMFLOAT4(position.x, position.y, position.z, 1.0f), XMFLOAT2(uv.x, uv.y), XMFLOAT3(&normal[0]), XMFLOAT3(&tangent[0]));
		}

		VertexDeclaration::CreateVertexBuffer(device, vertices, vertexBuffer);
	}

	void VertexSkinnedPositionTextureNormal::CreateVertexBuffer(not_null<ID3D11Device*> device, const Mesh& mesh, not_null<ID3D11Buffer**> vertexBuffer)
	{
		const Vector<glm::vec3>& sourceVertices = mesh.Vertices();
		const auto& sourceUVs = mesh.TextureCoordinates().At(0);
		assert(sourceUVs.Size() == sourceVertices.Size());
		const auto& sourceNormals = mesh.Normals();
		assert(sourceNormals.Size() == sourceVertices.Size());
		const auto& boneWeights = mesh.BoneWeights();
		assert(boneWeights.Size() == sourceVertices.Size());

		vector<VertexSkinnedPositionTextureNormal> vertices;
		vertices.reserve(sourceVertices.Size());
		for (size_t i = 0; i < sourceVertices.Size(); i++)
		{
			const glm::vec3& position = sourceVertices.At(i);
			const glm::vec3& uv = sourceUVs.At(i);
			const glm::vec3& normal = sourceNormals.At(i);
			const BoneVertexWeights& vertexWeights = boneWeights.At(i);

			float weights[BoneVertexWeights::MaxBoneWeightsPerVertex];
			uint32_t indices[BoneVertexWeights::MaxBoneWeightsPerVertex];
			ZeroMemory(weights, sizeof(float) * size(weights));
			ZeroMemory(indices, sizeof(uint32_t) * size(indices));
			for (size_t j = 0; j < vertexWeights.Weights().Size(); j++)
			{
				const BoneVertexWeights::VertexWeight& vertexWeight = vertexWeights.Weights().At(j);
				weights[j] = vertexWeight.Weight;
				indices[j] = vertexWeight.BoneIndex;
			}

			vertices.emplace_back(XMFLOAT4(position.x, position.y, position.z, 1.0f), XMFLOAT2(uv.x, uv.y), XMFLOAT3(&normal[0]), XMUINT4(indices), XMFLOAT4(weights));
		}

		VertexDeclaration::CreateVertexBuffer(device, vertices, vertexBuffer);
	}
}