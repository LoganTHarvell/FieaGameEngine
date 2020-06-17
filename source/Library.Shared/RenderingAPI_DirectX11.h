#pragma once

#ifdef RENDERINGAPI_DIRECTX11
#include <d3d11_4.h>
#include <gsl/gsl>

#include "RenderingManager.h"

namespace Library
{
	struct RenderingManager::Device
	{
		ID3D11Device3* DevicePtr{ nullptr };
		ID3D11DeviceContext3* ContextPtr{ nullptr };
	};
	
	struct RenderingManager::IndexBuffer
	{
		ID3D11Buffer* BufferPtr{ nullptr };
	};
	
	class RenderingAPI_DirectX11 final : public RenderingManager
	{
		RTTI_DECLARATIONS(RenderingAPI_DirectX11, RenderingManager)

	public:
		RenderingAPI_DirectX11() = default;
		~RenderingAPI_DirectX11() = default;
		RenderingAPI_DirectX11(const RenderingAPI_DirectX11& rhs) = default;
		RenderingAPI_DirectX11& operator=(const RenderingAPI_DirectX11& rhs) = default;
		RenderingAPI_DirectX11(RenderingAPI_DirectX11&& rhs) = default;
		RenderingAPI_DirectX11& operator=(RenderingAPI_DirectX11&& rhs) = default;
		
		explicit RenderingAPI_DirectX11(ID3D11Device3& device, ID3D11DeviceContext3& context);

		public:
		virtual Device& GetDevice() override;
		virtual const Device& GetDevice() const override;
		virtual void SetDevice(const Device device) override;
		virtual IndexBuffer CreateMeshIndexBuffer(const Mesh& mesh) const override;
		
	private:
		Device mDevice;
	};
}
#endif
