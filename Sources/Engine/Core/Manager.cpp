/*=============================================================================
Manager.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Manager.h"

namespace SonataEngine
{

Manager::Manager()
{
}

Manager::Manager(const String& name) :
	RefObject(),
	_name(name)
{
}

Manager::~Manager()
{
}

}
