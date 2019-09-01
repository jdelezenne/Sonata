/*=============================================================================
Array.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ARRAY_H_
#define _SE_ARRAY_H_

#include "Core/Common.h"
#include "Core/Containers/BaseArray.h"

namespace SonataEngine
{

/** Array for types that can be compared. */
template <class T>
class Array : public BaseArray<T>
{
public:
	Array();
	Array(int size);
	Array(T* data, int size);
	Array(const T* data, int size);
	Array(const Array<T>& value);
	virtual ~Array();

	virtual bool Contains(const T& value) const;

	virtual int IndexOf(const T& value) const;
};

#include "Array.inl"

}

#endif
