/*=============================================================================
ResourceHandler.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ResourceHandler.h"
#include "Core/Resource/ResourceManager.h"

namespace SonataEngine
{

ResourceHandler::ResourceHandler()
{
	ResourceManager::Instance()->RegisterHandler(this);
}

ResourceHandler::~ResourceHandler()
{
	ResourceManager::Instance()->UnregisterHandler(this);
}

void ResourceHandler::Create()
{
}

void ResourceHandler::Destroy()
{
}

}
