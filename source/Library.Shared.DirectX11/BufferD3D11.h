#pragma once

#include "Buffer.h"
#include "CoreD3D.h"

namespace Library
{
	/**
	 * @brief Buffer implementation for Direct3D 11.
	 * @see Buffer
	*/
	class BufferD3D11 final : public Buffer
	{
		RTTI_DECLARATIONS_ABSTRACT(BufferD3D11, Buffer)

#pragma region Static Methods
	public:
		/**
		 * @brief Gets the size for a buffer with the given descriptor.
		 * @param desc Descriptor analyzed to get the size.
		 * @return Size of the described buffer in bytes.
		*/
		static constexpr UINT GetSize(const BufferDesc& desc);
#pragma endregion Static Methods

#pragma region Special Members
	public:
		/* Default Special Members */
		BufferD3D11() = delete;
		~BufferD3D11() = default;
		BufferD3D11(const BufferD3D11 & rhs) = default;
		BufferD3D11& operator=(const BufferD3D11 & rhs) = default;
		BufferD3D11(BufferD3D11 && rhs) = default;
		BufferD3D11& operator=(BufferD3D11 && rhs) = default;

		/**
		 * @brief Specialized constructor for initializing the buffer from a descriptor and/or initial data.
		 * @param device Device pointer to be used to create the buffer.
		 * @param desc Descriptor for the buffer to be initialized.
		 * @param initialData Optional data pointer used to initialize the buffer.
		*/
		BufferD3D11(ID3D11Device* device, const BufferDesc& desc, const void* initialData);
#pragma endregion Special Members

#pragma region Accessors
	public:
		/**
		 * @brief Gets the underlying native Direct3D 11 data buffer.
		 * @return Pointer to the underlying native Direct3D 11 data buffer.
		*/
		ID3D11Buffer* Native();

		/**
		 * @brief Gets the underlying native Direct3D 11 data buffer.
		 * @return Pointer to the underlying native Direct3D 11 data buffer.
		*/
		const ID3D11Buffer* Native() const;

		/**
		 * @brief Gets the size of the buffer.
		 * @return Size of the buffer in bytes.
		*/
		UINT Size() const;

		/**
		 * @brief Gets the width of each element in the buffer.
		 * @return Width of each element in the buffer in bytes.
		 * @remarks Stride is only used for a VertexBuffer. Otherwise, it is always 0.
		*/
		UINT Stride() const;

		/**
		 * @brief Gets the Native Direct3D 11 resource data format used to create the buffer.
		 * @return Native Direct3D 11 resource data format flag.
		*/
		DXGI_FORMAT Format() const;

		/**
		 * @brief Gets the Native Direct3D 11 resource usage.
		 * @return Native Direct3D 11 resource usage flag.
		*/
		D3D11_USAGE Usage() const;
#pragma endregion Accessors

#pragma region Helper Methods
	private:
		/**
		 * @brief Creates a new buffer for CPU access.
		 * @param device Device used to create the buffer.
		 * @param desc Descriptor used to create the buffer.
		*/
		void CreateCpuAccessBuffer(ID3D11Device* device, const BufferDesc& desc);
#pragma endregion Helper Methods

#pragma region Data Members
	private:
		winrt::com_ptr<ID3D11Buffer> mBuffer;			//!< @brief Underlying native Direct3D 11 data buffer.
		winrt::com_ptr<ID3D11Buffer> mCpuAccessBuffer;	//!< @brief Underlying native Direct3D 11 CPU access buffer.

		UINT mSize{ 0 };								//!< @brief Size of the buffer in bytes used to create the buffer.
		UINT mStride{ 0 };								//!< @brief Size of each element in bytes used to create the buffer.
		DXGI_FORMAT mFormat{ DXGI_FORMAT_UNKNOWN };		//!< @brief Native resource data format used to create the buffer.
		D3D11_USAGE mUsage{ D3D11_USAGE_DEFAULT };		//!< @brief Native resource usage flag used to create the buffer.
#pragma endregion Data Members
	};
}

// Inline File
#include "BufferD3D11.inl"