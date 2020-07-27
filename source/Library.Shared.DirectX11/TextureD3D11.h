#pragma once

#include <winrt/Windows.Foundation.h>
#include <d3d11.h>

#include "Texture.h"

namespace Library
{
	class TextureD3D11 final : public Texture
	{
		RTTI_DECLARATIONS(TextureD3D11, Texture)

	public:
		TextureD3D11() = default;
		TextureD3D11(const TextureD3D11&) = default;
		TextureD3D11& operator=(const TextureD3D11&) = default;
		TextureD3D11(TextureD3D11&&) = default;
		TextureD3D11& operator=(TextureD3D11&&) = default;
		virtual ~TextureD3D11() = default;

		explicit TextureD3D11(const winrt::com_ptr<ID3D11ShaderResourceView>& shaderResourceView);

	public:
		winrt::com_ptr<ID3D11ShaderResourceView> ShaderResourceView() const;

	protected:
		winrt::com_ptr<ID3D11ShaderResourceView> mShaderResourceView;
	};

	class Texture2DD3D11 final : public Texture2D
	{
		RTTI_DECLARATIONS(Texture2DD3D11, Texture2D)

	public:
		Texture2DD3D11() = default;
		virtual ~Texture2DD3D11() = default;
		Texture2DD3D11(const Texture2DD3D11&) = default;
		Texture2DD3D11& operator=(const Texture2DD3D11&) = default;
		Texture2DD3D11(Texture2DD3D11&&) = default;
		Texture2DD3D11& operator=(Texture2DD3D11&&) = default;

		Texture2DD3D11(const winrt::com_ptr<ID3D11ShaderResourceView>& shaderResourceView, std::uint32_t width, std::uint32_t height);

	public:
		static std::shared_ptr<Texture2D> CreateTexture2D(gsl::not_null<ID3D11Device*> device, const D3D11_TEXTURE2D_DESC& textureDesc);
		static std::shared_ptr<Texture2D> CreateTexture2D(gsl::not_null<ID3D11Device*> device, std::uint32_t width, std::uint32_t height, std::uint32_t mipLevels = 1, std::uint32_t arraySize = 1, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SAMPLE_DESC sampleDesc = { 1, 0 }, std::uint32_t bindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE, std::uint32_t cpuAccessFlags = 0);

	public:
		winrt::com_ptr<ID3D11ShaderResourceView> ShaderResourceView() const;

	protected:
		winrt::com_ptr<ID3D11ShaderResourceView> mShaderResourceView;
	};

	class TextureCubeD3D11 final : public TextureCube
	{
		RTTI_DECLARATIONS(TextureCubeD3D11, TextureCube)

	public:
		TextureCubeD3D11() = default;
		TextureCubeD3D11(const TextureCubeD3D11&) = default;
		TextureCubeD3D11& operator=(const TextureCubeD3D11&) = default;
		TextureCubeD3D11(TextureCubeD3D11&&) = default;
		TextureCubeD3D11& operator=(TextureCubeD3D11&&) = default;
		~TextureCubeD3D11() = default;

		explicit TextureCubeD3D11(const winrt::com_ptr<ID3D11ShaderResourceView>& shaderResourceView);
		
	public:
		winrt::com_ptr<ID3D11ShaderResourceView> ShaderResourceView() const;
		
	private:
		winrt::com_ptr<ID3D11ShaderResourceView> mShaderResourceView;
	};
}

#include "TextureD3D11.inl"