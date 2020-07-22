#pragma once

namespace Library
{
	template <typename T>
	Buffer* VertexDeclaration<T>::CreateVertexBuffer(RenderingManager* renderingManager, const gsl::span<const T>& vertices)
	{
		if (renderingManager == nullptr) return nullptr;

		BufferDesc vertexBufferDesc;
		vertexBufferDesc.Size = VertexBufferByteWidth(vertices.size());
		vertexBufferDesc.Stride = VertexSize();
		vertexBufferDesc.VertexAttributes.Insert(vertexBufferDesc.VertexAttributes.cbegin(), T::InputElements.cbegin(), T::InputElements.cend());
		vertexBufferDesc.BindFlagsValue.BufferTypeValue = BufferType::Vertex;

		return renderingManager->CreateBuffer(vertexBufferDesc, &vertices[0]);
	}
}