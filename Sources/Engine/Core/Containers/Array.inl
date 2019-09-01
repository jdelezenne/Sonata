/*=============================================================================
Array.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

template <class T>
Array<T>::Array() :
	BaseArray<T>()
{
}

template <class T>
Array<T>::Array(int size) :
	BaseArray<T>(size)
{
}

template <class T>
Array<T>::Array(T* data, int size) :
	BaseArray<T>(data, size)
{
}

template <class T>
Array<T>::Array(const T* data, int size) :
	BaseArray<T>(data, size)
{
}

template <class T>
Array<T>::Array(const Array<T>& value) :
	BaseArray<T>(value)
{
}

template <class T>
Array<T>::~Array()
{
}

template <class T>
bool Array<T>::Contains(const T& value) const
{
	for (InnerType::const_iterator it = _internal.begin(); it != _internal.end(); it++)
	{
		if (*it == value)
			return true;
	}

	return false;
}

template <class T>
int Array<T>::IndexOf(const T& value) const
{
	int index = 0;
	for (InnerType::const_iterator it = _internal.begin(); it != _internal.end(); it++)
	{
		if (*it == value)
			return index;
		index++;
	}

	return -1;
}
