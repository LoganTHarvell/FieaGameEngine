#pragma once

// First Party
#include "BufferD3D11.h"

namespace Library::Direct3D11
{
	class BufferWithResourceViewD3D11 final : public BufferD3D11
	{
#pragma region Special Members
	public:
		/* Default Special Members */
		BufferWithResourceViewD3D11() = delete;
		~BufferWithResourceViewD3D11() = default;
		BufferWithResourceViewD3D11(const BufferWithResourceViewD3D11 & rhs) = default;
		BufferWithResourceViewD3D11& operator=(const BufferWithResourceViewD3D11 & rhs) = default;
		BufferWithResourceViewD3D11(BufferWithResourceViewD3D11 && rhs) = default;
		BufferWithResourceViewD3D11& operator=(BufferWithResourceViewD3D11 && rhs) = default;

		/**
		 * @brief Specialized constructor for initializing the buffer from a descriptor and/or initial data.
		 * @param device Device pointer to be used to create the buffer.
		 * @param desc Descriptor for the buffer to be initialized.
		 * @param initialData Optional data pointer used to initialize the buffer.
		*/
		BufferWithResourceViewD3D11(ID3D11Device* device, const BufferDesc& desc, const void* initialData=nullptr);
#pragma endregion Special Members

	public:
		void CreateShaderResourceView(
			ID3D11Device*				device,
			const DXGI_FORMAT			format,
			ID3D11ShaderResourceView**	srvOut,
			const UINT					firstElement,
			const UINT					numElements);
		
		void CreateUnorderedAccessView(
			ID3D11Device*				device,
			const DXGI_FORMAT			format,
			ID3D11UnorderedAccessView** uavOut,
			const UINT					firstElement,
			const UINT					numElements);
		
		ID3D11ShaderResourceView* ShaderResourceView() const;
		ID3D11UnorderedAccessView* UnorderedAccessView() const;
		UINT InitialCount() const;
		
	private:
		static DXGI_FORMAT GetResourceViewFormat(const BufferDesc& desc);
		static UINT GetFlagsUAV(const BufferDesc& desc);
		void CreateSRVHelper(ID3D11Device* device, const DXGI_FORMAT format, const UINT firstElement, const UINT numElements);
		void CreateUAVHelper(ID3D11Device* device, const DXGI_FORMAT format, const UINT firstElement, const UINT numElements);
		
	private:
		winrt::com_ptr<ID3D11ShaderResourceView> mSRV;
		winrt::com_ptr<ID3D11UnorderedAccessView> mUAV;
		
		UINT mFlagsUAV{ 0 };
		UINT mInitialCount{ std::numeric_limits<UINT>::max() };
	};
}

// Inline File
#include "BufferWithResourceViewD3D11.inl"
