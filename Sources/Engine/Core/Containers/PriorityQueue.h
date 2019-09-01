/*=============================================================================
PriorityQueue.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PRIORITYQUEUE_H_
#define _SE_PRIORITYQUEUE_H_

#include <deque>
#include "Core/Common.h"
#include "Core/Containers/IContainer.h"
#include "Core/Containers/IComparer.h"
#include "Core/IComparable.h"

namespace SonataEngine
{

template <class T>
class DefaultComparer : public IComparer<T>
{
public:
	virtual int Compare(const T& x, const T& y) const
	{
		if (x < y)
			return -1;
		else if (x > y)
			return 1;
		else
			return 0;
	}
};

/** Priority Queue. */
template < class T, class CMP = DefaultComparer<T> >
class PriorityQueue : public IContainer<T>
{
protected:
	typedef typename std::deque<T> InnerType;
	typename InnerType _internal;

	class PriorityQueueIterator : public IIterator<T>
	{
	protected:
		const PriorityQueue<T,CMP>* _pqueue;
		typename InnerType::const_iterator _it;
		bool _isFirst;

	public:
		PriorityQueueIterator(const PriorityQueue<T,CMP>* pqueue);

		virtual PriorityQueueIterator& operator++();
		virtual T Current() const;
		virtual bool Next();
	};

	friend PriorityQueueIterator;

public:
	typedef typename PriorityQueueIterator Iterator;

	/** @name Constructors / Destructor. */
	//@{
	PriorityQueue();
	//PriorityQueue(IComparer<T>* comparer);
	PriorityQueue(int capacity);
	//PriorityQueue(int capacity, IComparer<T>* comparer);
	PriorityQueue(const PriorityQueue<T,CMP>& value);
	virtual ~PriorityQueue();
	//@}

	/// Operators
	PriorityQueue<T,CMP>& operator=(const PriorityQueue<T,CMP>& value);
	T& operator[](int index);
	const T& operator[](int index) const;

	/// Returns an iterator that can iterate through the elements of the PriorityQueue.
	virtual Iterator GetIterator() const;

	/// Removes all elements from the PriorityQueue.
	virtual void Clear();

	/// Gets the number of elements contained in the PriorityQueue.
	virtual int Count() const;

	/// Returns whether the PriorityQueue is empty.
	virtual bool IsEmpty() const;

	/// Gets the element at the specified index.
	virtual T& GetItem(int index);
	virtual const T& GetItem(int index) const;

	/// Adds an object to the end of the PriorityQueue.
	void Enqueue(const T& value);
	//void Enqueue(const IComparable<T>& value);

	/// Removes and returns the object at the beginning of the PriorityQueue.
	T& Dequeue();
	const T& Dequeue() const;

	/// Returns the object at the beginning of the PriorityQueue without removing it.
	T& Peek();
	const T& Peek() const;
};

#include "PriorityQueue.inl"

}

#endif
