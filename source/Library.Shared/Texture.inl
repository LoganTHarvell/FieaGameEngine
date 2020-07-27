#pragma once

namespace Library
{
	inline Resource::Type Texture::ResourceType() const
	{
		return Resource::Type::Texture;
	}

	inline std::uint32_t Texture2D::Width() const
	{
		return mWidth;
	}

	inline std::uint32_t Texture2D::Height() const
	{
		return mHeight;
	}

	inline Rectangle Texture2D::Bounds() const
	{
		return Rectangle(0, 0, gsl::narrow<int32_t>(mWidth), gsl::narrow<int32_t>(mHeight));
	}

	inline Texture2D::Texture2D(std::uint32_t width, std::uint32_t height) :
		mWidth(width), mHeight(height)
	{
	}
}