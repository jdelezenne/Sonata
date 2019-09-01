/*=============================================================================
List.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

template <class T>
List<T>::List() :
	BaseList<T>()
{
}

template <class T>
List<T>::List(int size)  :
	BaseList<T>(size)
{
}

template <class T>
List<T>::List(T* data, int size) :
	BaseList<T>(data, size)
{
}

template <class T>
List<T>::List(const BaseList<T>& value) :
	BaseList<T>(value)
{
}

template <class T>
List<T>::~List()
{
}

template <class T>
void List<T>::Remove(const T& value)
{
	_internal.remove(value);
}

template <class T>
bool List<T>::Contains(const T& value) const
{
	int index = 0;
	for (InnerType::const_iterator it = _internal.begin(); it != _internal.end(); it++)
	{
		if (*it == value)
		{
			return true;
		}
		index++;
	}

	return false;
}
