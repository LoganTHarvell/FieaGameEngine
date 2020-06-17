namespace Library
{
	inline bool Point::operator<(const Point& rhs) const
	{
		return (X == rhs.X ? Y < rhs.Y : X < rhs.X);
	}
}