/*=============================================================================
List.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_LIST_H_
#define _SE_LIST_H_

#include <list>
#include "Core/Common.h"
#include "Core/Containers/BaseList.h"

namespace SonataEngine
{

/** List for types that can be compared. */
template <class T>
class List : public BaseList<T>
{
public:
	List();
	List(int size);
	List(T* data, int size);
	List(const BaseList<T>& value);
	virtual ~List();

	virtual void Remove(const T& value);

	virtual bool Contains(const T& value) const;
};

#include "List.inl"

}

#endif
