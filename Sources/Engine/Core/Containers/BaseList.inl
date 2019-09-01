/*=============================================================================
BaseList.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

template <class T>
BaseList<T>::Node::Node(BaseList<T>* list, typename BaseList<T>::InnerType::iterator it) :
	_list(list),
	_it(it)
{
}

template <class T>
bool BaseList<T>::Node::operator==(const typename BaseList<T>::Node& value) const
{
	return (_it == value._it);
}

template <class T>
bool BaseList<T>::Node::operator!=(const typename BaseList<T>::Node& value) const
{
	return (_it != value._it);
}

template <class T>
typename BaseList<T>::Node& BaseList<T>::Node::operator--()
{
	Previous();
	return *this;
}

template <class T>
typename BaseList<T>::Node& BaseList<T>::Node::operator++()
{
	Next();
	return *this;
}

template <class T>
BaseList<T>* BaseList<T>::Node::List() const
{
	return _list;
}

template <class T>
T BaseList<T>::Node::GetValue() const
{
	return *_it;
}

template <class T>
void BaseList<T>::Node::SetValue(const T& value)
{
	*_it = value;
}

template <class T>
typename BaseList<T>::Node BaseList<T>::Node::Previous() const
{
	BaseList<T>::InnerType::iterator it = _it;
	return BaseList<T>::Node(_list, --it);
}

template <class T>
typename BaseList<T>::Node BaseList<T>::Node::Next() const
{
	BaseList<T>::InnerType::iterator it = _it;
	return BaseList<T>::Node(_list, ++it);
}


template <class T>
BaseList<T>::ConstNode::ConstNode(const BaseList<T>* list, typename BaseList<T>::InnerType::const_iterator it) :
	_list(list),
	_it(it)
{
}

template <class T>
bool BaseList<T>::ConstNode::operator==(const typename BaseList<T>::ConstNode& value) const
{
	return (_it == value._it);
}

template <class T>
bool BaseList<T>::ConstNode::operator!=(const typename BaseList<T>::ConstNode& value) const
{
	return (_it != value._it);
}

template <class T>
typename BaseList<T>::ConstNode& BaseList<T>::ConstNode::operator--()
{
	Previous();
	return *this;
}

template <class T>
typename BaseList<T>::ConstNode& BaseList<T>::ConstNode::operator++()
{
	Next();
	return *this;
}

template <class T>
const BaseList<T>* BaseList<T>::ConstNode::List() const
{
	return _list;
}

template <class T>
T BaseList<T>::ConstNode::GetValue() const
{
	return *_it;
}

template <class T>
typename BaseList<T>::ConstNode BaseList<T>::ConstNode::Previous() const
{
	BaseList<T>::InnerType::const_iterator it = _it;
	return BaseList<T>::ConstNode(_list, --it);
}

template <class T>
typename BaseList<T>::ConstNode BaseList<T>::ConstNode::Next() const
{
	BaseList<T>::InnerType::const_iterator it = _it;
	return BaseList<T>::ConstNode(_list, ++it);
}


template <class T>
BaseList<T>::BaseListIterator::BaseListIterator(ConstNode node) :
	_node(node),
	_isFirst(true)
{
}

template <class T>
typename BaseList<T>::BaseListIterator& BaseList<T>::BaseListIterator::operator++()
{
	Next();
	return *this;
}

template <class T>
T BaseList<T>::BaseListIterator::Current() const
{
	if (_isFirst)
	{
		SEthrow("InvalidOperationException");
	}

	return _node.GetValue();
}

template <class T>
bool BaseList<T>::BaseListIterator::Next()
{
	if (_node._it == _node._list->_internal.end())
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
		_node = _node.Next();
		return (_node._it != _node._list->_internal.end());
	}
}


template <class T>
BaseList<T>::BaseList() :
	_internal()
{
}

template <class T>
BaseList<T>::BaseList(int size) :
	_internal(size)
{
}

template <class T>
BaseList<T>::BaseList(T* data, int size)
{
	for (int i = 0; i < size; ++i)
	{
		_internal.push_back(data[i]);
	}
}

template <class T>
BaseList<T>::BaseList(const BaseList<T>& value)
{
	BaseList<T>::Iterator it = value.GetIterator();
	while (it.Next())
	{
		AddLast(it.Current());
	}
}

template <class T>
BaseList<T>::~BaseList()
{
}

template <class T>
BaseList<T>& BaseList<T>::operator=(const BaseList<T>& value)
{
	if (this != &value)
	{
		Clear();
		BaseList<T>::Iterator it = value.GetIterator();
		while (it.Next())
		{
			AddLast(it.Current());
		}
	}
	return *this;
}

template <class T>
typename BaseList<T>::Iterator BaseList<T>::GetIterator() const
{
	BaseListIterator it(First());
	return it;
}

template <class T>
int BaseList<T>::Count() const
{
	return (int)_internal.size();
}

template <class T>
bool BaseList<T>::IsEmpty() const
{
	return (_internal.size() == 0);
}

template <class T>
void BaseList<T>::Clear()
{
	_internal.clear();
}

template <class T>
void BaseList<T>::AddFirst(const T& value)
{
	_internal.push_front(value);
}

template <class T>
void BaseList<T>::AddLast(const T& value)
{
	_internal.push_back(value);
}

template <class T>
void BaseList<T>::AddBefore(const Node& node, const T& value)
{
	InnerType::iterator it = node._it;
	it--;
	_internal.insert(it, value);
}

template <class T>
void BaseList<T>::AddAfter(const Node& node, const T& value)
{
	_internal.insert(node._it, value);
}

template <class T>
void BaseList<T>::RemoveFirst()
{
	_internal.pop_front();
}

template <class T>
void BaseList<T>::RemoveLast()
{
	_internal.pop_back();
}

template <class T>
typename BaseList<T>::Node BaseList<T>::First()
{
	return Node(this, _internal.begin());
}

template <class T>
typename BaseList<T>::ConstNode BaseList<T>::First() const
{
	return ConstNode(this, _internal.begin());
}

template <class T>
typename BaseList<T>::Node BaseList<T>::Last()
{
	return Node(this, _internal.end());
}

template <class T>
typename BaseList<T>::ConstNode BaseList<T>::Last() const
{
	return ConstNode(this, _internal.end());
}

template <class T>
void BaseList<T>::Sort(SortFunction fnSort)
{
	_internal.sort(fnSort);
}
