	#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
#include <gsl\gsl>

#include "GameException.h"

namespace Library
{
	class Mesh;

	template <typename T>
	struct VertexDeclaration
	{
		static constexpr uint32_t VertexSize() { return gsl::narrow_cast<uint32_t>(sizeof(T)); }
		static constexpr uint32_t VertexBufferByteWidth(size_t vertexCount) { return gsl::narrow_cast<uint32_t>(sizeof(T) * vertexCount); }
		static void CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const gsl::span<const T>& vertices, gsl::not_null<ID3D11Buffer**> vertexBuffer);
	};

	class VertexPosition : public VertexDeclaration<VertexPosition>
	{
	private:
		inline static const D3D11_INPUT_ELEMENT_DESC _InputElements[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

	public:
		VertexPosition() = default;

		VertexPosition(const DirectX::XMFLOAT4& position) :
			Position(position) { }

		DirectX::XMFLOAT4 Position;

		inline static const gsl::span<const D3D11_INPUT_ELEMENT_DESC> InputElements { _InputElements };

		static void CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const Library::Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer);
		static void CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const gsl::span<const VertexPosition>& vertices, gsl::not_null<ID3D11Buffer**> vertexBuffer)
		{
			VertexDeclaration::CreateVertexBuffer(device, vertices, vertexBuffer);
		}
	};

	class VertexPositionColor : public VertexDeclaration<VertexPositionColor>
	{
	private:
		inline static const D3D11_INPUT_ELEMENT_DESC _InputElements[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

	public:
		VertexPositionColor() = default;

		VertexPositionColor(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& color) :
			Position(position), Color(color) { }

		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT4 Color;

		inline static const gsl::span<const D3D11_INPUT_ELEMENT_DESC> InputElements{ _InputElements };

		static void CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const Library::Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer);
		static void CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const gsl::span<const VertexPositionColor>& vertices, gsl::not_null<ID3D11Buffer**> vertexBuffer)
		{
			VertexDeclaration::CreateVertexBuffer(device, vertices, vertexBuffer);
		}
	};

	class VertexPositionTexture : public VertexDeclaration<VertexPositionTexture>
	{
	private:
		inline static const D3D11_INPUT_ELEMENT_DESC _InputElements[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
	public:
		VertexPositionTexture() = default;

		VertexPositionTexture(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT2& textureCoordinates) :
			Position(position), TextureCoordinates(textureCoordinates) { }

		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT2 TextureCoordinates;

		inline static const gsl::span<const D3D11_INPUT_ELEMENT_DESC> InputElements{ _InputElements };

		static void CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const Library::Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer);
		static void CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const gsl::span<const VertexPositionTexture>& vertices, gsl::not_null<ID3D11Buffer**> vertexBuffer)
		{
			VertexDeclaration::CreateVertexBuffer(device, vertices, vertexBuffer);
		}
	};
		
	class VertexPositionSize : public VertexDeclaration<VertexPositionSize>
	{
	private:
		inline static const D3D11_INPUT_ELEMENT_DESC _InputElements[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

	public:
		VertexPositionSize() = default;

		VertexPositionSize(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT2& size) :
			Position(position), Size(size) { }

		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT2 Size;

		inline static const gsl::span<const D3D11_INPUT_ELEMENT_DESC> InputElements{ _InputElements };

		static void CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const gsl::span<const VertexPositionSize>& vertices, gsl::not_null<ID3D11Buffer**> vertexBuffer)
		{
			VertexDeclaration::CreateVertexBuffer(device, vertices, vertexBuffer);
		}
	};

	class VertexPositionNormal : public VertexDeclaration<VertexPositionNormal>
	{
	private:
		inline static const D3D11_INPUT_ELEMENT_DESC _InputElements[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

	public:
		VertexPositionNormal() = default;

		VertexPositionNormal(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT3& normal) :
			Position(position), Normal(normal) { }

		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT3 Normal;

		inline static const gsl::span<const D3D11_INPUT_ELEMENT_DESC> InputElements{ _InputElements };

		static void CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const Library::Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer);
		static void CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const gsl::span<const VertexPositionNormal>& vertices, gsl::not_null<ID3D11Buffer**> vertexBuffer)
		{
			VertexDeclaration::CreateVertexBuffer(device, vertices, vertexBuffer);
		}
	};

	class VertexPositionTextureNormal : public VertexDeclaration<VertexPositionTextureNormal>
	{
	private:
		inline static const D3D11_INPUT_ELEMENT_DESC _InputElements[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

	public:
		VertexPositionTextureNormal() = default;

		VertexPositionTextureNormal(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT2& textureCoordinates, const DirectX::XMFLOAT3& normal) :
			Position(position), TextureCoordinates(textureCoordinates), Normal(normal) { }

		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT2 TextureCoordinates;
		DirectX::XMFLOAT3 Normal;
		
		inline static const gsl::span<const D3D11_INPUT_ELEMENT_DESC> InputElements{ _InputElements };

		static void CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const Library::Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer);
		static void CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const gsl::span<const VertexPositionTextureNormal>& vertices, gsl::not_null<ID3D11Buffer**> vertexBuffer)
		{
			VertexDeclaration::CreateVertexBuffer(device, vertices, vertexBuffer);
		}
	};

	class VertexPositionTextureNormalTangent : public VertexDeclaration<VertexPositionTextureNormalTangent>
	{
	private:
		inline static const D3D11_INPUT_ELEMENT_DESC _InputElements[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

	public:
		VertexPositionTextureNormalTangent() = default;

		VertexPositionTextureNormalTangent(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT2& textureCoordinates, const DirectX::XMFLOAT3& normal, const DirectX::XMFLOAT3& tangent) :
			Position(position), TextureCoordinates(textureCoordinates), Normal(normal), Tangent(tangent) { }

		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT2 TextureCoordinates;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT3 Tangent;

		inline static const gsl::span<const D3D11_INPUT_ELEMENT_DESC> InputElements{ _InputElements };

		static void CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const Library::Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer);		
		static void CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const gsl::span<const VertexPositionTextureNormalTangent>& vertices, gsl::not_null<ID3D11Buffer**> vertexBuffer)
		{
			VertexDeclaration::CreateVertexBuffer(device, vertices, vertexBuffer);
		}
	};

	class VertexSkinnedPositionTextureNormal : public VertexDeclaration<VertexSkinnedPositionTextureNormal>
	{
	private:
		inline static const D3D11_INPUT_ELEMENT_DESC _InputElements[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

	public:
		VertexSkinnedPositionTextureNormal() = default;

		VertexSkinnedPositionTextureNormal(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT2& textureCoordinates, const DirectX::XMFLOAT3& normal, const DirectX::XMUINT4& boneIndices, const DirectX::XMFLOAT4& boneWeights) :
			Position(position), TextureCoordinates(textureCoordinates), Normal(normal), BoneIndices(boneIndices), BoneWeights(boneWeights) { }

		DirectX::XMFLOAT4 Position;
		DirectX::XMFLOAT2 TextureCoordinates;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMUINT4 BoneIndices;
		DirectX::XMFLOAT4 BoneWeights;
	
		inline static const gsl::span<const D3D11_INPUT_ELEMENT_DESC> InputElements{ _InputElements };

		static void CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const Library::Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer);
		static void CreateVertexBuffer(gsl::not_null<ID3D11Device*> device, const gsl::span<const VertexSkinnedPositionTextureNormal>& vertices, gsl::not_null<ID3D11Buffer**> vertexBuffer)
		{
			VertexDeclaration::CreateVertexBuffer(device, vertices, vertexBuffer);
		}
	};
}

#include "VertexDeclarations.inl"