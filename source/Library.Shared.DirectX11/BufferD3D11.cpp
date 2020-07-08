#include "pch.h"

#include "BufferD3D11.h"

#include "ResourceD3D11.h"

namespace Library
{
#pragma region Special Members
	BufferD3D11::BufferD3D11(ID3D11Device* device, const BufferDesc& desc, const void* initialData) :
		Buffer(desc.BindFlags.BufferType)
	{
		/* Initialize native buffer desc */
		D3D11_BUFFER_DESC descD3D;
		{
			descD3D.ByteWidth = GetSize(desc);
			descD3D.Usage = D3D11::GetBufferUsage(desc);
			descD3D.BindFlags = D3D11::GetBufferBindFlags(desc);
			descD3D.CPUAccessFlags = D3D11::GetCpuAccessFlags(desc.MiscFlags);
			descD3D.MiscFlags = D3D11::GetBufferMiscFlags(desc);
			descD3D.StructureByteStride = desc.Stride;
		}

		if (initialData)
		{
			/* Create native D3D11 buffer with initial sub-resource data */
			D3D11_SUBRESOURCE_DATA subResourceData;
			{
				subResourceData.pSysMem = initialData;
				subResourceData.SysMemPitch = 0;
				subResourceData.SysMemSlicePitch = 0;
			}
        	
			ThrowIfFailed(device->CreateBuffer(&descD3D, &subResourceData, mBuffer.put()), "ID3D11Buffer");
		}
		else
		{
			/* Create native D3D11 buffer */
			ThrowIfFailed(device->CreateBuffer(&descD3D, nullptr, mBuffer.put()), "ID3D11Buffer");
		}

		/* Create CPU access buffer (if required) */
		if (desc.CpuAccessFlags.ReadOrWrite())
		{
			CreateCpuAccessBuffer(device, desc);
		}
		
		/* Store buffer creation attributes */
		mSize = descD3D.ByteWidth;
		mStride = 0;//(desc.VertexAttributesList.IsEmpty() ? 0 : desc.VertexAttributesList.Front().Stride);
		mFormat = Direct3D::ResourceDataFormatMap[desc.Format];
		mUsage = descD3D.Usage;
	}
#pragma endregion Special Members

#pragma region Helper Methods
	void BufferD3D11::CreateCpuAccessBuffer(ID3D11Device* device, const BufferDesc& desc)
	{
		D3D11_BUFFER_DESC descD3D;
		{
			descD3D.ByteWidth = static_cast<UINT>(desc.Size);
			descD3D.Usage = D3D11::GetCpuAccessBufferUsage(desc); 
			descD3D.CPUAccessFlags = D3D11::GetCpuAccessFlags(desc.CpuAccessFlags);
			descD3D.MiscFlags = 0;
			descD3D.StructureByteStride = desc.Stride;
		}
		
		const auto hr = device->CreateBuffer(&descD3D, nullptr, mCpuAccessBuffer.put());
		ThrowIfFailed(hr, "ID3D11Buffer for CPU-access buffer");
	}
#pragma endregion Helper Methods
}
