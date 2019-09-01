/*=============================================================================
Stack.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

template <class T>
Stack<T>::StackIterator::StackIterator(const Stack<T>* stack) :
	_stack(stack),
	_isFirst(true)
{
	_it = _stack->_internal.begin();
}

template <class T>
typename Stack<T>::StackIterator& Stack<T>::StackIterator::operator++()
{
	Next();
	return *this;
}

template <class T>
T Stack<T>::StackIterator::Current() const
{
	if (_stack == NULL || _isFirst)
	{
		SEthrow("InvalidOperationException");
	}

	return *_it;
}

template <class T>
bool Stack<T>::StackIterator::Next()
{
	if (_it == _stack->_internal.end())
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
		return (_it != _stack->_internal.end());
	}
}


template <class T>
Stack<T>::Stack() :
	_internal()
{
}

template <class T>
Stack<T>::Stack(int size) :
	_internal(size)
{
}

template <class T>
Stack<T>::Stack(const Stack<T>& value)
{
	Iterator it = value.GetIterator();
	while (it.Next())
	{
		Add(it.Current());
	}
}

template <class T>
Stack<T>::~Stack()
{
}

template <class T>
Stack<T>& Stack<T>::operator=(const Stack<T>& value)
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
typename Stack<T>::Iterator Stack<T>::GetIterator() const
{
	StackIterator it(this);
	return it;
}

template <class T>
void Stack<T>::Clear()
{
	_internal.clear();
}

template <class T>
int Stack<T>::Count() const
{
	return (int) _internal.size();
}

template <class T>
bool Stack<T>::IsEmpty() const
{
	return _internal.size() == 0;
}

template <class T>
void Stack<T>::Push(const T& value)
{
	_internal.push_back(value);
}

template <class T>
T& Stack<T>::Pop()
{
	T& value = _internal.back();
	_internal.pop_back();
	return value;
}

template <class T>
const T& Stack<T>::Pop() const
{
	return _internal.pop_back();
}

template <class T>
T& Stack<T>::Peek()
{
	return _internal.back();
}

template <class T>
const T& Stack<T>::Peek() const
{
	return _internal.back();
}
