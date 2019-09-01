/*=============================================================================
PriorityQueue.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

template <class T, class CMP>
PriorityQueue<T,CMP>::PriorityQueueIterator::PriorityQueueIterator(const PriorityQueue<T,CMP>* pqueue) :
	_pqueue(pqueue),
	_isFirst(true)
{
	_it = _pqueue->_internal.begin();
}

template <class T, class CMP>
typename PriorityQueue<T,CMP>::PriorityQueueIterator& PriorityQueue<T,CMP>::PriorityQueueIterator::operator++()
{
	Next();
	return *this;
}

template <class T, class CMP>
T PriorityQueue<T,CMP>::PriorityQueueIterator::Current() const
{
	if (_pqueue == NULL || _isFirst)
	{
		SEthrow("InvalidOperationException");
	}

	return *_it;
}

template <class T, class CMP>
bool PriorityQueue<T,CMP>::PriorityQueueIterator::Next()
{
	if (_it == _pqueue->_internal.end())
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
		return (_it != _pqueue->_internal._end());
	}
}


template <class T, class CMP>
PriorityQueue<T,CMP>::PriorityQueue() :
	_internal()/*,
	_Comparer(NULL)*/
{
}

/*template <class T, class CMP>
PriorityQueue<T,CMP>::PriorityQueue(IComparer<T>* comparer) :
	_internal(),
	_Comparer(comparer)
{
}*/

template <class T, class CMP>
PriorityQueue<T,CMP>::PriorityQueue(int capacity) :
	_internal(capacity)/*,
	_Comparer(NULL)*/
{
}

/*template <class T, class CMP>
PriorityQueue<T,CMP>::PriorityQueue(int capacity, IComparer<T>* comparer) :
	_internal(capacity),
	_Comparer(comparer)
{
}*/

template <class T, class CMP>
PriorityQueue<T,CMP>::PriorityQueue(const PriorityQueue<T,CMP>& value)
{
	Iterator it = value.GetIterator();
	while (it.Next())
	{
		Add(it.Current());
	}
}

template <class T, class CMP>
PriorityQueue<T,CMP>::~PriorityQueue()
{
}

template <class T, class CMP>
PriorityQueue<T,CMP>& PriorityQueue<T,CMP>::operator=(const PriorityQueue<T,CMP>& value)
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

template <class T, class CMP>
T& PriorityQueue<T,CMP>::operator[](int index)
{
	return GetItem(index);
}

template <class T, class CMP>
const T& PriorityQueue<T,CMP>::operator[](int index) const
{
	return GetItem(index);
}

template <class T, class CMP>
typename PriorityQueue<T,CMP>::Iterator PriorityQueue<T,CMP>::GetIterator() const
{
	PriorityQueueIterator it(this);
	return it;
}

template <class T, class CMP>
void PriorityQueue<T,CMP>::Clear()
{
	_internal.clear();
}

template <class T, class CMP>
int PriorityQueue<T,CMP>::Count() const
{
	return (int) _internal.size();
}

template <class T, class CMP>
bool PriorityQueue<T,CMP>::IsEmpty() const
{
	return (_internal.size() == 0);
}

template <class T, class CMP>
T& PriorityQueue<T,CMP>::GetItem(int index)
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

template <class T, class CMP>
const T& PriorityQueue<T,CMP>::GetItem(int index) const
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

template <class T, class CMP>
void PriorityQueue<T,CMP>::Enqueue(const T& value)
{
	CMP comparer;
	InnerType::iterator end = _internal.end();
	for (InnerType::iterator it = _internal.begin(); it != end; ++it)
	{
		if (comparer.Compare(value, *it) == -1)
		{
			_internal.insert(it, value);
			return;
		}
		/*if (_Comparer != NULL)
		{
			if (_Comparer->Compare(value, *it) == IComparer<T>::Less)
			{
				_internal.insert(it, value);
				return;
			}
		}
		else
		{
			if (value < *it)
			{
				_internal.insert(it, value);
				return;
			}
		}*/
	}

	_internal.push_back(value);
}

/*template <class T, class CMP>
void PriorityQueue<T,CMP>::Enqueue(const IComparable<T>&)
{
	InnerType::iterator end = _internal.end();
	for (InnerType::iterator it = _internal.begin(); it != end; ++it)
	{
		if (_Comparer != NULL)
		{
			if (_Comparer->Compare(value, *it) == IComparer<T>::Less)
			{
				_internal.insert(it, value);
				return;
			}
		}
		else
		{
			if (value.CompareTo(*it) == IComparer<T>::Less)
			{
				_internal.insert(it, value);
				return;
			}
		}
	}

	_internal.push_back(value);
}*/

template <class T, class CMP>
T& PriorityQueue<T,CMP>::Dequeue()
{
	T& value = _internal.front();
	_internal.pop_front();
	return value;
}

template <class T, class CMP>
const T& PriorityQueue<T,CMP>::Dequeue() const
{
	return _internal.pop_front();
}

template <class T, class CMP>
T& PriorityQueue<T,CMP>::Peek()
{
	return _internal.front();
}

template <class T, class CMP>
const T& PriorityQueue<T,CMP>::Peek() const
{
	return _internal.back();
}
