/*=============================================================================
IContainer.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ICONTAINER_H_
#define _SE_ICONTAINER_H_

#include "Core/Common.h"
#include "Core/Containers/IIterable.h"

namespace SonataEngine
{

/** Interface for the containers. */
template <class T>
class IContainer// : public IIterable<T>
{
public:
	/// Gets the number of elements contained in the container.
	virtual int Count() const = 0;

	/// Returns whether the container is empty.
	virtual bool IsEmpty() const = 0;
};

}

#endif 
