/*=============================================================================
Tree.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

template <class T>
Tree<T>::Node::Node() :
	_index(0),
	_childCount(0),
	_parent(NULL)
{
}

template <class T>
Tree<T>::Node::Node(const T& value) :
	_data(value),
	_index(0),
	_childCount(0),
	_parent(NULL)
{
}

template <class T>
Tree<T>::Node::~Node()
{
}

template <class T>
T Tree<T>::Node::GetData() const
{
	return _data;
}

template <class T>
int Tree<T>::Node::GetIndex() const
{
	return _index;
}

template <class T>
typename Tree<T>::Node* Tree<T>::Node::GetParent() const
{
	return _parent;
}

template <class T>
typename Tree<T>::NodeList& Tree<T>::Node::GetNodes() const
{
	return _nodes;
}

template <class T>
typename Tree<T>::Node* Tree<T>::Node::FirstNode() const
{
	if (_parent->Nodes.Count() == 0)
	{
		return NULL;
	}
	else
	{
		return _parent->Nodes[0];
	}
}

template <class T>
typename Tree<T>::Node* Tree<T>::Node::LastNode() const
{
	if (_parent->Nodes.Count() == 0)
	{
		return NULL;
	}
	else
	{
		return _parent->Nodes[_parent->Nodes.Count() - 1];
	}
}

template <class T>
typename Tree<T>::Node* Tree<T>::Node::PreviousNode() const
{
	if (_parent != NULL && _index > 0)
	{
		return _parent->Nodes[_index - 1];
	}
	else
	{
		return NULL;
	}
}

template <class T>
typename Tree<T>::Node* Tree<T>::Node::NextNode() const
{
	if (_parent != NULL && _index + 1 < _parent->GetNodes().Count())
	{
		return _parent->Nodes[_index + 1];
	}
	else
	{
		return NULL;
	}
}

template <class T>
void Tree<T>::Node::EnsureCapacity()
{
	if (_nodes.IsEmpty())
	{
		//_nodes.SetCapacity(4);
		return;
	}

	if (_childCount == _nodes.Count())
	{
		//_nodes.SetCapacity(childCount * 2);
	}
}


template <class T>
Tree<T>::NodeList::NodeList() :
	_owner(NULL)
{
}

template <class T>
Tree<T>::NodeList::NodeList(Node* node) :
	_owner(node)
{
}

template <class T>
Tree<T>::NodeList::~NodeList()
{
}

template <class T>
typename Tree<T>::Node*& Tree<T>::NodeList::operator[](int index)
{
	return GetItem(index);
}

template <class T>
typename Tree<T>::Node*& Tree<T>::NodeList::operator[](int index) const
{
	return GetItem(index);
}

/*template <class T>
typename Tree<T>::InnerNodeList::Iterator Tree<T>::NodeList::GetIterator() const
{
	//TODO
	return Tree<T>::InnerNodeList::Iterator();//_nodes.GetIterator();
}*/

template <class T>
int Tree<T>::NodeList::Count() const
{
	return _nodes.Count();
}

template <class T>
bool Tree<T>::NodeList::IsEmpty() const
{
	return _nodes.IsEmpty();
}

template <class T>
void Tree<T>::NodeList::Clear()
{
	_nodes.Clear();
}

template <class T>
void Tree<T>::NodeList::Add(Node* node)
{
	_owner->EnsureCapacity();
	node->_parent = _owner;
	node->_index = _owner->_childCount++;
	_owner->_nodes[node->_index] = node;
	_nodes.AddLast(node);
}

template <class T>
void Tree<T>::NodeList::Remove(Node* node)
{
	_nodes.Remove(node);
}

template <class T>
bool Tree<T>::NodeList::Contains(Node* node) const
{
	return _nodes.Contains(node);
}


template <class T>
Tree<T>::Tree()
{
}

template <class T>
Tree<T>::~Tree()
{
}

template <class T>
Tree<T>& Tree<T>::operator=(const Tree<T>& value)
{
	if (this != &value)
	{
		//TODO
		_nodes = value.GetNodes();
	}
	return *this;
}

template <class T>
int Tree<T>::Count() const
{
	return _nodes.Count();
}

template <class T>
bool Tree<T>::IsEmpty() const
{
	return _nodes.IsEmpty();
}

template <class T>
typename Tree<T>::NodeList& Tree<T>::GetNodes()
{
	return _nodes;
}
