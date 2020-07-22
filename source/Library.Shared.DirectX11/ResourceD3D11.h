#pragma once

#include "CoreD3D.h"

namespace Library
{
	// Forward Declarations
	struct BufferDesc;
	struct CpuAccessFlags;
	struct MiscResourceFlags;
	struct BindFlags;

	// TODO: Potentially move all of these into BufferD3D11 class
	namespace D3D11
	{
		/**
		 * @brief Gets the native Direct3D 11 usage flag based on the descriptor
		 * @param desc Abstracted buffer descriptor to be analyzed for the appropriate flag
		 * @return Native Direct3D 11 usage flag
		*/
		D3D11_USAGE GetBufferUsage(const BufferDesc& desc);

		/**
		 * @brief Gets the native Direct3D 11 binding flags based on the descriptor
		 * @param desc Abstracted buffer descriptor to be analyzed for the appropriate flag
		 * @return Native Direct3D 11 binding flags
		*/
		UINT GetBufferBindFlags(const BufferDesc& desc);

		/**
		 * @brief Gets the native Direct3D 11 misc flags based on the descriptor
		 * @param desc Abstracted buffer descriptor to be analyzed for the appropriate flag
		 * @return Native Direct3D 11 misc flags
		*/
		UINT GetBufferMiscFlags(const BufferDesc& desc);

		/**
		 * @brief Gets the native Direct3D 11 CPU access flags based on the misc flags
		 * @param miscFlags Abstracted misc flags to be analyzed for the appropriate CPU access flags
		 * @return Native Direct3D 11 CPU access flags
		 * @see MiscFlags
		*/
		UINT GetCpuAccessFlags(const MiscResourceFlags& miscFlags);

		/**
		 * @brief Gets the native Direct3D 11 CPU access flags based on the CPU access flags
		 * @param cpuAccessFlags Abstracted CPU access flags to be converted to the appropriate native flags
		 * @return Native Direct3D 11 CPU access flags
		 * @see MiscFlags
		*/
		UINT GetCpuAccessFlags(const CpuAccessFlags& cpuAccessFlags);

		/**
		 * @brief Gets the native Direct3D 11 CPU buffer usage flag based on the descriptor
		 * @param desc Abstracted buffer descriptor to be analyzed for the appropriate flags
		 * @return Native Direct3D 11 CPU access flags flags
		 * @see MiscFlags
		*/
		D3D11_USAGE GetCpuAccessBufferUsage(const BufferDesc& desc);

		/**
		 * @brief Determines whether the bind flags denote a Buffer that has resource views
		 * @param bindFlags Bind flags to be analyzed
		 * @return True, if the bind flags denote a buffer with resource views, otherwise false
		*/
		bool RequiresBufferWithRV(const BindFlags& bindFlags);
	}
}
