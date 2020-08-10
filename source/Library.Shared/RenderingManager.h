#pragma once

#pragma region Includes
// First Party
#include "Resource.h"
#include "RTTI.h"
#include "Sampler.h"
#pragma endregion Includes

namespace Library
{
	// Forward Declarations
	template<typename T> class Vector;
	template<typename TKey, typename TData> class HashMap;
	class RenderContext;
	struct BufferDesc;
	enum class PrimitiveTopology;
	class Resource;
	class Buffer;
	class Shader;
	enum class ShaderStages;
	class VertexShader;
	class Mesh;
	enum class Format;

	/**
	 * @brief Abstraction of the main interface for the rendering system.
	 *		  It is the only class directly used by the engine.
	 *		  Relies on rendering API implementations for actual rendering.
	*/
	class RenderingManager : public RTTI
	{
		RTTI_DECLARATIONS_ABSTRACT(RenderingManager, RTTI)

#pragma region Type Definitions
	public:
		/**
		 * @brief Specifier for the rendering API implementation in use.
		*/
		enum class API
		{
			None,

			OpenGL,
			Direct3D11
		};
		
		using WindowCallback = std::shared_ptr<std::function<void* ()>>;
#pragma endregion Type Definitions

#pragma region Special Members
	protected:
		/* Default Special Members */
		RenderingManager() = default;
		virtual ~RenderingManager() = default;
		RenderingManager(const RenderingManager & rhs) = default;
		RenderingManager& operator=(const RenderingManager & rhs) = default;
		RenderingManager(RenderingManager && rhs) = default;
		RenderingManager& operator=(RenderingManager && rhs) = default;

		/**
		 * @brief Default Constructor
		 * @param api API specifier to be passed by a subclass
		*/
		explicit RenderingManager(const API api) : mAPI(api) {}
#pragma endregion Special Members

#pragma region Modifiers
	public:
		/**
		 * @brief Creates a render context
		 * @return Pointer to the render context created
		*/
		virtual RenderContext* CreateRenderContext() = 0;

		/**
		 * @brief Sets the primitive topology of the render device context
		 * @param topology Topology type to be set
		*/
		virtual void SetPrimitiveTopology(const PrimitiveTopology& topology) = 0;

		/**
		 * @brief Sets the vertex buffer of the render device context with the given buffer
		 * @param buffer Vertex buffer to be used.
		*/
		virtual void SetVertexBuffer(Buffer& buffer) = 0;

		/**
		 * @brief Sets the index buffer of the render device context with the given buffer
		 * @param buffer Index buffer to be used.
		*/
		virtual void SetIndexBuffer(Buffer& buffer) = 0;

		/**
		 * @brief Sets the index buffer of the render device context with the given buffer
		 * @param buffer Index buffer to be used.
		 * @param format Resource data format of the buffer
		 * @param offset Index offset of the buffer
		*/
		virtual void SetIndexBuffer(Buffer& buffer, const Format& format, const std::uint32_t offset) = 0;

		/**
		 * @brief Sets the shader to be used by the render device context
		 * @param shader Shader instance to be used
		*/
		virtual void SetShader(Shader* shader) const = 0;

		/**
		 * @brief Sets the shader resources for specific shader stages
		 * @param stage Stage for the resources should be set for
		 * @param resources Mapping of shader stages to their new resource data
		 * @param startSlot Starting slot for the resources to be overwritten
		*/
		virtual void SetShaderResources(const ShaderStages stage, const Resource::Type, const Vector<Resource*>& resources, const std::uint32_t startSlot=0) const = 0;

		/**
		 * @brief Updates sub-resource data for a Buffer
		 * @param buffer Buffer to be updated
		 * @param offset Offset into the buffer to update
		 * @param data Data used to update the buffer
		 * @param dataSize Size of the data in bytes
		*/
		virtual void UpdateBuffer(Buffer& buffer, const void* data, const std::uint16_t dataSize, const std::uint64_t offset=0) = 0;
		
		/**
		 * @brief Draws vertices using the set vertex buffer
		 * @param numVertices Number of vertices to be drawn
		 * @param firstVertex Index offset of the first vertex to be drawn
		*/
		virtual void Draw(const std::uint32_t numVertices, const std::uint32_t firstVertex) const = 0;

		/**
		 * @brief Draws indexed vertices using the set vertex and index buffer
		 * @param numIndices Number of indices to be drawn
		 * @param firstIndex Index offset of the first index to be drawn
		*/
		virtual void DrawIndexed(const std::uint32_t numIndices, const std::uint32_t firstIndex) const = 0;

		/**
		 * @brief Draws indexed vertices using the set vertex and index buffer
		 * @param numIndices Number of indices to be drawn
		 * @param firstIndex Index offset of the first index to be drawn
		 * @param vertexOffset Value added to each index before reading a vertex from the vertex buffer
		*/
		virtual void DrawIndexed(const std::uint32_t numIndices, const std::uint32_t firstIndex, const std::int32_t vertexOffset) const = 0;
		
		/**
		 * @brief Creates a buffer from a descriptor and initial data
		 * @param desc Descriptor for the buffer to be created
		 * @param initialData Data used to initialize the buffer
		 * @return Pointer to the created buffer
		*/
		virtual Buffer* CreateBuffer(const BufferDesc& desc, const void* initialData) = 0;
		
		/**
		 * @brief Creates an index buffer and returns a pointer to it
		 * @param indices Indices used to create the index buffer
		 * @return Pointer to the Buffer that was created
		*/
		Buffer* CreateIndexBuffer(const gsl::span<const std::uint16_t>& indices);
		
		/**
		 * @brief Creates an index buffer and returns a pointer to it
		 * @param indices Indices used to create the index buffer
		 * @return Pointer to the Buffer that was created
		*/
		Buffer* CreateIndexBuffer(const gsl::span<const std::uint32_t>& indices);
		
		/**
		 * @brief Creates an index buffer for a given mesh and returns a pointer to it
		 * @param mesh Mesh used to create the index buffer
		 * @return Pointer to the Buffer that was created
		*/
		Buffer* CreateMeshIndexBuffer(const Mesh& mesh);

		/**
		 * @brief Creates a constant buffer and returns a pointer to it
		 * @param byteWidth Size of the constant buffer to be created in bytes
		 * @return Pointer to the Buffer that was created
		*/
		Buffer* CreateConstantBuffer(const std::size_t byteWidth);
#pragma endregion Modifiers

#pragma region Accessors
	public:
		/**
		 * @brief Gets the rendering API specifier.
		 * @return 
		*/
		API GetAPI() const;

		/**
		 * @brief Gets the window callback functor.
		 *		  Functor is called on every window event.
		 *		  
		 * @return Window callback functor.
		*/
		const WindowCallback& GetWindowCallback() const;

		/**
		 * @brief Gets one of a number of sampler states based on the type
		 * @param samplerType Sampler state type to get
		 * @return Pointer to a Sampler wrapping the native sampler state
		*/
		virtual Sampler* GetSamplerState(const Sampler::Type samplerType) const = 0;
#pragma endregion Accessors

#pragma region Data Members
	private:
		API mAPI{ API::None };		//!< @brief Specifier denoting the actual rendering API implementation in use.
		WindowCallback mGetWindow;	//!< @brief Window callback functor.
#pragma endregion Data Members
	};
}

#include "RenderingManager.inl"