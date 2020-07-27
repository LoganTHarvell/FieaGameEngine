#pragma once

#pragma region Includes
// Standard
#include <set>

// Third Party
#include <d3d11_4.h>
#include <winrt/base.h>

// First Party
#include "RenderingManager.h"
#include "RenderContextD3D11.h"
#include "BufferD3D11.h"
#pragma endregion Includes

namespace Library
{	
	/**
	 * @brief Implementation of the main interface for the rendering system for Direct3D 11.
	*/
	class RenderingManagerD3D11 final : public RenderingManager
	{
		RTTI_DECLARATIONS(RenderingManagerD3D11, RenderingManager)
		
#pragma region Special Members
	public:
		/**
		 * @brief Default constructor.
		*/
		RenderingManagerD3D11();

		/* Default Special Members */
		~RenderingManagerD3D11() = default;
		RenderingManagerD3D11(const RenderingManagerD3D11 & rhs) = default;
		RenderingManagerD3D11& operator=(const RenderingManagerD3D11 & rhs) = default;
		RenderingManagerD3D11(RenderingManagerD3D11 && rhs) = default;
		RenderingManagerD3D11& operator=(RenderingManagerD3D11 && rhs) = default;
#pragma endregion Special Members

#pragma region Modifiers
	public:
		/**
		 * @brief Creates a render context.
		 * @return Pointer to the render context created.
		*/
		virtual RenderContext* CreateRenderContext() override;

		/**
		 * @brief Sets the primitive topology of the render device context.
		 * @param topology Topology type to be set.
		*/
		virtual void SetPrimitiveTopology(const PrimitiveTopology& topology) override;

		/**
		 * @brief Sets the vertex buffer of the render device context with the given buffer
		 * @param buffer Vertex buffer to be used.
		*/
		virtual void SetVertexBuffer(Buffer& buffer) override;

		/**
		 * @brief Sets the index buffer of the render device context with the given buffer.
		 * @param buffer Index buffer to be used.
		*/
		virtual void SetIndexBuffer(Buffer& buffer) override;

		/**
		 * @brief Sets the index buffer of the render device context with the given buffer
		 * @param buffer Index buffer to be used.
		 * @param format Resource data format of the buffer
		 * @param offset Index offset of the buffer
		*/
		virtual void SetIndexBuffer(Buffer& buffer, const Format& format, const std::uint32_t offset) override;

		/**
		 * @brief Sets the shader to be used by the render device context
		 * @param shader Shader instance to be used
		*/
		virtual void SetShader(Shader* shader) const override;

		/**
		 * @brief Sets the shader to be used by the render device context
		 * @param shader Shader instance to be used
		 * @param classInstances Span of class instances uses to set the shader
		*/
		void SetShader(Shader* shader, const gsl::span<ID3D11ClassInstance*>& classInstances) const;

		/**
		 * @brief Sets the shader resources for specific shader stages
		 * @param stage Stage for the resources should be set for
		 * @param resources Mapping of shader stages to their new resource data
		 * @param startSlot Starting slot for the resources to be overwritten
		*/
		virtual void SetShaderResources(const ShaderStages stage, const Resource::Type, const Vector<Resource*>& resources, const std::uint32_t startSlot=0) const override;

		/**
		 * @brief Updates sub-resource data for a Buffer
		 * @param buffer Buffer to be updated
		 * @param offset Offset into the buffer to update
		 * @param data Data used to update the buffer
		 * @param dataSize Size of the data in bytes
		*/
		virtual void UpdateBuffer(Buffer& buffer, const void* data, const std::uint16_t dataSize, const std::uint64_t offset=0) override;
		
		/**
		 * @brief Draws vertices using the set vertex buffer
		 * @param numVertices Number of vertices to be drawn
		 * @param firstVertex Index offset of the first vertex to be drawn
		*/
		virtual void Draw(const std::uint32_t numVertices, const std::uint32_t firstVertex) const override;

		/**
		 * @brief Draws indexed vertices using the set vertex and index buffer
		 * @param numIndices Number of indices to be drawn
		 * @param firstIndex Index offset of the first index to be drawn
		*/
		virtual void DrawIndexed(const std::uint32_t numIndices, const std::uint32_t firstIndex) const override;

		/**
		 * @brief Draws indexed vertices using the set vertex and index buffer
		 * @param numIndices Number of indices to be drawn
		 * @param firstIndex Index offset of the first index to be drawn
		 * @param vertexOffset Value added to each index before reading a vertex from the vertex buffer
		*/
		virtual void DrawIndexed(const std::uint32_t numIndices, const std::uint32_t firstIndex, const std::int32_t vertexOffset) const override;
		
		/**
		 * @brief Creates a buffer from a descriptor and initial data
		 * @param desc Descriptor for the buffer to be created
		 * @param initialData Data used to initialize the buffer
		 * @return Pointer to the created buffer
		*/
		virtual Buffer* CreateBuffer(const BufferDesc& desc, const void* initialData) override;

		/**
		 * @brief Creates a class linkage for use when creating a @c Shader
		 * @return Newly created class linkage instance
		*/
		winrt::com_ptr<ID3D11ClassLinkage> CreateClassLinkage() const;
#pragma endregion Modifiers

#pragma region Accessors
	public:
		/**
		 * @brief Gets the underlying native Direct3D 11 render device.
		 * @return Pointer to the render device object.
		*/
		ID3D11Device* Device() const;

		/**
		 * @brief Gets the underlying native Direct3D 11 render device context.
		 * @return Pointer to the render device context object.
		*/
		ID3D11DeviceContext* Context() const;

		/**
		 * @brief Gets one of a number of sampler states based on the type
		 * @param samplerType Sampler state type to get
		 * @return Pointer to a Sampler wrapping the native sampler state
		*/
		virtual Sampler* GetSamplerState(const Sampler::Type samplerType) const override;		

		// TODO: Remove the com ptr getters since only the rendering manager should handle them
		
		/**
		 * @brief Gets the underlying native Direct3D 11 render device.
		 * @return COM pointer to the render device object.
		*/
		winrt::com_ptr<ID3D11Device> DeviceComPtr() const;

		/**
		 * @brief Gets the underlying native Direct3D 11 render device const.
		 * @return COM pointer to the render device context object.
		*/
		winrt::com_ptr<ID3D11DeviceContext> ContextComPtr() const;
#pragma endregion Accessors

#pragma region Helper Methods
	private:
		/**
		 * @brief Creates a device using the given DirectX graphics infrastructure adapter.
		 * @param adapter Interface for a specific display subsystem used to create the render device and device context.
		*/
		void CreateDevice(IDXGIAdapter* adapter);

		/**
		 * @brief Creates a device with the given flags.
		 * @param adapter Interface for a specific display subsystem used to create the render device and device context.
		 * @param featureLevels All possible Direct3D feature levels available for device creation.
		 * @param flags Flags to be passed during render device creation.
		 * @param hr[out] HRESULT to be set by the device creation call. 
		 * @return True, if render device is successfully created. Otherwise, false.
		*/
		bool CreateDeviceWithFlags(IDXGIAdapter* adapter, const Vector<D3D_FEATURE_LEVEL>& featureLevels, const std::uint32_t flags, HRESULT& hr);
#pragma endregion Helper Methods

#pragma region Data Members
	private:
		winrt::com_ptr<ID3D11Device> mDevice;							//!< @brief Render device
		winrt::com_ptr<ID3D11DeviceContext> mContext;					//!< @brief Render device context
		
		D3D_FEATURE_LEVEL mFeatureLevel{ D3D_FEATURE_LEVEL_1_0_CORE }; //!< @brief Feature level used of the current render device

		winrt::com_ptr<IDXGIFactory> mFactory;							//!< @brief DXGI factory for creating render contexts

		std::set<std::unique_ptr<RenderContextD3D11>> mRenderContexts;	//!< @brief Set of render contexts
		std::set<std::unique_ptr<BufferD3D11>> mBuffers;				//!< @brief Set of created resource buffers
#pragma endregion Data Members
	};


#if defined(DEBUG) || defined(_DEBUG)
/**
 * @brief Check for SDK Layer support.
 * @return True, if SDKLayers for debugging are available. Otherwise, false.
*/
inline bool SdkLayersAvailable()
{
	const HRESULT hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
		nullptr,                    // Any feature level will do.
		0,
		D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
		nullptr,                    // No need to keep the D3D device reference.
		nullptr,                    // No need to know the feature level.
		nullptr                     // No need to keep the D3D device context reference.
	);

	return SUCCEEDED(hr);
}
#endif
}
