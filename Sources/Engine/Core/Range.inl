/*=============================================================================
Range.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

template <class T>
Range<T>::Range() :
	Min(T()),
	Max(T())
{
}

template <class T>
Range<T>::Range(const T& min, const T& max) :
	Min(min),
	Max(max)
{
}

template <class T>
void Range<T>::Get(T& min, T& max) const
{
	min = Min;
	max = Max;
}

template <class T>
void Range<T>::Set(const T& min, const T& max)
{
	Min = min;
	Max = max;
}

template <class T>
bool Range<T>::operator==(const Range& other) const
{
	return (Min == other.Min && Max == other.Max);
}

template <class T>
bool Range<T>::operator!=(const Range& other) const
{
	return !(*this == other);
}
