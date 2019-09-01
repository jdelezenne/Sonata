/*=============================================================================
NullMutex.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Core/Threading/Mutex.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

class MutexInternal
{
};


Mutex::Mutex() :
	_internal(NULL)
{
}

Mutex::~Mutex()
{
}

void Mutex::Enter()
{
}

void Mutex::TryEnter()
{
}

void Mutex::Exit()
{
}

}
