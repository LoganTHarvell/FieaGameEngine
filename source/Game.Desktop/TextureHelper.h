#pragma once

#include <d3d11.h>
#include <gsl\gsl>
#include "Rectangle.h"

namespace Library
{
	class TextureHelper final
	{
	public:
		static Point GetTextureSize(gsl::not_null<ID3D11Texture2D*> texture);
		static Rectangle GetTextureBounds(gsl::not_null<ID3D11Texture2D*> texture);
		static std::uint32_t BitsPerPixel(const DXGI_FORMAT format);
		
		TextureHelper() = delete;
		TextureHelper(const TextureHelper&) = delete;
		TextureHelper& operator=(const TextureHelper&) = delete;
		TextureHelper(TextureHelper&&) = delete;
		TextureHelper& operator=(TextureHelper&&) = delete;
		~TextureHelper() = default;
	};
}