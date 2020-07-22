#pragma once

#include "BufferWithResourceViewD3D11.h"

namespace Library::Direct3D11
{
	inline ID3D11ShaderResourceView* BufferWithResourceViewD3D11::ShaderResourceView() const
	{
		return mSRV.get();
	}

	inline ID3D11UnorderedAccessView* BufferWithResourceViewD3D11::UnorderedAccessView() const
	{
		return mUAV.get();
	}

	inline UINT BufferWithResourceViewD3D11::InitialCount() const
	{
		return mInitialCount;
	}

	inline DXGI_FORMAT BufferWithResourceViewD3D11::GetResourceViewFormat(const BufferDesc& desc)
	{
		if (desc.IsTypedBuffer())		return Direct3D::FormatMap[desc.FormatValue];
		if (desc.IsByteAddressBuffer()) return DXGI_FORMAT_R32_TYPELESS;

		return DXGI_FORMAT_UNKNOWN;
	}

	inline UINT BufferWithResourceViewD3D11::GetFlagsUAV(const BufferDesc& desc)
	{
		if (desc.BindFlagsValue.Storage)
		{
			if (desc.IsStructuredBuffer())
			{
				if (desc.MiscFlags.Append)	return D3D11_BUFFER_UAV_FLAG_APPEND;
				if (desc.MiscFlags.Counter)	return D3D11_BUFFER_UAV_FLAG_COUNTER;
			}
			else if (desc.IsByteAddressBuffer())
			{
				return D3D11_BUFFER_UAV_FLAG_RAW;
			}
		}
		
		return 0;
	}
}
