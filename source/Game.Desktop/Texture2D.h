#pragma once

#include <gsl\gsl>
#include "Texture.h"
#include "Rectangle.h"

namespace Library
{
	class Texture2D final : public Texture
	{
		RTTI_DECLARATIONS(Texture2D, Texture)

	public:
		Texture2D() = default;
		Texture2D(const Texture2D&) = default;
		Texture2D& operator=(const Texture2D&) = default;
		Texture2D(Texture2D&&) = default;
		Texture2D& operator=(Texture2D&&) = default;
		~Texture2D() = default;
		
		Texture2D(const winrt::com_ptr<ID3D11ShaderResourceView>& shaderResourceView, std::uint32_t width, std::uint32_t height);

		static std::shared_ptr<Texture2D> CreateTexture2D(gsl::not_null<ID3D11Device*> device, const D3D11_TEXTURE2D_DESC& textureDesc);
		static std::shared_ptr<Texture2D> CreateTexture2D(gsl::not_null<ID3D11Device*> device, std::uint32_t width, std::uint32_t height, std::uint32_t mipLevels = 1, std::uint32_t arraySize = 1, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SAMPLE_DESC sampleDesc = { 1, 0 }, std::uint32_t bindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE, std::uint32_t cpuAccessFlags = 0);

		std::uint32_t Width() const;
		std::uint32_t Height() const;
		Rectangle Bounds() const;

	private:
		std::uint32_t mWidth{ 0 };
		std::uint32_t mHeight{ 0 };
	};
}