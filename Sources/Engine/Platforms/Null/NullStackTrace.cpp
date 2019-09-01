/*=============================================================================
NullStackTrace.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Core/Debug/StackTrace.h"

namespace SonataEngine
{

typedef StackTrace NullStackTrace;

void NullStackTrace::CaptureCallStack()
{
}

}
