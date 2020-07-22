// Pre-compiled Header
#include "pch.h"

// Header
#include "RenderingManager.h"

// First Party
#include "Buffer.h"
#include "Mesh.h"

namespace Library
{
	Buffer* RenderingManager::CreateIndexBuffer(const gsl::span<const std::uint16_t>& indices)
	{
		BufferDesc indexBufferDesc;
		indexBufferDesc.Size = gsl::narrow<std::uint32_t>(indices.size_bytes());
		indexBufferDesc.BindFlags.BufferType = BufferType::Index;

		return CreateBuffer(indexBufferDesc, &indices[0]);
	}

	Buffer* RenderingManager::CreateIndexBuffer(const gsl::span<const std::uint32_t>& indices)
	{
		BufferDesc indexBufferDesc;
		indexBufferDesc.Size = gsl::narrow<std::uint32_t>(indices.size_bytes());
		indexBufferDesc.BindFlags.BufferType = BufferType::Index;

		return CreateBuffer(indexBufferDesc, &indices[0]);
	}

	Buffer* RenderingManager::CreateMeshIndexBuffer(const Mesh& mesh)
	{
		BufferDesc desc;
		desc.Size = gsl::narrow_cast<uint32_t>(sizeof(uint32_t) * mesh.Indices().Size());
		desc.BindFlags.BufferType = BufferType::Index;

		return CreateBuffer(desc, &mesh.Indices()[0]);
	}

	Buffer* RenderingManager::CreateConstantBuffer(const std::size_t byteWidth)
	{
		BufferDesc constantBufferDesc;
		constantBufferDesc.Size = gsl::narrow_cast<std::uint32_t>(byteWidth);
		constantBufferDesc.BindFlags.BufferType = BufferType::Constant;
		
		return CreateBuffer(constantBufferDesc, nullptr);
	}
}
