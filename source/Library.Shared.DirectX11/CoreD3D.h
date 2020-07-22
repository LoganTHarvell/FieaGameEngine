#pragma once

#pragma region Includes
// Third Party
#include "d3dcommon.h"
#include "dxgi1_6.h"
#include "dxgidebug.h"
#include "winrt/base.h"

// First Party
#include "GameException.h"
#include "HashMap.h"
#include "Resource.h"
#pragma endregion Includes

// TODO: Rework under Library::Direct3D namespace
namespace Library
{
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
		const HashMap<Format, DXGI_FORMAT> FormatMap =
		{
			{ Format::Unknown, DXGI_FORMAT_UNKNOWN },

			/* --- Alpha channel color formats --- */
			{ Format::A8UNorm, DXGI_FORMAT_A8_UNORM },

			/* --- Red channel color formats --- */
			{ Format::R8UNorm, DXGI_FORMAT_R8_UNORM },
			{ Format::R8SNorm, DXGI_FORMAT_R8_SNORM },
			{ Format::R8UInt, DXGI_FORMAT_R8_UINT },
			{ Format::R8SInt, DXGI_FORMAT_R8_SINT },

			{ Format::R16UNorm, DXGI_FORMAT_R16_UNORM },
			{ Format::R16SNorm, DXGI_FORMAT_R16_SNORM },
			{ Format::R16UInt, DXGI_FORMAT_R16_UINT },
			{ Format::R16SInt, DXGI_FORMAT_R16_SINT },
			{ Format::R16Float, DXGI_FORMAT_R16_FLOAT },

			{ Format::R32UInt, DXGI_FORMAT_R32_UINT },
			{ Format::R32SInt, DXGI_FORMAT_R32_SINT },
			{ Format::R32Float, DXGI_FORMAT_R32_FLOAT },

			{ Format::R64Float, DXGI_FORMAT_UNKNOWN },

			/* --- RG color formats --- */
			{ Format::RG8UNorm, DXGI_FORMAT_R8G8_UNORM },
			{ Format::RG8SNorm, DXGI_FORMAT_R8G8_SNORM },
			{ Format::RG8UInt, DXGI_FORMAT_R8G8_UINT },
			{ Format::RG8SInt, DXGI_FORMAT_R8G8_SINT },

			{ Format::RG16UNorm, DXGI_FORMAT_R16G16_UNORM },
			{ Format::RG16SNorm, DXGI_FORMAT_R16G16_SNORM },
			{ Format::RG16UInt, DXGI_FORMAT_R16G16_UINT },
			{ Format::RG16SInt, DXGI_FORMAT_R16G16_SINT },
			{ Format::RG16Float, DXGI_FORMAT_R16G16_FLOAT },

			{ Format::RG32UInt, DXGI_FORMAT_R32G32_UINT },
			{ Format::RG32SInt, DXGI_FORMAT_R32G32_SINT },
			{ Format::RG32Float, DXGI_FORMAT_R32G32_FLOAT },

			{ Format::RG64Float, DXGI_FORMAT_UNKNOWN },

			/* --- RGB color formats --- */
			{ Format::RGB8UNorm, DXGI_FORMAT_UNKNOWN },
			{ Format::RGB8UNorm_sRGB, DXGI_FORMAT_UNKNOWN },
			{ Format::RGB8SNorm, DXGI_FORMAT_UNKNOWN },
			{ Format::RGB8UInt, DXGI_FORMAT_UNKNOWN },
			{ Format::RGB8SInt, DXGI_FORMAT_UNKNOWN },

			{ Format::RGB16UNorm, DXGI_FORMAT_UNKNOWN },
			{ Format::RGB16SNorm, DXGI_FORMAT_UNKNOWN },
			{ Format::RGB16UInt, DXGI_FORMAT_UNKNOWN },
			{ Format::RGB16SInt, DXGI_FORMAT_UNKNOWN },
			{ Format::RGB16Float, DXGI_FORMAT_UNKNOWN },

			{ Format::RGB32UInt, DXGI_FORMAT_R32G32B32_UINT },
			{ Format::RGB32SInt, DXGI_FORMAT_R32G32B32_SINT },
			{ Format::RGB32Float, DXGI_FORMAT_R32G32B32_FLOAT },

			{ Format::RGB64Float, DXGI_FORMAT_UNKNOWN },

			/* --- RGBA color formats --- */
			{ Format::RGBA8UNorm, DXGI_FORMAT_R8G8B8A8_UNORM },
			{ Format::RGBA8UNorm_sRGB, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB },
			{ Format::RGBA8SNorm, DXGI_FORMAT_R8G8B8A8_SNORM },
			{ Format::RGBA8UInt, DXGI_FORMAT_R8G8B8A8_UINT },
			{ Format::RGBA8SInt, DXGI_FORMAT_R8G8B8A8_SINT },

			{ Format::RGBA16UNorm, DXGI_FORMAT_R16G16B16A16_UNORM },
			{ Format::RGBA16SNorm, DXGI_FORMAT_R16G16B16A16_SNORM },
			{ Format::RGBA16UInt, DXGI_FORMAT_R16G16B16A16_UINT },
			{ Format::RGBA16SInt, DXGI_FORMAT_R16G16B16A16_SINT },
			{ Format::RGBA16Float, DXGI_FORMAT_R16G16B16A16_FLOAT },

			{ Format::RGBA32UInt, DXGI_FORMAT_R32G32B32A32_UINT },
			{ Format::RGBA32SInt, DXGI_FORMAT_R32G32B32A32_SINT },
			{ Format::RGBA32Float, DXGI_FORMAT_R32G32B32A32_FLOAT },

			{ Format::RGBA64Float, DXGI_FORMAT_UNKNOWN },

			/* --- BGRA color formats --- */
			{ Format::BGRA8UNorm, DXGI_FORMAT_B8G8R8A8_UNORM },
			{ Format::BGRA8UNorm_sRGB, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB },
			{ Format::BGRA8SNorm, DXGI_FORMAT_UNKNOWN },
			{ Format::BGRA8UInt, DXGI_FORMAT_UNKNOWN },
			{ Format::BGRA8SInt, DXGI_FORMAT_UNKNOWN },

			/* --- Packed formats --- */
			{ Format::RGB10A2UNorm, DXGI_FORMAT_R10G10B10A2_UNORM },
			{ Format::RGB10A2UInt, DXGI_FORMAT_R10G10B10A2_UINT },
			{ Format::RG11B10Float, DXGI_FORMAT_R11G11B10_FLOAT },
			{ Format::RGB9E5Float, DXGI_FORMAT_R9G9B9E5_SHAREDEXP },

			/* --- Depth-stencil formats --- */
			{ Format::D16UNorm, DXGI_FORMAT_R16_TYPELESS },
			{ Format::D32Float, DXGI_FORMAT_R32_TYPELESS },
			{ Format::D24UNormS8UInt, DXGI_FORMAT_R24G8_TYPELESS },
			{ Format::D32FloatS8X24UInt, DXGI_FORMAT_R32G8X24_TYPELESS },

			/* --- Block compression (BC) formats --- */
			{ Format::BC1UNorm, DXGI_FORMAT_BC1_UNORM },
			{ Format::BC1UNorm_sRGB, DXGI_FORMAT_BC1_UNORM_SRGB },
			{ Format::BC2UNorm, DXGI_FORMAT_BC2_UNORM },
			{ Format::BC2UNorm_sRGB, DXGI_FORMAT_BC2_UNORM_SRGB },
			{ Format::BC3UNorm, DXGI_FORMAT_BC3_UNORM },
			{ Format::BC3UNorm_sRGB, DXGI_FORMAT_BC3_UNORM_SRGB },
			{ Format::BC4UNorm, DXGI_FORMAT_BC4_UNORM },
			{ Format::BC4SNorm, DXGI_FORMAT_BC4_SNORM },
			{ Format::BC5UNorm, DXGI_FORMAT_BC5_UNORM },
			{ Format::BC5SNorm, DXGI_FORMAT_BC5_SNORM },
		};
#pragma endregion Abstracted to Native Type Mappings
	}

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
