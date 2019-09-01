/*=============================================================================
Tree.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_TREE_H_
#define _SE_TREE_H_

#include "Core/Common.h"
#include "Core/Containers/IContainer.h"
#include "Core/Containers/List.h"

namespace SonataEngine
{

/** Tree. */
template <class T>
class Tree : public IContainer<T>
{
public:
	class NodeList;

	/// Represents a node of a Tree.
	class Node
	{
		friend NodeList;

	protected:
		T _data;
		int _index;
		int _childCount;
		Node* _parent;
		NodeList _nodes;

	public:
		/** @name Constructors / Destructor. */
		//@{
		Node();
		Node(const T& value);
		virtual ~Node();
		//@}

		/// Gets the data of the tree node.
		T GetData() const;

		/// Gets the position of the tree node in the tree node list.
		int GetIndex() const;

		/// Gets the parent tree node of the current tree node.
		Node* GetParent() const;

		/// Gets the list of Node assigned to the current tree node.
		NodeList& GetNodes() const;

		/// Gets the _isFirst child tree node in the tree node collection.
		Node* FirstNode() const;

		/// Gets the last child tree node.
		Node* LastNode() const;

		/// Gets the previous sibling tree node.
		Node* PreviousNode() const;

		/// Gets the next sibling tree node.
		Node* NextNode() const;

	protected:
		void EnsureCapacity();
		void Clear();
		void InsertNodeAt(int index, Node* node);
		void Remove();
	};

	/// Represents a list of Node objects.
	class NodeList : public IContainer<Node*>
	{
	public:
		typedef List<Node*> InnerNodeList;

	protected:
		Node* _owner;
		InnerNodeList _nodes;

	public:
		/** @name Constructors / Destructor. */
		//@{
		NodeList();
		NodeList(Node* node);
		virtual ~NodeList();
		//@}

		/// Operators
		Node*& operator[](int index);
		Node*& operator[](int index) const;

		//virtual typename NodeList::InnerNodeList::Iterator GetIterator() const;

		/// Gets the number of nodes contained in the NodeList.
		virtual int Count() const;

		/// Returns whether the NodeList is empty.
		virtual bool IsEmpty() const;

		/// Removes all nodes from the NodeList.
		void Clear();

		/// Adds a node to the end of the NodeList.
		void Add(Node* node);

		/// Removes the _isFirst occurrence of a specific node from the NodeList.
		void Remove(Node* node);

		/// Returns whether the specified is contained in the NodeList.
		bool Contains(Node* node) const;
	};

protected:
	NodeList _nodes;

public:
	/** @name Constructors / Destructor. */
	//@{
	Tree();
	Tree(const Tree<T>& value);
	virtual ~Tree();
	//@}

	/// Operators
	Tree<T>& operator=(const Tree<T>& value);

	/// Gets the number of elements contained in the Tree.
	virtual int Count() const;

	/// Returns whether the Tree is empty.
	virtual bool IsEmpty() const;

	/// Gets the list of Node assigned to the Tree.
	NodeList& GetNodes();
};

#include "Tree.inl"

}

#endif
