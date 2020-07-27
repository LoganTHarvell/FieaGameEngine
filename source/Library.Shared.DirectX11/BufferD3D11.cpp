#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "BufferD3D11.h"

// First Party
#include "ResourceD3D11.h"
#pragma endregion Includes

namespace Library
{
#pragma region Special Members
	BufferD3D11::BufferD3D11(ID3D11Device* device, const BufferDesc& desc, const void* initialData) :
		Buffer(desc.BindFlagsValue.BufferTypeValue)
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
		if (desc.CpuAccessFlagsValue.ReadOrWrite())
		{
			CreateCpuAccessBuffer(device, desc);
		}
		
		/* Store buffer creation attributes */
		mSize = descD3D.ByteWidth;
		mStride = (desc.VertexAttributes.IsEmpty() ? 0 : desc.Stride);
		mFormat = Direct3D::FormatMap[desc.FormatValue];
		mUsage = descD3D.Usage;
	}
#pragma endregion Special Members

#pragma region Modifiers
	void BufferD3D11::UpdateSubresource(ID3D11DeviceContext* context, const void* data, const UINT dataSize, const UINT offset)
	{
		/* Validate parameters */
		assert(dataSize + offset <= Size());

		if (mUsage == D3D11_USAGE_DYNAMIC)
		{
			/* Discard previous content if the entire resource will be updated */
			const auto writeDiscard = (offset == 0 && dataSize == Size()) ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE;

			/* Update partial subresource by mapping buffer from GPU into CPU memory space */
			D3D11_MAPPED_SUBRESOURCE subresource;
			context->Map(mBuffer.get(), 0, writeDiscard, 0, &subresource);
			{
				std::memcpy(static_cast<char*>(subresource.pData) + offset, data, dataSize);
			}
			
			context->Unmap(Native(), 0);
		}
		else
		{
			if (BufferType() == BufferType::Constant)
			{
				/* Update entire subresource */
				if (dataSize == Size())
					context->UpdateSubresource(Native(), 0, nullptr, data, 0, 0);
				else
					throw std::out_of_range("cannot update D3D11 buffer partially when it is created with static usage");
			}
			else
			{
				/* Update sub region of buffer */
				const D3D11_BOX dstBox{ offset, 0, 0, offset + dataSize, 1, 1 };
				context->UpdateSubresource(Native(), 0, &dstBox, data, 0, 0);
			}
		}
	}

	void BufferD3D11::UpdateSubresource(ID3D11DeviceContext* context, const void* data)
	{
		context->UpdateSubresource(Native(), 0, nullptr, data, 0, 0);
	}
#pragma endregion Modifiers

#pragma region Helper Methods
	void BufferD3D11::CreateCpuAccessBuffer(ID3D11Device* device, const BufferDesc& desc)
	{
		D3D11_BUFFER_DESC descD3D;
		{
			descD3D.ByteWidth = static_cast<UINT>(desc.Size);
			descD3D.Usage = D3D11::GetCpuAccessBufferUsage(desc); 
			descD3D.CPUAccessFlags = D3D11::GetCpuAccessFlags(desc.CpuAccessFlagsValue);
			descD3D.MiscFlags = 0;
			descD3D.StructureByteStride = desc.Stride;
		}
		
		const auto hr = device->CreateBuffer(&descD3D, nullptr, mCpuAccessBuffer.put());
		ThrowIfFailed(hr, "ID3D11Buffer for CPU-access buffer");
	}
#pragma endregion Helper Methods
}
