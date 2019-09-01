/*=============================================================================
Size.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

template <class T>
const Size<T> Size<T>::Empty = Size<T>();

template <class T>
SE_INLINE Size<T>::Size()
{
	Width = Height = (T)0;
}

template <class T>
SE_INLINE Size<T>::Size(T width, T height)
{
	Width = width;
	Height = height;
}

template <class T>
SE_INLINE Size<T>::Size(const Size<T>& other)
{
	Width = other.Width;
	Height = other.Height;
}

template <class T>
SE_INLINE bool Size<T>::operator==(const Size<T>& other) const
{
	return (Width == other.Width) && (Height == other.Height);
}

template <class T>
SE_INLINE bool Size<T>::operator!=(const Size<T>& other) const
{
	return (Width != other.Width) || (Height != other.Height);
}

template <class T>
SE_INLINE bool Size<T>::IsEmpty() const
{
	return (Width == (T)0) && (Height == (T)0);
}

template <class T>
SE_INLINE Stream& operator<<(Stream& stream, Size<T>& other)
{
	return stream;// << other.Width << other.Height;
}

template <class T>
SE_INLINE Stream& operator>>(Stream& stream, Size<T>& other)
{
	return stream;// >> other.Width >> other.Height;
}
