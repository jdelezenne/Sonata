/*=============================================================================
Exception.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Exception.h"

namespace SonataEngine
{

Exception::Exception()
{
}

Exception::Exception(const String& message) :
	_message(message)
{
}

Exception::Exception(const Exception& exception)
{
	_message = exception.GetMessage();
}

Exception::~Exception()
{
}

}
