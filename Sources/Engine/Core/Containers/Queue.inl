/*=============================================================================
Queue.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

template <class T>
Queue<T>::QueueIterator::QueueIterator(const Queue<T>* queue) :
	_queue(queue),
	_isFirst(true)
{
	_it = _queue->_internal.begin();
}

template <class T>
typename Queue<T>::QueueIterator& Queue<T>::QueueIterator::operator++()
{
	Next();
	return *this;
}

template <class T>
T Queue<T>::QueueIterator::Current() const
{
	if (_isFirst)
	{
		SEthrow("InvalidOperationException");
	}

	return *_it;
}

template <class T>
bool Queue<T>::QueueIterator::Next()
{
	if (_it == _queue->_internal.end())
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
		return (_it != _queue->_internal.end());
	}
}


template <class T>
Queue<T>::Queue() :
	_internal()
{
}

template <class T>
Queue<T>::Queue(int size) :
	_internal(size)
{
}

template <class T>
Queue<T>::Queue(const Queue<T>& value)
{
	Iterator it = value.GetIterator();
	while (it.Next())
	{
		Add(it.Current());
	}
}

template <class T>
Queue<T>::~Queue()
{
}

template <class T>
Queue<T>& Queue<T>::operator=(const Queue<T>& value)
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
T& Queue<T>::operator[](int index)
{
	return GetItem(index);
}

template <class T>
const T& Queue<T>::operator[](int index) const
{
	return GetItem(index);
}

template <class T>
typename Queue<T>::Iterator Queue<T>::GetIterator() const
{
	QueueIterator it(this);
	return it;
}

template <class T>
void Queue<T>::Clear()
{
	_internal.clear();
}

template <class T>
int Queue<T>::Count() const
{
	return (int) _internal.size();
}

template <class T>
bool Queue<T>::IsEmpty() const
{
	return (_internal.size() == 0);
}

template <class T>
T& Queue<T>::GetItem(int index)
{
	int i = 0;
	for (typename InnerType::iterator it = _internal.begin(); it != _internal.end(); it++)
	{
		if (i == index)
			return *it;

		i++;
	}

	return *_internal.end();
}

template <class T>
const T& Queue<T>::GetItem(int index) const
{
	int i = 0;
	for (typename InnerType::const_iterator it = _internal.begin(); it != _internal.end(); it++)
	{
		if (i == index)
			return *it;

		i++;
	}

	return *_internal.end();
}

template <class T>
void Queue<T>::Enqueue(const T& value)
{
	_internal.push_back(value);
}

template <class T>
T& Queue<T>::Dequeue()
{
	T& value = _internal.front();
	_internal.pop_front();
	return value;
}

template <class T>
const T& Queue<T>::Dequeue() const
{
	return _internal.pop_front();
}

template <class T>
T& Queue<T>::Peek()
{
	return _internal.front();
}

template <class T>
const T& Queue<T>::Peek() const
{
	return _internal.back();
}
