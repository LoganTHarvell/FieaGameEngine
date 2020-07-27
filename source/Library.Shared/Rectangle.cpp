#include "pch.h"
#include "Rectangle.h"

using namespace std;

namespace Library
{
	const Rectangle Rectangle::Empty{ 0, 0, 0, 0 };

	Rectangle::Rectangle(const Point& location, const Point& size) :
		X(location.X), Y(location.Y), Width(size.X), Height(size.Y)
	{
	}

	Rectangle::Rectangle(int32_t x, int32_t y, int32_t width, int32_t height) :
		X(x), Y(y), Width(width), Height(height)
	{
	}

	bool Rectangle::Intersects(const Rectangle& other) const
	{
		return other.Left() < Right() && Left() < other.Right() &&
			other.Top() < Bottom() && Top() < other.Bottom();
	}
}
