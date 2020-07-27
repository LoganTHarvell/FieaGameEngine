#include "Rectangle.h"

namespace Library
{	
	inline std::int32_t Rectangle::Left() const
	{
		return X;
	}

	inline std::int32_t Rectangle::Right() const
	{
		return X + Width;
	}

	inline std::int32_t Rectangle::Top() const
	{
		return Y;
	}

	inline std::int32_t Rectangle::Bottom() const
	{
		return Y + Height;
	}

	inline Point Rectangle::Size() const
	{
		return Point(Width, Height);
	}

	inline void Rectangle::SetSize(const Point& size)
	{
		Width = size.X;
		Height = size.Y;
	}

	inline Point Rectangle::Center() const
	{
		return Point(X + (Width / 2), Y + (Height / 2));
	}

	inline Point Rectangle::Location() const
	{
		return Point(X, Y);
	}

	inline void Rectangle::SetLocation(Point& location)
	{
		X = location.X;
		Y = location.Y;
	}
}