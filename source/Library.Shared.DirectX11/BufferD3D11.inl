#pragma once

#include "BufferD3D11.h"
#include <Utility.h>

namespace Library
{
#pragma region Static Methods
	constexpr UINT BufferD3D11::GetSize(const BufferDesc& desc)
	{
		UINT size = static_cast<UINT>(desc.Size);
		return desc.BindFlags.BufferType == BufferType::Constant ? AlignedSize(size, 16u) : size;
	}
#pragma endregion Static Methods
	
#pragma region Accessors
	inline ID3D11Buffer* BufferD3D11::Native()
	{
		return mBuffer.get();
	}

	inline const ID3D11Buffer* BufferD3D11::Native() const
	{
		return mBuffer.get();
	}

	inline UINT BufferD3D11::Size() const
	{
		return mSize;
	}

	inline UINT BufferD3D11::Stride() const
	{
		return mStride;
	}

	inline DXGI_FORMAT BufferD3D11::Format() const
	{
		return mFormat;
	}

	inline D3D11_USAGE BufferD3D11::Usage() const
	{
		return mUsage;
	}
#pragma endregion Accessors
}
