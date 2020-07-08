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

	inline bool BufferDesc::IsStructuredBuffer() const
	{
		return Stride > 0 && (BindFlags.Sampled || BindFlags.Storage);
	}

	inline bool BufferDesc::IsTypedBuffer() const
	{
		return Stride == 0 && Format != ResourceDataFormat::Unknown
			&& (BindFlags.Sampled || BindFlags.Storage);
	}

	inline bool BufferDesc::IsByteAddressBuffer() const
	{
		return Stride == 0 && Format == ResourceDataFormat::Unknown
			&& (BindFlags.Sampled || BindFlags.Storage);
	}
#pragma endregion Accessors
}
