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
	inline Buffer::Type Buffer::GetType() const
	{
		return mType;
	}

	inline std::uint32_t Buffer::GetStorageBufferStride(const BufferDesc& desc) const
	{
		if (desc.Stride > 0) return desc.Stride;

		if (desc.Format != Format::Unknown)
		{
			return std::max(1u, (FormatDescMap[desc.Format].BitSize / 8u));
		}
		
		return 1;
	}

	inline bool BufferDesc::IsStructuredBuffer() const
	{
		return Stride > 0 && (BindFlags.Sampled || BindFlags.Storage);
	}

	inline bool BufferDesc::IsTypedBuffer() const
	{
		return Stride == 0 && Format != Format::Unknown
			&& (BindFlags.Sampled || BindFlags.Storage);
	}

	inline bool BufferDesc::IsByteAddressBuffer() const
	{
		return Stride == 0 && Format == Format::Unknown
			&& (BindFlags.Sampled || BindFlags.Storage);
	}
#pragma endregion Accessors
}
