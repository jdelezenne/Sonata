/*=============================================================================
Point.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

template <class T>
const Point<T> Point<T>::Empty = Point<T>();

template <class T>
SE_INLINE Point<T>::Point()
{
	X = Y = (T)0;
}

template <class T>
SE_INLINE Point<T>::Point(T x, T y)
{
	X = x;
	Y = y;
}

template <class T>
SE_INLINE Point<T>::Point(const Point<T>& other)
{
	X = other.X;
	Y = other.Y;
}

template <class T>
SE_INLINE bool Point<T>::operator==(const Point<T>& other) const
{
	return (X == other.X) && (Y == other.Y);
}

template <class T>
SE_INLINE bool Point<T>::operator!=(const Point<T>& other) const
{
	return (X != other.X) || (Y != other.Y);
}

template <class T>
SE_INLINE bool Point<T>::IsEmpty() const
{
	return (X == (T)0) && (Y == (T)0);
}

template <class T>
SE_INLINE Stream& operator<<(Stream& stream, Point<T>& other)
{
	return stream;// << other.X << other.Y;
}

template <class T>
SE_INLINE Stream& operator>>(Stream& stream, Point<T>& other)
{
	return stream;// >> other.X >> other.Y;
}
