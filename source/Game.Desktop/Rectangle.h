#pragma once

#include "Point.h"

namespace Library
{
	class Rectangle final
	{
	public:		
		std::int32_t X;
		std::int32_t Y;
		std::int32_t Width;
		std::int32_t Height;

		Rectangle(const Point& location, const Point& size);		
		Rectangle(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height);
		Rectangle(const Rectangle&) = default;
		Rectangle(Rectangle&&) = default;
		Rectangle& operator=(const Rectangle&) = default;
		Rectangle& operator=(Rectangle&&) = default;
		~Rectangle() = default;

		std::int32_t Left() const;
		std::int32_t Right() const;
		std::int32_t Top() const;
		std::int32_t Bottom() const;
		Point Size() const;
		void SetSize(const Point& size);
		Point Center() const;
		Point Location() const;
		void SetLocation(Point& location);
		
		bool Intersects(const Rectangle& other) const;		

		static const Rectangle Empty;
	};
}

#include "Rectangle.inl"