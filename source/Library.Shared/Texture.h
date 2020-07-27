#pragma once

#include "Resource.h"
#include "Rectangle.h"

namespace Library
{
	class Texture : public Resource
	{
		RTTI_DECLARATIONS_ABSTRACT(Texture, Resource)

	protected:
		Texture() = default;

	public:
		virtual ~Texture() = default;
		Texture(const Texture&) = default;
		Texture& operator=(const Texture&) = default;
		Texture(Texture&&) = default;
		Texture& operator=(Texture&&) = default;

		virtual Resource::Type ResourceType() const override;
	};

	class Texture2D : public Texture
	{
		RTTI_DECLARATIONS_ABSTRACT(Texture2D, Texture)

	protected:
		Texture2D() = default;

	public:
		virtual ~Texture2D() = default;
		Texture2D(const Texture2D&) = default;
		Texture2D& operator=(const Texture2D&) = default;
		Texture2D(Texture2D&&) = default;
		Texture2D& operator=(Texture2D&&) = default;

		Texture2D(std::uint32_t width, std::uint32_t height);

		std::uint32_t Width() const;
		std::uint32_t Height() const;
		Rectangle Bounds() const;

	private:
		std::uint32_t mWidth{ 0 };
		std::uint32_t mHeight{ 0 };
	};

	class TextureCube : public Texture
	{
		RTTI_DECLARATIONS_ABSTRACT(TextureCube, Texture)

	public:
		TextureCube() = default;

	public:
		virtual ~TextureCube() = default;
		TextureCube(const TextureCube&) = default;
		TextureCube& operator=(const TextureCube&) = default;
		TextureCube(TextureCube&&) = default;
		TextureCube& operator=(TextureCube&&) = default;
	};
}

#include "Texture.inl"