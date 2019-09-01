/*=============================================================================
IIterable.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_IITERABLE_H_
#define _SE_IITERABLE_H_

#include "Core/Common.h"
#include "Core/Containers/IIterator.h"

namespace SonataEngine
{

/** Interface for the enumerable objects. */
template <class T>
class IIterable
{
public:
	/// Returns an iterator that can iterate through the elements of the container.
	//virtual IIterator<T> GetIterator() const = 0;
};

}

#endif 
