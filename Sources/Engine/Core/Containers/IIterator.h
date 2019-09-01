/*=============================================================================
IIterator.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_IITERATOR_H_
#define _SE_IITERATOR_H_

#include "Core/Common.h"

namespace SonataEngine
{

/** Interface for the iterators. */
template <class T>
class IIterator
{
public:
	/// Operators
	virtual IIterator<T>& operator++() = 0;

	/// Gets the current element in the container.
	virtual T Current() const = 0;

	/// Advances the iterator to the next element of the container.
	virtual bool Next() = 0;
};


}

#endif 
