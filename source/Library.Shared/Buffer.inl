#pragma once

#include "Buffer.h"

namespace Library
{
#pragma region Special Members
	inline Buffer::Buffer(const Type type) : mType(type)
	{
	}
#pragma endregion Special Members

#pragma region Accessors
	inline Resource::Type Buffer::ResourceType() const
	{
		return Resource::Type::Buffer;
	}

	inline Buffer::Type Buffer::BufferType() const
	{
		return mType;
	}

	inline std::uint32_t Buffer::GetStorageBufferStride(const BufferDesc& desc) const
	{
		if (desc.Stride > 0) return desc.Stride;

		if (desc.FormatValue != Format::Unknown)
		{
			return std::max(1u, (FormatDescMap[desc.FormatValue].BitSize / 8u));
		}
		
		return 1;
	}

	inline bool BufferDesc::IsStructuredBuffer() const
	{
		return Stride > 0 && (BindFlagsValue.Sampled || BindFlagsValue.Storage);
	}

	inline bool BufferDesc::IsTypedBuffer() const
	{
		return Stride == 0 && FormatValue != Format::Unknown
			&& (BindFlagsValue.Sampled || BindFlagsValue.Storage);
	}

	inline bool BufferDesc::IsByteAddressBuffer() const
	{
		return Stride == 0 && FormatValue == Format::Unknown
			&& (BindFlagsValue.Sampled || BindFlagsValue.Storage);
	}
#pragma endregion Accessors
}
