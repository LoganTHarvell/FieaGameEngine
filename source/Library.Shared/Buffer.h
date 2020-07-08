#pragma once

#pragma region Includes
// First Party
#include "RTTI.h"
#include "Resource.h"
#include "RenderingTypes.h"
#include "Vector.h"
#pragma endregion Includes

namespace Library
{
    /**
     * @brief Hardware buffer descriptor.
    */
    struct BufferDesc
    {
        /**
         * @brief Buffer size (in bytes). This must not be larger than RenderingLimits::maxBufferSize. By default 0.
         * @remarks If stride is greater than zero, then size must be a multiple of stride.
        */
        std::uint64_t Size = 0;

        /**
         * @brief Optional stride for structured buffers. By default 0.
         * 
         * @remarks This is only used for Direct3D structured buffer,
         *          i.e. @c StructuredBuffer, @c RWStructuredBuffer,
         *          @c AppendStructuredBuffer, and @c ConsumeStructuredBuffer in HLSL.
         * @remarks If this is non-zero, the @c format attribute is ignored for sampled and storage buffers,
         *          i.e. buffers with the binding flags BindFlags::Sampled or BindFlags::Storage.
         *          
         * @note: If the buffer has the binding flag BindFlags::IndirectBuffer, this @b must be 0.
         * @note Only supported with: Direct3D 11, Direct3D 12.
         * 
         * @see MiscResourceFlags::Append
         * @see MiscResourceFlags::Counter
        */
    	std::uint32_t Stride = 0;

    	/**
         * @summary Optional hardware buffer format. By default Format::Unknown.
         * @remarks This is used for index buffers, typed buffers (e.g. @c Buffer<uint4> and @c RWBuffer<float4> in HLSL),
         *          and byte address buffers (i.e. @c ByteAddressBuffer and @c RWByteAddressBuffer in HLSL).
         *          
         * @remarks This field is ignored if the binding flags do not contain at least one of the following bits:
         *          BindFlags::IndexBuffer, BindFlags::Sampled, or BindFlags::Storage
         * @remarks If the BindFlags::IndexBuffer bit is set, this must be either Format::R16UInt, Format::R32UInt, or Format::Unknown.
         * @remarks If Format::Unknown is specified and the BindFlags::IndexBuffer bit is set,
         *          only the secondary @c SetIndexBuffer function can be used in the CommandBuffer interface.
         * @remarks If Format::Unknown is specified and @c stride is zero,
         *          sampled and storage buffers will be interpreted as byte address buffers.
         * @see BindFlags::IndexBuffer
         * @see RenderingManager::SetIndexBuffer(Buffer&)
        */
    	ResourceDataFormat Format = ResourceDataFormat::Unknown;

        /// <summary> These flags describe to which resource slots the buffer can be bound. By default 0.</summary>
        /// <remarks> When the buffer will be bound to a vertex buffer slot for instance, the BindFlags::VertexBuffer flag is required.</remarks>
        /// <see> BindFlags</see>        
        BindFlags BindFlags;

        /// <summary> CPU read/write access flags. By default all false.</summary>
        /// <remarks> If false, the buffer cannot be mapped from GPU memory space into CPU memory space and vice versa.</remarks>
        /// <see> CpuAccessFlags</see>
        CpuAccessFlags CpuAccessFlags;

        /// <summary> Miscellaneous buffer flags. By default all false.</summary>
        /// <remarks> This can be used as a hint for the renderer how frequently the buffer will be updated.</remarks>
        /// <see> MiscResourceFlags</see>
        MiscResourceFlags MiscFlags;

    	// TODO: Implement VertexAttributes
        /// <summary> Specifies the list of vertex attributes.</summary>
        /// <remarks> This is only used for vertex buffers and ignored if @c bindFlags does not contain the BindFlags::VertexBuffer bit.</remarks>
        //Vector<VertexAttributes> VertexAttributesList;

        /**
         * @brief Gets whether the description denotes a structured buffer.
         * @return True, if structured. Otherwise, false.
        */
        bool IsStructuredBuffer() const;

        /**
         * @brief Gets whether the description denotes a structured buffer.
         * @return True, if structured. Otherwise, false.
        */
    	bool IsTypedBuffer() const;

        /**
         * @brief Gets whether the description denotes a structured buffer.
         * @return True, if structured. Otherwise, false.
        */
    	bool IsByteAddressBuffer() const;
    };

    /**
     * @brief Hardware buffer abstraction layer. 
    */
    class Buffer : public RTTI
	{
		RTTI_DECLARATIONS_ABSTRACT(Buffer, RTTI)

#pragma region Type Definitions
	public:
		/**
		 * @brief Internal alias for BufferType.
		*/
		using Type = BufferType;
#pragma endregion Type Definitions

#pragma region Special Members
	protected:
		/**
		 * @brief Default constructor.
		 *        For use by subclasses only.
		 * @param type Buffer type.
		*/
		explicit Buffer(const Type type=Type::Undefined);

	public:
		/* Default Special Members */
		virtual ~Buffer() = default;
		Buffer(const Buffer & rhs) = default;
		Buffer& operator=(const Buffer & rhs) = default;
		Buffer(Buffer && rhs) = default;
		Buffer& operator=(Buffer && rhs) = default;
#pragma endregion Special Members

#pragma region Accessors
	public:
		/**
		 * @brief Gets the type of data represented by the Buffer.
		 * @return Current BufferType value.
		*/
		Type GetType() const;
#pragma endregion Accessors

#pragma region Data Members
	private:
		/**
		 * @brief Type of data represented by the Buffer.
		*/
		Type mType;
#pragma endregion Data Members
	};
}

// Inline File
#include "Buffer.inl"