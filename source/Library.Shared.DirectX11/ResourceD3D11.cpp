#pragma region Includes
//Pre-compiled Header
#include "pch.h"

// Header
#include "ResourceD3D11.h"

// First Party
#include "Buffer.h"
#pragma endregion Includes

namespace Library::D3D11
{
	D3D11_USAGE GetBufferUsage(const BufferDesc& desc)
	{
		if (!desc.BindFlags.Storage && desc.MiscFlags.DynamicUsage)
		{
			return D3D11_USAGE_DYNAMIC;
		}

		return D3D11_USAGE_DEFAULT;
	}

	UINT GetBufferBindFlags(const BufferDesc& desc)
	{
		UINT flags = 0;

		switch (desc.BindFlags.BufferType)
		{
		case Buffer::Type::Vertex:
			flags |= D3D11_BIND_VERTEX_BUFFER;
			break;

		case Buffer::Type::Index:
			flags |= D3D11_BIND_INDEX_BUFFER;
			break;

		case Buffer::Type::Constant:
			flags |= D3D11_BIND_CONSTANT_BUFFER;
			break;

		case Buffer::Type::StreamOutput:
			flags |= D3D11_BIND_STREAM_OUTPUT;
			break;

		default:
			break;
		}

		if (desc.BindFlags.Sampled || desc.BindFlags.CopySource)
		{
			flags |= D3D11_BIND_SHADER_RESOURCE;
		}

		if (desc.BindFlags.Storage || desc.BindFlags.CopyDestination)
		{
			flags |= D3D11_BIND_UNORDERED_ACCESS;
		}

		return flags;
	}

	UINT GetBufferMiscFlags(const BufferDesc& desc)
	{
		UINT flags = 0;

		if (desc.BindFlags.BufferType == BufferType::Indirect)
		{
			flags |= D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
		}

		if (desc.IsStructuredBuffer())
		{
			flags |= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		}
		else if (desc.IsByteAddressBuffer())
		{
			flags |= D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
		}

		return flags;
	}

	UINT GetCpuAccessFlags(const MiscResourceFlags& miscFlags)
	{
		UINT flags = 0;

		if (miscFlags.DynamicUsage)
		{
			flags |= D3D11_CPU_ACCESS_WRITE;
		}

		return flags;
	}

	UINT GetCpuAccessFlags(const CpuAccessFlags& cpuAccessFlags)
	{
		UINT flags = 0;

		if (cpuAccessFlags.Read)
		{
			flags |= D3D11_CPU_ACCESS_READ;
		}

		if (cpuAccessFlags.Write)
		{
			flags |= D3D11_CPU_ACCESS_WRITE;
		}

		return flags;
	}

	D3D11_USAGE GetCpuAccessBufferUsage(const BufferDesc& desc)
	{
		if (desc.CpuAccessFlags.Read)	return D3D11_USAGE_STAGING;
		if (desc.CpuAccessFlags.Write)	return D3D11_USAGE_DYNAMIC;

		return D3D11_USAGE_DEFAULT;
	}

	bool RequiresBufferWithRV(const BindFlags& bindFlags)
	{
		return bindFlags.Sampled || bindFlags.Storage;
	}
}
