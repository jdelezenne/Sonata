/*=============================================================================
Stack.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_STACK_H_
#define _SE_STACK_H_

#include <deque>
#include "Core/Common.h"
#include "Core/Containers/IContainer.h"

namespace SonataEngine
{

/** Stack. */
template <class T>
class Stack : public IContainer<T>
{
protected:
	typedef typename std::deque<T> InnerType;
	typename InnerType _internal;

	class StackIterator : public IIterator<T>
	{
	protected:
		typename InnerType::const_iterator _it;
		const Stack<T>* _stack;
		bool _isFirst;

	public:
		StackIterator(const Stack<T>* stack);

		virtual StackIterator& operator++();
		virtual T Current() const;
		virtual bool Next();
	};

	friend StackIterator;

public:
	typedef typename StackIterator Iterator;

	/** @name Constructors / Destructor. */
	//@{
	Stack();
	Stack(int size);
	Stack(const Stack<T>& value);
	virtual ~Stack();
	//@}

	/// Operators
	Stack<T>& operator=(const Stack<T>& value);

	/// Returns an iterator that can iterate through the elements of the Stack.
	virtual Iterator GetIterator() const;

	/// Removes all elements from the Stack.
	virtual void Clear();

	/// Gets the number of elements contained in the Stack.
	virtual int Count() const;

	/// Returns whether the Stack is empty.
	virtual bool IsEmpty() const;

	/// Inserts an object at the top of the Stack.
	void Push(const T& value);

	/// Removes and returns the object at the top of the Stack.
	T& Pop();
	const T& Pop() const;

	/// Returns the object at the top of the Stack without removing it.
	T& Peek();
	const T& Peek() const;
};

#include "Stack.inl"

}

#endif
