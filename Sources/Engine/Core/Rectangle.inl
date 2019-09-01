/*=============================================================================
Rectangle.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

template <class T>
const Rectangle<T> Rectangle<T>::Empty = Rectangle<T>();

template <class T>
SE_INLINE Rectangle<T>::Rectangle()
{
	X = Y = Width = Height = (T)0;
}

template <class T>
SE_INLINE Rectangle<T>::Rectangle(T x, T y, T width, T height)
{
	X = x;
	Y = y;
	Width = width;
	Height = height;
}

template <class T>
SE_INLINE Rectangle<T>::Rectangle(const Point<T>& location, const Size<T>& size)
{
	X = location.X;
	Y = location.Y;
	Width = size.Width;
	Height = size.Height;
}

template <class T>
SE_INLINE Rectangle<T>::Rectangle(const Rectangle<T>& other)
{
	X = other.X;
	Y = other.Y;
	Width = other.Width;
	Height = other.Height;
}

template <class T>
SE_INLINE Point<T> Rectangle<T>::GetLocation() const
{
	return Point<T>(X, Y);
}

template <class T>
SE_INLINE void Rectangle<T>::SetLocation(const Point<T>& location)
{
	X = location.X;
	Y = location.Y;
}

template <class T>
SE_INLINE Size<T> Rectangle<T>::GetSize() const
{
	return Size<T>(Width, Height);
}

template <class T>
SE_INLINE void Rectangle<T>::SetSize(const Size<T>& size)
{
	Width = size.Width;
	Height = size.Height;
}

template <class T>
SE_INLINE bool Rectangle<T>::operator==(const Rectangle<T>& other) const
{
	return (X == other.X) && (Y == other.Y) && (Width == other.Width) && (Height == other.Height);
}

template <class T>
SE_INLINE bool Rectangle<T>::operator!=(const Rectangle<T>& other) const
{
	return (X != other.X) || (Y != other.Y) || (Width != other.Width) || (Height != other.Height);
}

template <class T>
SE_INLINE bool Rectangle<T>::IsEmpty() const
{
	return (X == (T)0) && (Y == (T)0) && (Width == (T)0) && (Height == (T)0);
}

template <class T>
SE_INLINE bool Rectangle<T>::Contains(T x, T y) const
{
	return ((X <= x) && (x < X + Width) && (Y <= y) && (y < Y + Height));
}

template <class T>
SE_INLINE void Rectangle<T>::Offset(T x, T y)
{
	X += x;
	Y += y;
}

template <class T>
SE_INLINE void Rectangle<T>::Inflate(T width, T height)
{
	X -= width;
	Y -= height;
	Width += (2 * width);
	Height += (2 * height);
}

template <class T>
SE_INLINE void Rectangle<T>::Intersect(const Rectangle<T>& value)
{
	*this = Rectangle<T>::Intersect(*this, value);
}

template <class T>
SE_INLINE Rectangle<T> Rectangle<T>::Intersect(const Rectangle<T>& left, const Rectangle<T>& right)
{
	T l, r, t, b;

	l = Math::Max(left.X, right.X);
	r = Math::Min((left.X + left.Width), (right.X + right.Width));
	t = Math::Max(left.Y, right.Y);
	b = Math::Min((left.Y + left.Height), (right.Y + right.Height));

	if (r < l)
	{
		return Rectangle<T>::Empty;
	}
	if (b < t)
	{
		return Rectangle<T>::Empty;
	}
	return Rectangle<T>(l, t, (r - l), (b - t));
}

template <class T>
SE_INLINE void Rectangle<T>::Union(const Rectangle<T>& value)
{
	*this = Rectangle<T>::Union(*this, value);
}

template <class T>
SE_INLINE Rectangle<T> Rectangle<T>::Union(const Rectangle<T>& left, const Rectangle<T>& right)
{
	T l, r, t, b;

	l = Math::Min(left.X, right.X);
	r = Math::Max((left.X + left.Width), (right.X + right.Width));
	t = Math::Min(left.Y, right.Y);
	b = Math::Max((left.Y + left.Height), (right.Y + right.Height));

	return Rectangle<T>(l, t, (r - l), (b - t));
}

template <class T>
SE_INLINE Stream& operator<<(Stream& stream, Rectangle<T>& other)
{
	return stream;// << other.X << other.Y << other.Width << other.Height;
}

template <class T>
SE_INLINE Stream& operator>>(Stream& stream, Rectangle<T>& other)
{
	return stream;// >> other.X >> other.Y >> other.Width >> other.Height;
}
