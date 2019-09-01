/*=============================================================================
IRefCounter.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_IREFCOUNTER_H_
#define _SE_IREFCOUNTER_H_

#include "Core/Common.h"

namespace SonataEngine
{

/** Interface for reference counters. */
class IRefCounter
{
public:
	virtual uint32 GetRefCounter() const = 0;
	virtual bool IsFree() const = 0;
	virtual uint32 AddRef() = 0;
	virtual uint32 Release() = 0;
};

}

#endif
