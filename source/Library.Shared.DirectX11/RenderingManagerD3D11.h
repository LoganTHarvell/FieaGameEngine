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
		 * @brief Creates an index buffer for a given mesh and returns it.
		 * @param mesh Mesh used to create the index buffer.
		 * @return Pointer to the Buffer that was created.
		*/
		virtual Buffer* CreateMeshIndexBuffer(const Mesh& mesh) override;

		/**
		 * @brief Sets the primitive topology of the render device context.
		 * @param topology Topology type to be set. 
		*/
		virtual void SetPrimitiveTopology(const PrimitiveTopology& topology) override;

		/**
		 * @brief Sets the index buffer of the render device context with the given buffer.
		 * @param buffer Index buffer to be used.
		*/
		virtual void SetIndexBuffer(Buffer& buffer) override;

		// TODO: Set Vertex Buffer method
		//virtual void SetVertexBuffer(Buffer& buffer) override;
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
}
