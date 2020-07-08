#pragma once

namespace Library
{
	// Forward Declarations
	struct BufferDesc;

	/**
	 * @brief Enumeration of possible Buffer types.
	*/
	enum class BufferType
	{
		Undefined = -1,

		Index,
		Vertex,
		Constant,
		StreamOutput,
		Indirect,
		Sampled,
		Storage
	};

	/**
	 * @brief Flags that describe how Buffers and Textures are bound.
	*/
	struct BindFlags
	{
		BufferType BufferType{ BufferType::Undefined };

		bool Sampled{ false };
		bool Storage{ false };
		
		bool CopySource{ false };
		bool CopyDestination{ false };
	};


	/**
	 * @brief CPU read/write access flags
	 * @remarks Determines whether the buffer can be mapped between GPU and CPU memory.
	*/
	struct CpuAccessFlags
	{
        bool Read{ false };
        bool Write{ false };

		bool ReadOrWrite() const
		{
            return Read || Write;
		}
	};

	/**
	 * @brief Miscellaneous flags for how to handle resources.
	*/
	struct MiscResourceFlags
	{
		/**
		@brief Hint to the renderer that the resource will be frequently updated from the CPU.
		@remarks This is useful for a constant buffer for instance, that is updated by the host program every frame.
		*/
		bool DynamicUsage{ false };

		/**
		@brief Multi-sampled Texture resource has fixed sample locations.
		@remarks This can only be used with multi-sampled Texture resources (i.e. TextureType::Texture2DMS, TextureType::Texture2DMSArray).
		*/
		bool FixedSamples{ false };

		/**
		@brief Generates MIP-maps at texture creation time with the initial image data (if specified).
		@remarks To generate MIP-maps, the texture must be created with the binding flags BindFlags::Sampled and BindFlags::ColorAttachment, which is the default.
		@remarks This can be used to generate all MIP-maps when a new texture is created without explicitly encoding the CommandBuffer::GenerateMips function.
		The number of MIP-maps being generated depends on the @c mipLevels attribute in TextureDescriptor.
		*/
		bool GenerateMips{ false };

		/**
		@brief Specifies to ignore resource data initialization.
		@remarks If this is specified, a texture or buffer resource will stay uninitialized during creation and the content is undefined.
		*/
		bool NoInitialDat{ false };

		/**
		@brief Enables a storage buffer to be used for @c AppendStructuredBuffer and @c ConsumeStructuredBuffer in HLSL only.
		@remarks This can only be used with buffers that also have the binding flag BindFlags::Storage and a @c stride greater than zero.
		@remarks This cannot be used together with the MiscFlags::Counter bit.
		@note Only supported with: Direct3D 11, Direct3D 12.
		@see Buffer::BufferDesc.Stride
		@see https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ne-d3d11-d3d11_buffer_uav_flag
		*/
		bool Append{ false };

		/**
		@brief Enables the hidden counter in a storage buffer to be used for @c RWStructuredBuffer in HLSL only.
		@remarks This can only be used with buffers that also have the binding flag BindFlags::Storage and a @c stride greater than zero.
		@remarks This cannot be used together with the MiscFlags::Append bit.
		@note Only supported with Direct3D 11, Direct3D 12.
		@see Buffer::BufferDesc.Stride
		@see https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ne-d3d11-d3d11_buffer_uav_flag
		*/
		bool Counter{ false };
	};
}
