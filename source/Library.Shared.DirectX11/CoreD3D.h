#pragma once

#pragma region Includes
// Third Party
#include <cstdint>
#include <d3d11.h>
#include <d3d11_2.h>
#include <DirectXMath.h>
#include <dxgidebug.h>
#include <gsl/gsl>
#include "winrt/base.h"

// First Party
#include "GameException.h"
#include "HashMap.h"
#include "RenderingTypes.h"
#pragma endregion Includes

// TODO: Rework under Library::Direct3D namespace
namespace Library
{
#pragma region Shaders
	/**
	 * @brief Shader stages enumeration.
	*/
	enum class ShaderStages
	{
		IA,
		VS,
		HS,
		DS,
		GS,
		SO,
		RS,
		PS,
		OM,

		CS
	};

	/**
	 * @brief Programmable shader stages listing.
	*/
	constexpr std::array<ShaderStages, 6> ProgrammableGraphicsShaderStages
	{
		ShaderStages::VS,
		ShaderStages::HS,
		ShaderStages::DS,
		ShaderStages::GS,
		ShaderStages::PS,
		ShaderStages::CS,
	};

	/**
	 * @brief Validates if a shader stage is programmable.
	 * @param shaderStage Shader stage to be validated.
	 * @return True, if shader stage is programmable. Otherwise, false.
	*/
	inline bool ShaderStageIsProgrammable(const ShaderStages shaderStage)
	{
		static const HashMap<ShaderStages, bool> isProgrammableMap
		{
			{ ShaderStages::IA, false },
			{ ShaderStages::VS, true },
			{ ShaderStages::HS, true },
			{ ShaderStages::DS, true },
			{ ShaderStages::GS, true },
			{ ShaderStages::SO, false },
			{ ShaderStages::RS, false },
			{ ShaderStages::PS, true },
			{ ShaderStages::OM, false },
			{ ShaderStages::CS, true },
		};

		return isProgrammableMap.At(shaderStage);
	}
#pragma endregion Shaders

	namespace Direct3D
	{
#pragma region Abstracted to Native Type Mappings
		/**
		 * @brief Mapping from abstracted PrimitiveTopology type to native Direct3D type.
		*/
		const HashMap<PrimitiveTopology, D3D_PRIMITIVE_TOPOLOGY> PrimitiveTopologyMap =
		{
			{ PrimitiveTopology::Undefined,					D3D_PRIMITIVE_TOPOLOGY_UNDEFINED					},
			{ PrimitiveTopology::PointList,	    			D3D_PRIMITIVE_TOPOLOGY_POINTLIST					},
			{ PrimitiveTopology::LineList,    				D3D_PRIMITIVE_TOPOLOGY_LINELIST						},
			{ PrimitiveTopology::LineStrip,	    			D3D_PRIMITIVE_TOPOLOGY_LINESTRIP					},
			{ PrimitiveTopology::TriangleList,    			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST					},
			{ PrimitiveTopology::TriangleStrip, 			D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP				},
			{ PrimitiveTopology::LineListAdjacency, 		D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ					},
			{ PrimitiveTopology::LineStripAdjacency,		D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ				},
			{ PrimitiveTopology::TriangleListAdjacency,		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ				},
			{ PrimitiveTopology::TriangleStripAdjacency,	D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ			},
			{ PrimitiveTopology::Patches1,					D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches2,					D3D_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches3,					D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches4,					D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches5,					D3D_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches6,					D3D_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches7,					D3D_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches8,					D3D_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches9,					D3D_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches10,					D3D_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches11,					D3D_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches12,					D3D_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches13,					D3D_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches14,					D3D_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches15,					D3D_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches16,					D3D_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches17,					D3D_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches18,					D3D_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches19,					D3D_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches20,					D3D_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches21,					D3D_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches22,					D3D_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches23,					D3D_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches24,					D3D_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches25,					D3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches26,					D3D_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches27,					D3D_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches28,					D3D_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches29,					D3D_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches30,					D3D_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches31,					D3D_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST	},
			{ PrimitiveTopology::Patches32,					D3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST	}
		};

		/**
		 * @brief Mapping from abstracted PrimitiveTopology type to native Direct3D type.
		*/
		const HashMap<ResourceDataFormat, DXGI_FORMAT> ResourceDataFormatMap =
		{
			{ ResourceDataFormat::Unknown, DXGI_FORMAT_UNKNOWN },

			/* --- Alpha channel color formats --- */
			{ ResourceDataFormat::A8UNorm, DXGI_FORMAT_A8_UNORM },

			/* --- Red channel color formats --- */
			{ ResourceDataFormat::R8UNorm, DXGI_FORMAT_R8_UNORM },
			{ ResourceDataFormat::R8SNorm, DXGI_FORMAT_R8_SNORM },
			{ ResourceDataFormat::R8UInt, DXGI_FORMAT_R8_UINT },
			{ ResourceDataFormat::R8SInt, DXGI_FORMAT_R8_SINT },

			{ ResourceDataFormat::R16UNorm, DXGI_FORMAT_R16_UNORM },
			{ ResourceDataFormat::R16SNorm, DXGI_FORMAT_R16_SNORM },
			{ ResourceDataFormat::R16UInt, DXGI_FORMAT_R16_UINT },
			{ ResourceDataFormat::R16SInt, DXGI_FORMAT_R16_SINT },
			{ ResourceDataFormat::R16Float, DXGI_FORMAT_R16_FLOAT },

			{ ResourceDataFormat::R32UInt, DXGI_FORMAT_R32_UINT },
			{ ResourceDataFormat::R32SInt, DXGI_FORMAT_R32_SINT },
			{ ResourceDataFormat::R32Float, DXGI_FORMAT_R32_FLOAT },

			{ ResourceDataFormat::R64Float, DXGI_FORMAT_UNKNOWN },

			/* --- RG color formats --- */
			{ ResourceDataFormat::RG8UNorm, DXGI_FORMAT_R8G8_UNORM },
			{ ResourceDataFormat::RG8SNorm, DXGI_FORMAT_R8G8_SNORM },
			{ ResourceDataFormat::RG8UInt, DXGI_FORMAT_R8G8_UINT },
			{ ResourceDataFormat::RG8SInt, DXGI_FORMAT_R8G8_SINT },

			{ ResourceDataFormat::RG16UNorm, DXGI_FORMAT_R16G16_UNORM },
			{ ResourceDataFormat::RG16SNorm, DXGI_FORMAT_R16G16_SNORM },
			{ ResourceDataFormat::RG16UInt, DXGI_FORMAT_R16G16_UINT },
			{ ResourceDataFormat::RG16SInt, DXGI_FORMAT_R16G16_SINT },
			{ ResourceDataFormat::RG16Float, DXGI_FORMAT_R16G16_FLOAT },

			{ ResourceDataFormat::RG32UInt, DXGI_FORMAT_R32G32_UINT },
			{ ResourceDataFormat::RG32SInt, DXGI_FORMAT_R32G32_SINT },
			{ ResourceDataFormat::RG32Float, DXGI_FORMAT_R32G32_FLOAT },

			{ ResourceDataFormat::RG64Float, DXGI_FORMAT_UNKNOWN },

			/* --- RGB color formats --- */
			{ ResourceDataFormat::RGB8UNorm, DXGI_FORMAT_UNKNOWN },
			{ ResourceDataFormat::RGB8UNorm_sRGB, DXGI_FORMAT_UNKNOWN },
			{ ResourceDataFormat::RGB8SNorm, DXGI_FORMAT_UNKNOWN },
			{ ResourceDataFormat::RGB8UInt, DXGI_FORMAT_UNKNOWN },
			{ ResourceDataFormat::RGB8SInt, DXGI_FORMAT_UNKNOWN },

			{ ResourceDataFormat::RGB16UNorm, DXGI_FORMAT_UNKNOWN },
			{ ResourceDataFormat::RGB16SNorm, DXGI_FORMAT_UNKNOWN },
			{ ResourceDataFormat::RGB16UInt, DXGI_FORMAT_UNKNOWN },
			{ ResourceDataFormat::RGB16SInt, DXGI_FORMAT_UNKNOWN },
			{ ResourceDataFormat::RGB16Float, DXGI_FORMAT_UNKNOWN },

			{ ResourceDataFormat::RGB32UInt, DXGI_FORMAT_R32G32B32_UINT },
			{ ResourceDataFormat::RGB32SInt, DXGI_FORMAT_R32G32B32_SINT },
			{ ResourceDataFormat::RGB32Float, DXGI_FORMAT_R32G32B32_FLOAT },

			{ ResourceDataFormat::RGB64Float, DXGI_FORMAT_UNKNOWN },

			/* --- RGBA color formats --- */
			{ ResourceDataFormat::RGBA8UNorm, DXGI_FORMAT_R8G8B8A8_UNORM },
			{ ResourceDataFormat::RGBA8UNorm_sRGB, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB },
			{ ResourceDataFormat::RGBA8SNorm, DXGI_FORMAT_R8G8B8A8_SNORM },
			{ ResourceDataFormat::RGBA8UInt, DXGI_FORMAT_R8G8B8A8_UINT },
			{ ResourceDataFormat::RGBA8SInt, DXGI_FORMAT_R8G8B8A8_SINT },

			{ ResourceDataFormat::RGBA16UNorm, DXGI_FORMAT_R16G16B16A16_UNORM },
			{ ResourceDataFormat::RGBA16SNorm, DXGI_FORMAT_R16G16B16A16_SNORM },
			{ ResourceDataFormat::RGBA16UInt, DXGI_FORMAT_R16G16B16A16_UINT },
			{ ResourceDataFormat::RGBA16SInt, DXGI_FORMAT_R16G16B16A16_SINT },
			{ ResourceDataFormat::RGBA16Float, DXGI_FORMAT_R16G16B16A16_FLOAT },

			{ ResourceDataFormat::RGBA32UInt, DXGI_FORMAT_R32G32B32A32_UINT },
			{ ResourceDataFormat::RGBA32SInt, DXGI_FORMAT_R32G32B32A32_SINT },
			{ ResourceDataFormat::RGBA32Float, DXGI_FORMAT_R32G32B32A32_FLOAT },

			{ ResourceDataFormat::RGBA64Float, DXGI_FORMAT_UNKNOWN },

			/* --- BGRA color formats --- */
			{ ResourceDataFormat::BGRA8UNorm, DXGI_FORMAT_B8G8R8A8_UNORM },
			{ ResourceDataFormat::BGRA8UNorm_sRGB, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB },
			{ ResourceDataFormat::BGRA8SNorm, DXGI_FORMAT_UNKNOWN },
			{ ResourceDataFormat::BGRA8UInt, DXGI_FORMAT_UNKNOWN },
			{ ResourceDataFormat::BGRA8SInt, DXGI_FORMAT_UNKNOWN },

			/* --- Packed formats --- */
			{ ResourceDataFormat::RGB10A2UNorm, DXGI_FORMAT_R10G10B10A2_UNORM },
			{ ResourceDataFormat::RGB10A2UInt, DXGI_FORMAT_R10G10B10A2_UINT },
			{ ResourceDataFormat::RG11B10Float, DXGI_FORMAT_R11G11B10_FLOAT },
			{ ResourceDataFormat::RGB9E5Float, DXGI_FORMAT_R9G9B9E5_SHAREDEXP },

			/* --- Depth-stencil formats --- */
			{ ResourceDataFormat::D16UNorm, DXGI_FORMAT_R16_TYPELESS },
			{ ResourceDataFormat::D32Float, DXGI_FORMAT_R32_TYPELESS },
			{ ResourceDataFormat::D24UNormS8UInt, DXGI_FORMAT_R24G8_TYPELESS },
			{ ResourceDataFormat::D32FloatS8X24UInt, DXGI_FORMAT_R32G8X24_TYPELESS },

			/* --- Block compression (BC) formats --- */
			{ ResourceDataFormat::BC1UNorm, DXGI_FORMAT_BC1_UNORM },
			{ ResourceDataFormat::BC1UNorm_sRGB, DXGI_FORMAT_BC1_UNORM_SRGB },
			{ ResourceDataFormat::BC2UNorm, DXGI_FORMAT_BC2_UNORM },
			{ ResourceDataFormat::BC2UNorm_sRGB, DXGI_FORMAT_BC2_UNORM_SRGB },
			{ ResourceDataFormat::BC3UNorm, DXGI_FORMAT_BC3_UNORM },
			{ ResourceDataFormat::BC3UNorm_sRGB, DXGI_FORMAT_BC3_UNORM_SRGB },
			{ ResourceDataFormat::BC4UNorm, DXGI_FORMAT_BC4_UNORM },
			{ ResourceDataFormat::BC4SNorm, DXGI_FORMAT_BC4_SNORM },
			{ ResourceDataFormat::BC5UNorm, DXGI_FORMAT_BC5_UNORM },
			{ ResourceDataFormat::BC5SNorm, DXGI_FORMAT_BC5_SNORM },
		};
#pragma endregion Abstracted to Native Type Mappings
	}

	// TODO: Move these functions to RenderingManagerDirect3D11
	void CreateIndexBuffer(gsl::not_null<ID3D11Device*> device, const gsl::span<const std::uint16_t>& indices, gsl::not_null<ID3D11Buffer**> indexBuffer);
	void CreateIndexBuffer(gsl::not_null<ID3D11Device*> device, const gsl::span<const std::uint32_t>& indices, gsl::not_null<ID3D11Buffer**> indexBuffer);
	void CreateConstantBuffer(gsl::not_null<ID3D11Device*> device, std::size_t byteWidth, gsl::not_null<ID3D11Buffer**> constantBuffer);

#pragma region Utility
	/**
	 * @brief Converts device independent pixels to actual pixels.
	 * @param dips Device independent pixel (DIP) size.
	 * @param dpi Dots per inch (DPI). Hard-coded to expect a value of 96.
	 * @return Ratio of pixel to DIP size.
	*/
	inline float ConvertDipsToPixels(float dips, float dpi)
	{
		static const float dipsPerInch = 96.0f;
		return floorf(dips * dpi / dipsPerInch + 0.5f); // Round to nearest integer.
	}

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

#if defined(DEBUG) || defined(_DEBUG)
	/**
	 * @brief Debug Dump.
	*/
	inline void DumpD3DDebug()
	{
		winrt::com_ptr<IDXGIDebug1> debugInterface = nullptr;
		ThrowIfFailed(DXGIGetDebugInterface1(0, IID_PPV_ARGS(debugInterface.put())));
		ThrowIfFailed(debugInterface->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL));
	}
#endif
#pragma endregion Utility
}
