#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "RenderingManagerD3D11.h"

// First Party
#include "CoreD3D.h"
#include "BufferD3D11.h"
#include "Mesh.h"
#pragma endregion Includes

namespace Library
{
#pragma region Special Members
	RenderingManagerD3D11::RenderingManagerD3D11() :
		RenderingManager(API::Direct3D11)
	{
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(mFactory.put())), "IDXGIFactory");
		
		CreateDevice(nullptr);
	}
#pragma endregion Special Members

#pragma region Modifiers
	RenderContext* RenderingManagerD3D11::CreateRenderContext()
	{
		auto newRenderContext = std::make_unique<RenderContextD3D11>(mDevice, mContext);
		return mRenderContexts.emplace(std::forward<std::unique_ptr<RenderContextD3D11>>(std::move(newRenderContext))).first->get();
	}

	Buffer* RenderingManagerD3D11::CreateMeshIndexBuffer(const Mesh& mesh)
	{
		BufferDesc desc;
		desc.Size = gsl::narrow_cast<uint32_t>(sizeof(uint32_t) * mesh.Indices().Size());
		desc.BindFlags.BufferType = BufferType::Index;

		auto newBuffer = std::make_unique<BufferD3D11>(mDevice.get(), desc, &mesh.Indices()[0]);		
		return mBuffers.emplace(std::forward<std::unique_ptr<BufferD3D11>>(std::move(newBuffer))).first->get();
	}

	void RenderingManagerD3D11::SetPrimitiveTopology(const PrimitiveTopology& topology)
	{
		mContext->IASetPrimitiveTopology(Direct3D::PrimitiveTopologyMap[topology]);
	}

	void RenderingManagerD3D11::SetIndexBuffer(Buffer& buffer)
	{
		assert(buffer.Is(BufferD3D11::TypeIdClass()));
		BufferD3D11& bufferD3D11 = static_cast<BufferD3D11&>(buffer);
		mContext->IASetIndexBuffer(bufferD3D11.Native(), bufferD3D11.Format(), 0);
	}
#pragma endregion Modifiers

#pragma region Accessors
	ID3D11Device* RenderingManagerD3D11::Device() const
	{
		return mDevice.get();
	}

	ID3D11DeviceContext* RenderingManagerD3D11::Context() const
	{
		return mContext.get();
	}

	winrt::com_ptr<ID3D11Device> RenderingManagerD3D11::DeviceComPtr() const
	{
		return mDevice;
	}

	winrt::com_ptr<ID3D11DeviceContext> RenderingManagerD3D11::ContextComPtr() const
	{
		return mContext;
	}
#pragma endregion Accessors

#pragma region Helper Methods
	void RenderingManagerD3D11::CreateDevice(IDXGIAdapter* adapter)
	{
		// This flag adds support for surfaces with a different color channel ordering
		// than the API default. It is required for compatibility with Direct2D.
		uint32_t flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
		if (SdkLayersAvailable())
		{
			// If the project is in a debug build, enable debugging via SDK Layers with this flag.
			flags |= D3D11_CREATE_DEVICE_DEBUG;
		}
#endif

		const Vector<D3D_FEATURE_LEVEL> featureLevels =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};

		HRESULT hr = 0;

		/* Create device without debug layer */
		CreateDeviceWithFlags(adapter, featureLevels, flags, hr);

		ThrowIfFailed(hr, "ID3D11Device");
	}

	bool RenderingManagerD3D11::CreateDeviceWithFlags(IDXGIAdapter* adapter, const Vector<D3D_FEATURE_LEVEL>& featureLevels, const std::uint32_t flags, HRESULT& hr)
	{
		for (D3D_DRIVER_TYPE driver : { D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_SOFTWARE })
		{
			hr = D3D11CreateDevice(
				adapter,                                    // Video adapter
				driver,                                     // Driver type
				nullptr,                                    // Software rasterizer module (none)
				flags,                                      // Flags
				featureLevels.Data(),                       // Feature level
				static_cast<UINT>(featureLevels.Size()),    // Num feature levels
				D3D11_SDK_VERSION,                          // SDK version
				mDevice.put(),                              // Output device
				&mFeatureLevel,                             // Output feature level
				mContext.put()                              // Output device context
			);
        	
			if (SUCCEEDED(hr)) return true;
		}
    	
		return false;
	}
#pragma endregion Helper Methods
}
