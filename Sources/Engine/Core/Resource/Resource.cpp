/*=============================================================================
Resource.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Resource.h"
#include "Core/Resource/ResourceManager.h"

namespace SonataEngine
{

Resource::Resource() :
	RefObject(),
	_handle(NULL),
	_size(0),
	_data(NULL),
	_lastAccessed(0.0)
{
}

Resource::~Resource()
{
	Unload();
}

bool Resource::Load()
{
	return true;
}

bool Resource::Unload()
{
	return true;
}

}
