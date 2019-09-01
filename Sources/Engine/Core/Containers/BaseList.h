/*=============================================================================
BaseList.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_BASELIST_H_
#define _SE_BASELIST_H_

#include <list>

#include "Core/Common.h"
#include "Core/Containers/IContainer.h"

namespace SonataEngine
{

/** List for types that can't be compared. */
template <class T>
class BaseList : public IContainer<T>
{
protected:
	typedef typename std::list<T> InnerType;
	typename InnerType _internal;

public:
	class BaseListIterator;

	/** List Node. */
	class Node
	{
		friend BaseList;
		friend BaseListIterator;

	protected:
		BaseList<T>* _list;
		typename InnerType::iterator _it;

	public:
		virtual bool operator==(const Node& value) const;

		virtual bool operator!=(const Node& value) const;

		virtual Node& operator--();

		virtual Node& operator++();

		virtual BaseList<T>* List() const;

		virtual T GetValue() const;

		virtual void SetValue(const T& value);

		virtual Node Previous() const;

		virtual Node Next() const;

	protected:
		Node(BaseList<T>* list, typename BaseList<T>::InnerType::iterator it);
	};

	/** List Node. */
	class ConstNode
	{
		friend BaseList;
		friend BaseListIterator;

	protected:
		const BaseList<T>* _list;
		typename InnerType::const_iterator _it;

	public:
		virtual bool operator==(const ConstNode& value) const;

		virtual bool operator!=(const ConstNode& value) const;

		virtual ConstNode& operator--();

		virtual ConstNode& operator++();

		virtual const BaseList<T>* List() const;

		virtual T GetValue() const;

		virtual ConstNode Previous() const;

		virtual ConstNode Next() const;

	protected:
		ConstNode(const BaseList<T>* list, typename BaseList<T>::InnerType::const_iterator it);
	};

	class BaseListIterator : public IIterator<T>
	{
	protected:
		ConstNode _node;
		bool _isFirst;

	public:
		BaseListIterator(ConstNode node);

		virtual BaseListIterator& operator++();
		virtual T Current() const;
		virtual bool Next();
	};

	friend Node;
	friend BaseListIterator;

public:
	typedef typename BaseListIterator Iterator;
	typedef bool (*SortFunction) (const T& left, const T& right);

	BaseList();
	BaseList(int size);
	BaseList(T* data, int size);
	BaseList(const BaseList<T>& value);
	virtual ~BaseList();

	BaseList<T>& operator=(const BaseList<T>& value);

	virtual Iterator GetIterator() const;

	virtual int Count() const;

	virtual bool IsEmpty() const;

	virtual void Clear();

	virtual void AddFirst(const T& value);

	virtual void AddLast(const T& value);

	virtual void AddBefore(const Node& node, const T& value);

	virtual void AddAfter(const Node& node, const T& value);

	virtual void Remove(const T& value) { SEthrow("NotSupportedException"); }

	virtual void RemoveFirst();

	virtual void RemoveLast();

	virtual bool Contains(const T& value) const { SEthrow("NotSupportedException"); return false; }

	virtual Node First();
	virtual ConstNode First() const;

	virtual Node Last();
	virtual ConstNode Last() const;

	virtual void Sort(SortFunction fnSort);
};

#include "BaseList.inl"

}

#endif
