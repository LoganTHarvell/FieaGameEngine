#include "pch.h"

// Header
#include "RenderContextD3D11.h"

namespace Library
{
	RenderContextD3D11::RenderContextD3D11(const winrt::com_ptr<ID3D11Device>& device, const winrt::com_ptr<ID3D11DeviceContext>& context) :
		mDevice(device), mContext(context)
	{
	}
}
