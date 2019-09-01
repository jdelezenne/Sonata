/*=============================================================================
BaseArray.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

template <class T>
BaseArray<T>::BaseArrayIterator::BaseArrayIterator(const BaseArray<T>* array) :
	_array(array),
	_isFirst(true)
{
	_it = _array->_internal.begin();
}

template <class T>
typename BaseArray<T>::Iterator& BaseArray<T>::BaseArrayIterator::operator++()
{
	Next();
	return *this;
}

template <class T>
T BaseArray<T>::BaseArrayIterator::Current() const
{
	if (_isFirst)
	{
		SEthrow("InvalidOperationException");
	}

	return *_it;
}

template <class T>
bool BaseArray<T>::BaseArrayIterator::Next()
{
	if (_it == _array->_internal.end())
	{
		return false;
	}
	else if (_isFirst)
	{
		_isFirst = false;
		return true;
	}
	else
	{
		_it++;
		return (_it != _array->_internal.end());
	}
}


template <class T>
BaseArray<T>::BaseArray() :
	_internal()
{
}

template <class T>
BaseArray<T>::BaseArray(int size) :
	_internal(size)
{
}

template <class T>
BaseArray<T>::BaseArray(T* data, int size)
{
	for (int i = 0; i < size; ++i)
	{
		_internal.push_back(data[i]);
	}
}

template <class T>
BaseArray<T>::BaseArray(const T* data, int size)
{
	for (int i = 0; i < size; ++i)
	{
		_internal.push_back(data[i]);
	}
}

template <class T>
BaseArray<T>::BaseArray(const BaseArray<T>& value)
{
	Iterator it = value.GetIterator();
	while (it.Next())
	{
		Add(it.Current());
	}
}

template <class T>
BaseArray<T>::~BaseArray()
{
}

template <class T>
BaseArray<T>& BaseArray<T>::operator=(const BaseArray<T>& value)
{
	if (this != &value)
	{
		Clear();
		int count = value.Count();
		for (int i=0; i<count; i++)
		{
			_internal.push_back(value[i]);
		}
	}
	return *this;
}

template <class T>
T& BaseArray<T>::operator[](int index)
{
	return GetItem(index);
}

template <class T>
const T& BaseArray<T>::operator[](int index) const
{
	return GetItem(index);
}

template <class T>
typename BaseArray<T>::Iterator BaseArray<T>::GetIterator() const
{
	BaseArrayIterator it(this);
	return it;
}

template <class T>
int BaseArray<T>::GetCapacity()
{
	return (int) _internal.capacity();
}

template <class T>
void BaseArray<T>::SetCapacity(int value)
{
	_internal.reserve(value);
}

template <class T>
void BaseArray<T>::Resize(int value)
{
	_internal.resize(value);
}

template <class T>
int BaseArray<T>::Count() const
{
	return (int)_internal.size();
}

template <class T>
bool BaseArray<T>::IsEmpty() const
{
	return _internal.size() == 0;
}

template <class T>
void BaseArray<T>::Clear()
{
	_internal.clear();
}

template <class T>
void BaseArray<T>::Add(const T& value)
{
	_internal.push_back(value);
}

template <class T>
T& BaseArray<T>::GetItem(int index)
{
	if (index < 0 || index > (int)_internal.size())
	{
		SEthrow("OutOfRangeException");
	}

	return _internal[index];
}

template <class T>
const T& BaseArray<T>::GetItem(int index) const
{
	if (index < 0 || index > (int)_internal.size())
	{
		SEthrow("OutOfRangeException");
	}

	return _internal[index];
}

template <class T>
void BaseArray<T>::SetItem(int index, const T& value)
{
	if (index < 0 || index > (int)_internal.size())
	{
		SEthrow("OutOfRangeException");
		return;
	}

	_internal[index] = value;
}

template <class T>
void BaseArray<T>::Sort(SortFunction fnSort)
{
	std::sort(_internal.begin(), _internal.end(), fnSort);
}
