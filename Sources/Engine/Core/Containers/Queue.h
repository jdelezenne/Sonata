/*=============================================================================
Queue.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_QUEUE_H_
#define _SE_QUEUE_H_

#include <deque>
#include "Core/Common.h"
#include "Core/Containers/IContainer.h"

namespace SonataEngine
{

/** Queue. */
template <class T>
class Queue : public IContainer<T>
{
protected:
	typedef typename std::deque<T> InnerType;
	typename InnerType _internal;

	class QueueIterator : public IIterator<T>
	{
	protected:
		const Queue<T>* _queue;
		typename InnerType::const_iterator _it;
		bool _isFirst;

	public:
		QueueIterator(const Queue<T>* queue);

		virtual QueueIterator& operator++();
		virtual T Current() const;
		virtual bool Next();
	};

	friend QueueIterator;

public:
	typedef typename QueueIterator Iterator;

	/** @name Constructors / Destructor. */
	//@{
	Queue();
	Queue(int size);
	Queue(const Queue<T>& value);
	virtual ~Queue();
	//@}

	/// Operators
	Queue<T>& operator=(const Queue<T>& value);
	T& operator[](int index);
	const T& operator[](int index) const;

	/// Returns an iterator that can iterate through the elements of the Queue.
	Iterator GetIterator() const;

	/// Removes all elements from the Queue.
	void Clear();

	/// Gets the number of elements contained in the Queue.
	int Count() const;

	/// Returns whether the Queue is empty.
	bool IsEmpty() const;

	/// Gets the element at the specified index.
	virtual T& GetItem(int index);
	virtual const T& GetItem(int index) const;

	/// Adds an object to the end of the Queue.
	void Enqueue(const T& value);

	/// Removes and returns the object at the beginning of the Queue.
	T& Dequeue();
	const T& Dequeue() const;

	/// Returns the object at the beginning of the Queue without removing it.
	T& Peek();
	const T& Peek() const;
};

#include "Queue.inl"

}

#endif
