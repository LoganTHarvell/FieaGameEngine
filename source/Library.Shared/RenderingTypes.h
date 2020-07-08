#pragma once

namespace Library
{
    /**
     * @brief Primitive topology enumeration.
     * 
     * @remarks Strip topologies (i.e. LineStrip, LineStripAdjacency, TriangleStrip, and TriangleStripAdjacency)
     *          use the maximum possible value for the respective index buffer format to restart the primitives.
     *          i.e. 0xFFFF for Format::R16UInt and 0xFFFFFFFF for Format::R32UInt.
    */
	enum class PrimitiveTopology
    {
    	Undefined = -1,
        PointList,      //!< Point list, where each vertex represents a single point primitive
        LineList,       //!< Line list, where each pair of two vertices represents a single line primitive
        LineStrip,      //!< Line strip, where each vertex generates a new line primitive and the previous vertex is used as line start

        /// <summary>Line loop, where the first and last vertices generate a line primitive.</summary>
        /// <remarks>Only supported with: OpenGL.</remarks>
        LineLoop,

        /// <summary>Adjacency line list, where each end point has an adjacent vertex accessible in a geometry shader.</summary>
        /// <remarks>Only supported with: OpenGL, Vulkan, Direct3D 11, Direct3D 12.</remarks>
        LineListAdjacency,

        /// <summary>Adjacency line strip, where each end point has an adjacent vertex accessible in a geometry shader.</summary>
        /// <remarks>Only supported with: OpenGL, Vulkan, Direct3D 11, Direct3D 12.</remarks>
        LineStripAdjacency,

        TriangleList,   //!< Triangle list, where each set of three vertices represent a single triangle primitive.
        TriangleStrip,  //!< Triangle strip, where each vertex generates a new triangle primitive with an alternative triangle winding.
        
        /// <summary>Triangle fan, where each vertex generates a new triangle primitive while all share the same first vertex.</summary>
        /// <remarks>Only supported with: OpenGL, Vulkan.</remarks>
        TriangleFan,
        
        /// <summary>Adjacency triangle list, where each edge has an adjacent vertex accessible in a geometry shader.</summary>
        /// <remarks>Only supported with: OpenGL, Vulkan, Direct3D 11, Direct3D 12.</remarks>   
        TriangleListAdjacency,
        
        /// <summary>Adjacency triangle strips, where each edge has an adjacent vertex accessible in a geometry shader.</summary>
        /// <remarks>Only supported with: OpenGL, Vulkan, Direct3D 11, Direct3D 12.</remarks>        
        TriangleStripAdjacency,

        Patches1,       //!< Patches with 1 control point that is accessible in a tessellation shader
        Patches2,       //!< Patches with 2 control points that are accessible in a tessellation shader
        Patches3,       //!< Patches with 3 control points that are accessible in a tessellation shader
        Patches4,       //!< Patches with 4 control points that are accessible in a tessellation shader
        Patches5,       //!< Patches with 5 control points that are accessible in a tessellation shader
        Patches6,       //!< Patches with 6 control points that are accessible in a tessellation shader
        Patches7,       //!< Patches with 7 control points that are accessible in a tessellation shader
        Patches8,       //!< Patches with 8 control points that are accessible in a tessellation shader
        Patches9,       //!< Patches with 9 control points that are accessible in a tessellation shader
        Patches10,      //!< Patches with 10 control points that are accessible in a tessellation shader
        Patches11,      //!< Patches with 11 control points that are accessible in a tessellation shader
        Patches12,      //!< Patches with 12 control points that are accessible in a tessellation shader
        Patches13,      //!< Patches with 13 control points that are accessible in a tessellation shader
        Patches14,      //!< Patches with 14 control points that are accessible in a tessellation shader
        Patches15,      //!< Patches with 15 control points that are accessible in a tessellation shader
        Patches16,      //!< Patches with 16 control points that are accessible in a tessellation shader
        Patches17,      //!< Patches with 17 control points that are accessible in a tessellation shader
        Patches18,      //!< Patches with 18 control points that are accessible in a tessellation shader
        Patches19,      //!< Patches with 19 control points that are accessible in a tessellation shader
        Patches20,      //!< Patches with 20 control points that are accessible in a tessellation shader
        Patches21,      //!< Patches with 21 control points that are accessible in a tessellation shader
        Patches22,      //!< Patches with 22 control points that are accessible in a tessellation shader
        Patches23,      //!< Patches with 23 control points that are accessible in a tessellation shader
        Patches24,      //!< Patches with 24 control points that are accessible in a tessellation shader
        Patches25,      //!< Patches with 25 control points that are accessible in a tessellation shader
        Patches26,      //!< Patches with 26 control points that are accessible in a tessellation shader
        Patches27,      //!< Patches with 27 control points that are accessible in a tessellation shader
        Patches28,      //!< Patches with 28 control points that are accessible in a tessellation shader
        Patches29,      //!< Patches with 29 control points that are accessible in a tessellation shader
        Patches30,      //!< Patches with 30 control points that are accessible in a tessellation shader
        Patches31,      //!< Patches with 31 control points that are accessible in a tessellation shader
        Patches32,      //!< Patches with 32 control points that are accessible in a tessellation shader
    };

    /**
     * @brief Resource data format enumeration.
    */
    enum class ResourceDataFormat
    {
        Unknown = -1,

        A8UNorm,            //!< Alpha color format: normalized 8-bit unsigned integer component

    	R8UNorm,            //!< Red color format: 8-bit normalized unsigned integer component
        R8SNorm,            //!< Red color format: 8-bit normalized signed integer component
        R8UInt,             //!< Red color format: 8-bit unsigned integer component
        R8SInt,             //!< Red color format: 8-bit signed integer component

        R16UNorm,           //!< Red color format: 16-bit normalized unsigned integer component
        R16SNorm,           //!< Red color format: 16-bit normalized signed integer component
        R16UInt,            //!< Red color format: 16-bit unsigned integer component
        R16SInt,            //!< Red color format: 16-bit signed integer component
        R16Float,           //!< Red color format: 16-bit floating point component

        R32UInt,            //!< Red color format: 32-bit unsigned integer component
        R32SInt,            //!< Red color format: 32-bit signed integer component
        R32Float,           //!< Red color format: 32-bit floating point component

        R64Float,           //!< Red color format: 64-bit floating point component Only supported with: Vulkan.

        RG8UNorm,           //!< RG color format: 8-bit normalized unsigned integer components
        RG8SNorm,           //!< RG color format: 8-bit normalized signed integer components
        RG8UInt,            //!< RG color format: 8-bit unsigned integer components
        RG8SInt,            //!< RG color format: 8-bit signed integer components

        RG16UNorm,          //!< RG color format: 16-bit normalized unsigned integer components
        RG16SNorm,          //!< RG color format: 16-bit normalized signed integer components
        RG16UInt,           //!< RG color format: 16-bit unsigned integer components
        RG16SInt,           //!< RG color format: 16-bit signed integer components
        RG16Float,          //!< RG color format: 16-bit floating point components

        RG32UInt,           //!< RG format: 32-bit unsigned integer components
        RG32SInt,           //!< RG format: 32-bit signed integer components
        RG32Float,          //!< RG format: 32-bit floating point components

        RG64Float,          //!< RG format: 64-bit floating point components. @note Only supported with: Vulkan.

        RGB8UNorm,          //!< RGB color format: red, green, blue 8-bit normalized unsigned integer components. @note Only supported with: OpenGL, Vulkan.
        RGB8UNorm_sRGB,     //!< RGB color format: red, green, blue 8-bit normalized unsigned integer components in non-linear sRGB color space. @note Only supported with: OpenGL, Vulkan.
        RGB8SNorm,          //!< RGB color format: red, green, blue 8-bit normalized signed integer components. @note Only supported with: OpenGL, Vulkan.
        RGB8UInt,           //!< RGB color format: red, green, blue 8-bit unsigned integer components. @note Only supported with: OpenGL, Vulkan.
        RGB8SInt,           //!< RGB color format: red, green, blue 8-bit signed integer components. @note Only supported with: OpenGL, Vulkan.

        RGB16UNorm,         //!< RGB color format: red, green, blue 16-bit normalized unsigned integer components. @note Only supported with: OpenGL, Vulkan.
        RGB16SNorm,         //!< RGB color format: red, green, blue 16-bit normalized signed integer components. @note Only supported with: OpenGL, Vulkan.
        RGB16UInt,          //!< RGB color format: red, green, blue 16-bit unsigned integer components. @note Only supported with: OpenGL, Vulkan.
        RGB16SInt,          //!< RGB color format: red, green, blue 16-bit signed integer components. @note Only supported with: OpenGL, Vulkan.
        RGB16Float,         //!< RGB color format: red, green, blue 16-bit floating point components. @note Only supported with: OpenGL, Vulkan.

        RGB32UInt,          //!< RGB color format: red, green, blue 32-bit unsigned integer components. @note As texture format only supported with: OpenGL, Vulkan, Direct3D 11, Direct3D 12.
        RGB32SInt,          //!< RGB color format: red, green, blue 32-bit signed integer components. @note As texture format only supported with: OpenGL, Vulkan, Direct3D 11, Direct3D 12.
        RGB32Float,         //!< RGB color format: red, green, blue 32-bit floating point components. @note As texture format only supported with: OpenGL, Vulkan, Direct3D 11, Direct3D 12.

        RGB64Float,         //!< RGB color format: red, green, blue 64-bit floating point components @note Only supported with: Vulkan.

        RGBA8UNorm,         //!< RGBA color format: red, green, blue, alpha 8-bit normalized unsigned integer components
        RGBA8UNorm_sRGB,    //!< RGBA color format: red, green, blue, alpha 8-bit normalized unsigned integer components in non-linear sRGB color space
        RGBA8SNorm,         //!< RGBA color format: red, green, blue, alpha 8-bit normalized signed integer components
        RGBA8UInt,          //!< RGBA color format: red, green, blue, alpha 8-bit unsigned integer components
        RGBA8SInt,          //!< RGBA color format: red, green, blue, alpha 8-bit signed integer components

        RGBA16UNorm,        //!< RGBA color format: red, green, blue, alpha 16-bit normalized unsigned integer components
        RGBA16SNorm,        //!< RGBA color format: red, green, blue, alpha 16-bit normalized signed integer components
        RGBA16UInt,         //!< RGBA color format: red, green, blue, alpha 16-bit unsigned integer components
        RGBA16SInt,         //!< RGBA color format: red, green, blue, alpha 16-bit signed integer components
        RGBA16Float,        //!< RGBA color format: red, green, blue, alpha 16-bit floating point components

        RGBA32UInt,         //!< RGBA color format: red, green, blue, alpha 32-bit unsigned integer components
        RGBA32SInt,         //!< RGBA color format: red, green, blue, alpha 32-bit signed integer components
        RGBA32Float,        //!< RGBA color format: red, green, blue, alpha 32-bit floating point components

        RGBA64Float,        //!< RGBA color format: red, green, blue, alpha 64-bit floating point components. @note Only supported with: Vulkan.

        BGRA8UNorm,         //!< BGRA color format: blue, green, red, alpha 8-bit normalized unsigned integer components
        BGRA8UNorm_sRGB,    //!< BGRA color format: blue, green, red, alpha 8-bit normalized unsigned integer components in non-linear sRGB color space.
        BGRA8SNorm,         //!< BGRA color format: blue, green, red, alpha 8-bit normalized signed integer components Only supported with: Vulkan.
        BGRA8UInt,          //!< BGRA color format: blue, green, red, alpha 8-bit unsigned integer components Only supported with: Vulkan.
        BGRA8SInt,          //!< BGRA color format: blue, green, red, alpha 8-bit signed integer components Only supported with: Vulkan.

        RGB10A2UNorm,       //!< Packed color format: red, green, blue 10-bit and alpha 2-bit normalized unsigned integer components
        RGB10A2UInt,        //!< Packed color format: red, green, blue 10-bit and alpha 2-bit unsigned integer components
        RG11B10Float,       //!< Packed color format: red, green 11-bit and blue 10-bit unsigned floating point, i.e. 6-bit mantissa for red and green, 5-bit mantissa for blue, and 5-bit exponent for all components
        RGB9E5Float,        //!< Packed color format: red, green, blue 9-bit unsigned floating-point with shared 5-bit exponent, i.e. 9-bit mantissa for each component and one 5-bit exponent for all components

        D16UNorm,           //!< Depth-stencil format: depth 16-bit normalized unsigned integer component
        D24UNormS8UInt,     //!< Depth-stencil format: depth 24-bit normalized unsigned integer component, and 8-bit unsigned integer stencil component
        D32Float,           //!< Depth-stencil format: depth 32-bit floating point component
        D32FloatS8X24UInt,  //!< Depth-stencil format: depth 32-bit floating point component, and 8-bit unsigned integer stencil components (remaining 24 bits are unused)
	  //S8UInt,             //!< Stencil only format: 8-bit unsigned integer stencil component. Only supported with: OpenGL, Vulkan, Metal.

        BC1UNorm,           //!< Block compression format: S3TC BC1 compressed RGBA with normalized unsigned integer components in 64-bit per 4x4 block
        BC1UNorm_sRGB,      //!< Block compression format: S3TC BC1 compressed RGBA with normalized unsigned integer components in 64-bit per 4x4 block in non-linear sRGB color space
        BC2UNorm,           //!< Block compression format: S3TC BC2 compressed RGBA with normalized unsigned integer components in 128-bit per 4x4 block
        BC2UNorm_sRGB,      //!< Block compression format: S3TC BC2 compressed RGBA with normalized unsigned integer components in 128-bit per 4x4 block in non-linear sRGB color space
        BC3UNorm,           //!< Block compression format: S3TC BC3 compressed RGBA with normalized unsigned integer components in 128-bit per 4x4 block
        BC3UNorm_sRGB,      //!< Block compression format: S3TC BC3 compressed RGBA with normalized unsigned integer components in 128-bit per 4x4 block in non-linear sRGB color space
        BC4UNorm,           //!< Block compression format: S3TC BC4 compressed red color with normalized unsigned integer component in 64-bit per 4x4 block
        BC4SNorm,           //!< Block compression format: S3TC BC4 compressed red color with normalized signed integer component 64-bit per 4x4 block
        BC5UNorm,           //!< Block compression format: S3TC BC5 compressed red and green colors with normalized unsigned integer components in 64-bit per 4x4 block
        BC5SNorm,           //!< Block compression format: S3TC BC5 compressed red and green colors with normalized signed integer components in 128-bit per 4x4 block
    };
}
