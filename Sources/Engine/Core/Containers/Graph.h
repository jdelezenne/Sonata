/*=============================================================================
Graph.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_GRAPH_H_
#define _SE_GRAPH_H_

#include "Core/Common.h"
#include "Core/Containers/IContainer.h"
#include "Core/Containers/List.h"

namespace SonataEngine
{

/** Graph.
	@remark Could be moved to an interface and implemented as either
		-an adjacency list
		-an adjacency matrix
*/
template <class T>
class Graph : public IContainer<T>
{
public:
	class Node;

	typedef List<Node*> NodeList;

protected:
	NodeList _nodes;

	/** Edge.
		An Edge (or Arc) is a transition between two Nodes (or Vertices) in a Graph.
	*/
	class Edge
	{
	public:
		/** @name Constructors / Destructor. */
		//@{
		Edge();
		Edge(Node* source, Node* destination);
		Edge(Node* source, Node* destination, int weight);
		//@}

		Node* GetSource() const { return _source; }
		void SetSource(Node* value) { _source = value; }

		Node* GetDestination() const { return _destination; }
		void SetDestination(Node* value) { _destination = value; }

		int GetWeight() const { return _weight; }
		void SetWeight(int value) { _weight = value; }

		bool operator==(const Edge& e) const;
		bool operator!=(const Edge& e) const;

	protected:
		Node* _source;
		Node* _destination;
		int _weight;
	};

	/** Represents a node of a Graph. */
	class Node
	{
		friend Graph<T>;

	protected:
		List<Edge> _edges;
		T _data;

	public:
		/** @name Constructors / Destructor. */
		//@{
		Node();
		Node(const T& value);
		//@}

		/// Gets the data of the graph node.
		T GetData() const;

		/// Gets the list of Edge assigned to the current graph node.
		const List<Edge>& GetEdges() const;

		/// Gets the list of Node assigned to the current graph node.
		const NodeList GetNeighbours() const;

	private:
		void AddEdge(const Edge& edge);
		void RemoveEdge(const Edge& edge);
	};

private:
	class GraphIterator : public IIterator<T>
	{
	public:
		GraphIterator(const Graph<T>* graph);

		virtual GraphIterator& operator++();
		virtual T Current() const;
		virtual bool Next();

	protected:
		const Graph<T>* _graph;
		typename NodeList::Iterator _iterator;
	};

public:
	/** @name Constructors / Destructor. */
	//@{
	Graph();
	Graph(const Graph<T>& value);
	virtual ~Graph();
	//@}

	/// Operators
	Graph<T>& operator=(const Graph<T>& value);

	/// Returns an iterator that can iterate through the Node objects of the Graph.
	virtual typename Graph<T>::NodeList::Iterator GetIterator() const;

	/// Gets the number of nodes contained in the Graph.
	virtual int Count() const;

	/// Returns whether the Graph is empty.
	virtual bool IsEmpty() const;

	/// Gets the list of Node assigned to the Graph.
	const NodeList& GetNodes() const;

	/// Adds a node to the Graph.
	void Add(Node* node);
	void Add(const T& value);

	/// Removes a node from the Graph.
	void Remove(Node* node);
	void Remove(const T& value);

	/// Returns whether the specified node is contained in the Graph.
	bool Contains(Node* node) const;
	bool Contains(const T& value) const;

	void AddDirectedEdge(Node* source, Node* destination, int weight = 0);
	void AddDirectedEdge(const T& source, const T& destination, int weight = 0);

	void AddUndirectedEdge(Node* nodeA, Node* nodeB, int weight = 0);
	void AddUndirectedEdge(const T& valueA, const T& valueB, int weight = 0);

protected:
	Node* FindNode(const T& value);
	void RemoveEdges(const Edge& edge);
};

#include "Graph.inl"

}

#endif
