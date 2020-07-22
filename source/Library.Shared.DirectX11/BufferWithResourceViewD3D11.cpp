// Pre-compiled Header
#include "pch.h"

// Header
#include "BufferWithResourceViewD3D11.h"

namespace Library::Direct3D11
{
	BufferWithResourceViewD3D11::BufferWithResourceViewD3D11(
		ID3D11Device*		device, 
		const BufferDesc&	desc, 
		const void*			initialData)
	:
		BufferD3D11(device, desc, initialData),
		mFlagsUAV(GetFlagsUAV(desc))
	{
		const UINT stride = GetStorageBufferStride(desc);
		const DXGI_FORMAT format = GetResourceViewFormat(desc);
		const UINT numElements = gsl::narrow_cast<UINT>(desc.Size) / stride;

		if (desc.BindFlagsValue.Sampled)
		{
			CreateSRVHelper(device, format, 0, numElements);
		}

		if (desc.BindFlagsValue.Storage)
		{
			CreateUAVHelper(device, format, 0, numElements);
		}
	}

	void BufferWithResourceViewD3D11::CreateShaderResourceView(
		ID3D11Device*				device,
		const DXGI_FORMAT			format, 
		ID3D11ShaderResourceView**	srvOut,
		const UINT					firstElement,
		const UINT					numElements)
	{
		/* Initialize descriptor and create SRV */
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		{
			desc.Format = format;
			desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			desc.Buffer.FirstElement = firstElement;
			desc.Buffer.NumElements = numElements;
		}

		ThrowIfFailed(device->CreateShaderResourceView(Native(), &desc, srvOut), "ID3D11ShaderResourceView");
	}

	void BufferWithResourceViewD3D11::CreateUnorderedAccessView(
		ID3D11Device*				device,
		const DXGI_FORMAT			format,
		ID3D11UnorderedAccessView** uavOut,
		const UINT					firstElement, 
		const UINT					numElements)
	{
		/* Initialize descriptor and create UAV */
		D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
		{
			desc.Format = format;
			desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			desc.Buffer.FirstElement = firstElement;
			desc.Buffer.NumElements = numElements;
			desc.Buffer.Flags = mFlagsUAV;
		}
		
		ThrowIfFailed(device->CreateUnorderedAccessView(Native(), &desc, uavOut), "ID3D11UnorderedAccessView");
	}

	void BufferWithResourceViewD3D11::CreateSRVHelper(ID3D11Device* device, const DXGI_FORMAT format,
		const UINT firstElement, const UINT numElements)
	{
		CreateShaderResourceView(device, format, mSRV.put(), firstElement, numElements);
	}

	void BufferWithResourceViewD3D11::CreateUAVHelper(ID3D11Device* device, const DXGI_FORMAT format,
		const UINT firstElement, const UINT numElements)
	{
		CreateUnorderedAccessView(device, format, mUAV.put(), firstElement, numElements);
	}
}
