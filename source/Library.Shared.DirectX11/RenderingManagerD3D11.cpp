#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "RenderingManagerD3D11.h"

// First Party
#include "CoreD3D.h"
#include "BufferD3D11.h"
#include "BufferWithResourceViewD3D11.h"
#include "ResourceD3D11.h"
#include "SamplerD3D11.h"
#include "SamplerStatesD3D11.h"
#include "ShaderD3D11.h"
#include "TextureD3D11.h"
#pragma endregion Includes

namespace Library
{
	struct ShaderStageCallInfo
	{
		std::function<void(ID3D11DeviceContext*, UINT, UINT, ID3D11Buffer* const*)> SetConstantBuffers;
		std::function<void(ID3D11DeviceContext*, UINT, UINT, ID3D11ShaderResourceView* const*)> SetShaderResources;
		std::function<void(ID3D11DeviceContext*, UINT, UINT, ID3D11SamplerState* const*)> SetSamplers;
	};
	
	static const HashMap<ShaderStages, ShaderStageCallInfo> ShaderStageCalls
	{
		{
			ShaderStages::VS,
			{
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->VSSetConstantBuffers(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->VSSetShaderResources(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->VSSetSamplers(PH2, PH3, PH4); },
			}
		},
		{
			ShaderStages::HS,
			{
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->HSSetConstantBuffers(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->HSSetShaderResources(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->HSSetSamplers(PH2, PH3, PH4); },
			}
		},
		{
			ShaderStages::DS,
			{
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->DSSetConstantBuffers(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->DSSetShaderResources(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->DSSetSamplers(PH2, PH3, PH4); },
			}
		},
		{
			ShaderStages::GS,
			{
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->GSSetConstantBuffers(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->GSSetShaderResources(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->GSSetSamplers(PH2, PH3, PH4); },
			}
		},
		{
			ShaderStages::PS,
			{
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->PSSetConstantBuffers(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->PSSetShaderResources(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->PSSetSamplers(PH2, PH3, PH4); },
			}
		},
		{
			ShaderStages::CS,
			{
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->CSSetConstantBuffers(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->CSSetShaderResources(PH2, PH3, PH4); },
				[](auto&& PH1, auto&& PH2, auto&& PH3, auto&& PH4) { PH1->CSSetSamplers(PH2, PH3, PH4); },
			}
		}
	};
	
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

	void RenderingManagerD3D11::SetPrimitiveTopology(const PrimitiveTopology& topology)
	{
		mContext->IASetPrimitiveTopology(Direct3D::PrimitiveTopologyMap[topology]);
	}

	void RenderingManagerD3D11::SetVertexBuffer(Buffer& buffer)
	{
		BufferD3D11& bufferD3D = *buffer.AssertAs<BufferD3D11>();

		ID3D11Buffer* const buffers[] = { bufferD3D.Native() };
		UINT stride = bufferD3D.Stride();
		UINT offset = 0;

		mContext->IASetVertexBuffers(0, 1, buffers, &stride, &offset);
	}

	void RenderingManagerD3D11::SetIndexBuffer(Buffer& buffer)
	{
		BufferD3D11& bufferD3D11 = *buffer.AssertAs<BufferD3D11>();
		mContext->IASetIndexBuffer(bufferD3D11.Native(), bufferD3D11.Format(), 0);
	}

	void RenderingManagerD3D11::SetIndexBuffer(Buffer& buffer, const Format& format, const std::uint32_t offset)
	{
		BufferD3D11& bufferD3D11 = *buffer.AssertAs<BufferD3D11>();
		mContext->IASetIndexBuffer(bufferD3D11.Native(), Direct3D::FormatMap[format], offset);
	}

	void RenderingManagerD3D11::SetShader(Shader* shader) const
	{
		SetShader(shader, gsl::make_span<ID3D11ClassInstance*>(nullptr, nullptr));
	}

	void RenderingManagerD3D11::SetShader(Shader* shader, const gsl::span<ID3D11ClassInstance*>& classInstances) const
	{
		switch (shader->ShaderStage())
		{
		case ShaderStages::VS:
		{
			auto* vs = shader->AssertAs<VertexShaderD3D11>();
			mContext->IASetInputLayout(vs->InputLayout().get());
			mContext->VSSetShader(vs->Native().get(), classInstances.data(), static_cast<UINT>(classInstances.size()));
			break;
		}
		case ShaderStages::HS:
			if (shader)
			{
				auto* hs = shader->AssertAs<HullShaderD3D11>();
				mContext->HSSetShader(hs->Native().get(), classInstances.data(), static_cast<UINT>(classInstances.size()));
			}
			else
			{
				mContext->HSSetShader(nullptr, nullptr, 0);
			}
			break;
			
		case ShaderStages::DS:
			if (shader)
			{
				auto* ds = shader->AssertAs<DomainShaderD3D11>();
				mContext->DSSetShader(ds->Native().get(), classInstances.data(), static_cast<UINT>(classInstances.size()));
			}
			else
			{
				mContext->DSSetShader(nullptr, nullptr, 0);
			}
			break;
			
		case ShaderStages::GS:
			if (shader)
			{
				auto* gs = shader->AssertAs<GeometryShaderD3D11>();
				mContext->GSSetShader(gs->Native().get(), classInstances.data(), static_cast<UINT>(classInstances.size()));
			}
			else
			{
				mContext->GSSetShader(nullptr, nullptr, 0);
			}
			break;
			
		case ShaderStages::PS:
			if (shader)
			{
				auto* ps = shader->AssertAs<PixelShaderD3D11>();
				mContext->PSSetShader(ps->Native().get(), classInstances.data(), static_cast<UINT>(classInstances.size()));
			}
			else
			{
				mContext->PSSetShader(nullptr, nullptr, 0);
			}
			break;
			
		case ShaderStages::CS:
			if (shader)
			{
				auto* cs = shader->AssertAs<ComputeShaderD3D11>();
				mContext->CSSetShader(cs->Native().get(), classInstances.data(), static_cast<UINT>(classInstances.size()));
			}
			else
			{
				mContext->CSSetShader(nullptr, nullptr, 0);
			}
			break;

		default:
			throw std::runtime_error("Invalid shader");
		}
	}

	void RenderingManagerD3D11::SetShaderResources(const ShaderStages stage, const Resource::Type type, const Vector<Resource*>& resources, const std::uint32_t startSlot) const
	{		
		switch (type)
		{
		case Resource::Type::Buffer:
		{
			Vector<ID3D11Buffer*> cBuffers;

			for (auto* buffer : resources)
			{
				cBuffers.EmplaceBack(buffer->AssertAs<BufferD3D11>()->Native());
			}

			ShaderStageCalls[stage].SetConstantBuffers(mContext.get(), startSlot, static_cast<UINT>(cBuffers.Size()), cBuffers.Data());
			break;
		}
		case Resource::Type::Texture:
		{
			Vector<ID3D11ShaderResourceView*> shaderResourceViews;

			for (auto* buffer : resources)
			{
				// TODO: Fix inheritance for textures
				if (auto* tex = buffer->As<TextureD3D11>())
				{
					shaderResourceViews.EmplaceBack(tex->ShaderResourceView().get());				
				}
				else if (auto* tex2D = buffer->As<Texture2DD3D11>())
				{
					shaderResourceViews.EmplaceBack(tex2D->ShaderResourceView().get());				
				}
				else if (auto* texCube = buffer->As<TextureCubeD3D11>())
				{
					shaderResourceViews.EmplaceBack(texCube->ShaderResourceView().get());				
				}
				else
				{
					throw std::runtime_error("Invalid texture buffer");
				}
			}

			ShaderStageCalls[stage].SetShaderResources(mContext.get(), startSlot, static_cast<UINT>(shaderResourceViews.Size()), shaderResourceViews.Data());

			break;
		}
		case Resource::Type::Sampler:
		{
			Vector<ID3D11SamplerState*> samplers;

			for (auto* buffer : resources)
			{
				samplers.EmplaceBack(buffer->AssertAs<SamplerD3D11>()->Native());
			}

			ShaderStageCalls[stage].SetSamplers(mContext.get(), startSlot, static_cast<UINT>(samplers.Size()), samplers.Data());
				
			break;
		}
			
		default:
			throw std::runtime_error("Resource type undefined");
		}
	}

	void RenderingManagerD3D11::UpdateBuffer(Buffer& buffer, const void* data, const std::uint16_t dataSize, const std::uint64_t offset)
	{
		buffer.AssertAs<BufferD3D11>()->UpdateSubresource(mContext.get(), data, static_cast<UINT>(dataSize), static_cast<UINT>(offset));
	}

	void RenderingManagerD3D11::Draw(const std::uint32_t numVertices, const std::uint32_t firstVertex) const
	{
		mContext->Draw(numVertices, firstVertex);
	}

	void RenderingManagerD3D11::DrawIndexed(const std::uint32_t numIndices, const std::uint32_t firstIndex) const
	{
		mContext->DrawIndexed(numIndices, firstIndex, 0);
	}

	void RenderingManagerD3D11::DrawIndexed(const std::uint32_t numIndices, const std::uint32_t firstIndex, const std::int32_t vertexOffset) const
	{
		mContext->DrawIndexed(numIndices, firstIndex, vertexOffset);
	}

	Buffer* RenderingManagerD3D11::CreateBuffer(const BufferDesc& desc, const void* initialData)
	{
		std::unique_ptr<BufferD3D11> newBuffer;
		
		if (D3D11::RequiresBufferWithRV(desc.BindFlagsValue))
		{
			newBuffer = std::make_unique<Direct3D11::BufferWithResourceViewD3D11>(mDevice.get(), desc, initialData);
		}
		else
		{
			newBuffer = std::make_unique<BufferD3D11>(mDevice.get(), desc, initialData);
		}
		
		return mBuffers.emplace(std::forward<std::unique_ptr<BufferD3D11>>(std::move(newBuffer))).first->get();
	}

	winrt::com_ptr<ID3D11ClassLinkage> RenderingManagerD3D11::CreateClassLinkage() const
	{
		winrt::com_ptr<ID3D11ClassLinkage> classLinkage;
		ThrowIfFailed(mDevice->CreateClassLinkage(classLinkage.put()), "ID3D11Device::CreateClassLinkage() failed.");

		return classLinkage;
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

	Sampler* RenderingManagerD3D11::GetSamplerState(const Sampler::Type samplerType) const
	{
		return SamplerStatesD3D11::TypeSamplerMap[samplerType].get();
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
