#pragma once

#pragma region Includes
// Standard
#include <cstdint>
#include <string>

// First Party
#include "Resource.h"
#include "ShaderSystemValue.h"
#pragma endregion Includes

namespace Library
{
	/**
	 * @brief Data structure for reflecting a shader vertex attribute data
	*/
	struct VertexAttribute
	{
#pragma region Special Members
	public:
		/* Default Special Members */
		VertexAttribute() = default;
		~VertexAttribute() = default;
		VertexAttribute(const VertexAttribute& rhs) = default;
		VertexAttribute& operator=(const VertexAttribute& rhs) = default;
		VertexAttribute(VertexAttribute&& rhs) = default;
		VertexAttribute& operator=(VertexAttribute&& rhs) = default;

		/**
         * @brief Constructor for minimal vertex attribute information and system value semantics
         * @param name String representing the vertex semantic name
         * @param format 
         * @param location 
         * @param instanceDivisor 
         * @param systemValue 
         * @return 
        */
		VertexAttribute(
			const std::string&      name,
			const Format            format,
			std::uint32_t           location = 0,
			std::uint32_t           instanceDivisor = 0,
			const ShaderSystemValue systemValue = ShaderSystemValue::Undefined
		);

		/**
         * @brief Constructor for basic vertex attribute information
         * @param semanticName String representing the vertex semantic name
         * @param semanticIndex 
         * @param format 
         * @param location 
         * @param instanceDivisor 
         * @return 
        */
		VertexAttribute(
			const std::string&  semanticName,
			std::uint32_t       semanticIndex,
			const Format        format,
			std::uint32_t       location = 0,
			std::uint32_t       instanceDivisor = 0
		);

		/**
         * @brief Constructor for common vertex attribute information
         * @param name String representing the vertex semantic name
         * @param format 
         * @param location 
         * @param offset 
         * @param stride 
         * @param slot 
         * @param instanceDivisor 
         * @return 
        */
		VertexAttribute(
			const std::string&  name,
			const Format        format,
			std::uint32_t       location,
			std::uint32_t       offset,
			std::uint32_t       stride,
			std::uint32_t       slot = 0,
			std::uint32_t       instanceDivisor = 0
		);

		/**
         * @brief Constructor for the most vertex attribute information, including semantic index
         * @param semanticName String representing the vertex semantic name
         * @param semanticIndex 
         * @param format 
         * @param location 
         * @param offset 
         * @param stride 
         * @param slot 
         * @param instanceDivisor 
         * @return 
        */
		VertexAttribute(
			const std::string&  semanticName,
			std::uint32_t       semanticIndex,
			const Format        format,
			std::uint32_t       location,
			std::uint32_t       offset,
			std::uint32_t       stride,
			std::uint32_t       slot = 0,
			std::uint32_t       instanceDivisor = 0
		);
#pragma endregion Special Members

#pragma region Convenience Methods
	public:
        /**
         * @brief Equals operator
         * @param rhs Value to be compared
         * @return True, if equal, otherwise false
        */
        bool operator==(const VertexAttribute& rhs) const noexcept;

		/**
         * @brief Not equal operator
         * @param rhs Value to be compared
         * @return True, if not equal, otherwise false
        */
		bool operator!=(const VertexAttribute& rhs) const noexcept;
		
		/**
		 * @brief Gets the size required for the vertex attribute
		 * @return Size required, in bytes, or zero if invalid
		*/
		std::uint32_t GetSize() const;
#pragma endregion Convenience Methods

	public:
        std::string Name;   //!< Vertex attribute name (for GLSL) or semantic name (for HLSL)

		/**
         * @brief Vertex attribute format, by default Format::RGBA32Float
         * @remarks Not all hardware formats are allowed for vertex attributes
		 *			In particular, depth-stencil formats and compressed formats are not allowed
		 *			To specify a vertex attribute of a matrix type, multiple attributes with ascending semantic indices must be used
        */
        Format Format{ Format::RGBA32Float };

        /**
         * @brief Vertex attribute location (only for OpenGL, Vulkan, Metal) or stream-output number (only for Direct3D)
         * 
         * @remarks This is only required for OpenGL, Vulkan, and Metal, for Direct3D, this is ignored and instead * @c semanticIndex is used
        */
        std::uint32_t   Location = 0;

        /**
         * @brief Semantic index for HLSL
         * @remarks This is only required for Direct3D when a semantic name is used multiple times
					This happens when a matrix type is distributed over multiple vector attributes
         * @remarks The following example uses semantic names * @c POS0, * @c MATRIX0, * @c MATRIX1, * @c MATRIX2, * @c MATRIX3:
         * @code
                struct MyVertexInput
                {
                    float4   vertexPosition   : POS;
                    float4x4 projectionMatrix : MATRIX;
                };
         * @endcode
        */
        std::uint32_t   SemanticIndex = 0;

        /**
         * @brief Specifies the system value type for this vertex attribute or SystemValue::Undefined if this attribute is not a system value By default SystemValue::Undefined
         * @remarks System value semantics are only used for shader code reflection Examples of system value semantics are:
         *          - Vertex ID: * @c SV_VertexID (HLSL), * @c gl_VertexID (GLSL), * @c gl_VertexIndex (SPIR-V), <code>[[vertex_id]]</code> (Metal)
         *          - Instance ID: * @c SV_InstanceID (HLSL), * @c gl_InstanceID (GLSL), * @c gl_InstanceIndex (SPIR-V), <code>[[instance_id]]</code> (Metal)
         * @see ShaderProgram::Reflect
        */
        ShaderSystemValue     SystemValue = ShaderSystemValue::Undefined;

        /**
         * @brief Vertex buffer binding slot By default 0
         * @remarks This is used when multiple vertex buffers are used simultaneously
                    This binding slot refers either to the input buffer indices (determined by * @c SetVertexBufferArray),
                    or stream-output buffer indices (determined by * @c BeginStreamOutput)
         * @note Only supported with: Direct3D 11, Direct3D 12, Vulkan, Metal
         * @note For OpenGL, the vertex binding slots are automatically generated in ascending order and beginning with zero
        */
        std::uint32_t   Slot = 0;

        /**
         * @brief Byte offset within each vertex and each buffer for input attributes, or component offset for output attributes By default 0
         * @remarks For vertex input attributes, this offset specifies the byte aligned offset within each vertex buffer
         * @remarks For stream-output attributes, this offset specifies the first component that is to be written and must be either 0, 1, 2, or 3
        */
        std::uint32_t   Offset = 0;

        //! Specifies the vertex data stride which describes the byte offset between consecutive vertices
        std::uint32_t   Stride = 0;

        /**
         * @brief Instance data divisor (or instance data step rate)
         * @remarks If this is 0, this attribute is considered to be stored per vertex
		 *			If this is greater than 0, this attribute is considered to be stored per every instanceDivisor's instance
         * @note For Vulkan, this must only be 0 or 1
        */
        std::uint32_t   InstanceDivisor = 0;
	};
}
