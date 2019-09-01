/*=============================================================================
Graph.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

template <class T>
Graph<T>::Edge::Edge()
{
}

template <class T>
Graph<T>::Edge::Edge(Node* source, Node* destination) :
	_source(source),
	_destination(destination)
{
}

template <class T>
Graph<T>::Edge::Edge(Node* source, Node* destination, int weight) :
	_source(source),
	_destination(destination),
	_weight(weight)
{
}

template <class T>
bool Graph<T>::Edge::operator==(const Edge& e) const
{
	return (_source == e.GetSource() &&
		_destination == e.GetDestination() &&
		_weight == e.GetWeight());
}

template <class T>
bool Graph<T>::Edge::operator!=(const Edge& e) const
{
	return (!(*this == e));
}


template <class T>
Graph<T>::Node::Node()
{
}

template <class T>
Graph<T>::Node::Node(const T& value) :
	_data(value)
{
}

template <class T>
T Graph<T>::Node::GetData() const
{
	return _data;
}

template <class T>
const List< typename Graph<T>::Edge >& Graph<T>::Node::GetEdges() const
{
	return _edges;
}

template <class T>
const typename Graph<T>::NodeList Graph<T>::Node::GetNeighbours() const
{
	return _neighbours;
}

template <class T>
void Graph<T>::Node::AddEdge(typename const Graph<T>::Edge& edge)
{
	if (!_edges.Contains(edge))
		_edges.AddLast(edge);
}

template <class T>
void Graph<T>::Node::RemoveEdge(typename const Graph<T>::Edge& edge)
{
	_edges.Remove(edge);
}


template <class T>
Graph<T>::GraphIterator::GraphIterator(const Graph<T>* graph) :
	_graph(graph)
{
	_iterator = _graph->GetNodes().GetIterator();
}

template <class T>
typename Graph<T>::GraphIterator& Graph<T>::GraphIterator::operator++()
{
	Next();
	return *this;
}

template <class T>
T Graph<T>::GraphIterator::Current() const
{
	return _iterator.Current()->GetData();
}

template <class T>
bool Graph<T>::GraphIterator::Next()
{
	return _iterator.Next();
}


template <class T>
Graph<T>::Graph()
{
}

template <class T>
Graph<T>::~Graph()
{
}

template <class T>
Graph<T>& Graph<T>::operator=(const Graph<T>& value)
{
	if (this != &value)
	{
		//TODO
		_nodes = value.GetNodes();
	}
	return *this;
}

template <class T>
typename Graph<T>::NodeList::Iterator Graph<T>::GetIterator() const
{
	return _nodes.GetIterator();
}

template <class T>
int Graph<T>::Count() const
{
	return _nodes.Count();
}

template <class T>
bool Graph<T>::IsEmpty() const
{
	return _nodes.IsEmpty();
}

template <class T>
const typename Graph<T>::NodeList& Graph<T>::GetNodes() const
{
	return _nodes;
}

template <class T>
void Graph<T>::Add(Node* node)
{
	if (!_nodes.Contains(node))
		_nodes.AddLast(node);
}

template <class T>
void Graph<T>::Add(const T& value)
{
	_nodes.AddLast(new Graph<T>::Node(value));
}

template <class T>
void Graph<T>::Remove(Node* node)
{
	if (!_nodes.Contains(node))
		return;

	// Removes the edges directed to this node
	Graph<T>::EdgeList::Iterator it = node->GetEdges().GetIterator();
	while (it.Next())
	{
		RemoveEdges(it.Current());
	}

	_nodes.Remove(node);
}

template <class T>
void Graph<T>::Remove(const T& value)
{
	Graph<T>::Node* node = FindNode(value);
	if (node != NULL)
	{
		Remove(node);
	}
}

template <class T>
bool Graph<T>::Contains(typename Graph<T>::Node* node) const
{
	return _nodes.Contains(node);
}

template <class T>
bool Graph<T>::Contains(const T& value) const
{
	_nodes.Contains(node);
}

template <class T>
void Graph<T>::AddDirectedEdge(typename Graph<T>::Node* source, typename Graph<T>::Node* destination, int weight)
{
	Graph<T>::Edge edge(source, destination, weight);
	source->AddEdge(edge);
	destination->AddEdge(edge);
}

template <class T>
void Graph<T>::AddDirectedEdge(const T& source, const T& destination, int weight)
{
	Graph<T>::Node* sourceNode = FindNode(source);
	Graph<T>::Node* destinationNode = FindNode(destination);

	if (sourceNode == NULL)
		sourceNode = new Graph<T>::Node(source);
	if (destinationNode == NULL)
		destinationNode = new Graph<T>::Node(destination);

	Graph<T>::Edge edge(sourceNode, destinationNode, weight);
	sourceNode->AddEdge(edge);
	destinationNode->AddEdge(edge);
}

template <class T>
void Graph<T>::AddUndirectedEdge(typename Graph<T>::Node* nodeA, typename Graph<T>::Node* nodeB, int weight)
{
	Graph<T>::Edge edgeAB(nodeA, nodeB, weight);
	nodeA->AddEdge(edgeAB);
	nodeB->AddEdge(edgeAB);

	Graph<T>::Edge edgeBA(nodeB, nodeA, weight);
	nodeA->AddEdge(edgeBA);
	nodeB->AddEdge(edgeBA);
}

template <class T>
void Graph<T>::AddUndirectedEdge(const T& valueA, const T& valueB, int weight)
{
	Graph<T>::Node* nodeA = FindNode(source);
	Graph<T>::Node* nodeB = FindNode(destination);

	if (nodeA == NULL)
		nodeA = new Graph<T>::Node(source);
	if (nodeB == NULL)
		nodeB = new Graph<T>::Node(destination);

	Graph<T>::Edge edgeAB(nodeA, nodeB, weight);
	nodeA->AddEdge(edgeAB);
	nodeB->AddEdge(edgeAB);

	Graph<T>::Edge edgeBA(nodeB, nodeA, weight);
	nodeA->AddEdge(edgeBA);
	nodeB->AddEdge(edgeBA);
}

template <class T>
typename Graph<T>::Node* Graph<T>::FindNode(const T& value)
{
	Graph<T>::NodeList::Iterator it = _nodes.GetIterator();
	while (it.Next())
	{
		Graph<T>::Node* node = it.Current();
		if (node->GetData() == value)
		{
			return node;
		}
	}

	return NULL;
}

template <class T>
void Graph<T>::RemoveEdges(typename const Graph<T>::Edge& edge)
{
	Graph<T>::NodeList::Iterator it = _nodes.GetIterator();
	while (it.Next())
	{
		node->RemoveEdge(edge);
	}
}
