/*=============================================================================
IList.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ILIST_H_
#define _SE_ILIST_H_

#include "Core/Common.h"
#include "Core/Containers/IContainer.h"

namespace SonataEngine
{

/** Interface for the lists. */
template <class T>
class IList : public IContainer<T>
{
public:
	/// Operators
	virtual T& operator[](int index) = 0;
	virtual const T& operator[](int index) const = 0;

	/// Removes all elements from the List.
	virtual void Clear() = 0;

	/// Adds an object to the end of the List.
	virtual void Add(const T& value) = 0;

	/// Inserts an element into the List at the specified index.
	virtual void Insert(int index, const T& value) = 0;

	/// Removes the _isFirst occurrence of a specific object from the List.
	virtual void Remove(const T& value) = 0;

	/// Removes the element at the specified index from the List.
	virtual void RemoveAt(int index) = 0;

	/// Returns whether the specified object is contained in the List.
	virtual bool Contains(const T& value) const = 0;

	/// Returns the index of the _isFirst occurrence within the List.
	virtual int IndexOf(const T& value) const = 0;

	/// Gets the element at the specified index.
	virtual T& GetItem(int index) = 0;
	virtual const T& GetItem(int index) const = 0;

	/// Sets the element at the specified index.
	virtual void SetItem(int index, const T& value) = 0;
};

}

#endif 
