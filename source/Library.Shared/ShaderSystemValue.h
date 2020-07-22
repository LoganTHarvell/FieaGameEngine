#pragma once

namespace Library
{
	/**
	 * @brief Enumeration of system values for vertex input or fragment output attributes.
	 *
	 * @remarks This is only used for shader code reflection.
	*/
	enum class ShaderSystemValue
	{
        Undefined,          //!< Undefined system value
        ClipDistance,       //!< Forward-compatible mechanism for vertex clipping
        Color,              //!< Fragment output color value
        CullDistance,       //!< Mechanism for controlling user culling
		Depth,              //!< Fragment depth value
        DepthGreater,       //!< Fragment depth value that is greater than or equal to the previous one
		DepthLess,          //!< Fragment depth value that is less than or equal to the previous one
        FrontFacing,        //!< Indicates whether a primitive is front or back facing
        InstanceID,         //!< Index of the input instance
        Position,           //!< Vertex or fragment position
        PrimitiveID,        //!< Index of the geometry primitive
        RenderTargetIndex,  //!< Index of the render target layer
        SampleMask,         //!< Sample coverage mask
        SampleID,           //!< Index of the input sample
        Stencil,            //!< Fragment stencil value
        VertexID,           //!< Index of the input vertex
        ViewportIndex,      //!< Index of the viewport array
	};
}
