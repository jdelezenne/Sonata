/*=============================================================================
ResourceManager.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ResourceManager.h"

namespace SonataEngine
{

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::Create()
{
	ResourceHandlerList::Iterator it = _ResourceHandlers.GetIterator();
	while (it.Next())
	{
		it.Current()->Create();
	}
}

void ResourceManager::Destroy()
{
	ResourceHandlerList::Iterator it = _ResourceHandlers.GetIterator();
	while (it.Next())
	{
		it.Current()->Destroy();
	}
}

void ResourceManager::RegisterHandler(ResourceHandler* handler)
{
	_ResourceHandlers.Add(handler);
}

void ResourceManager::UnregisterHandler(ResourceHandler* handler)
{
	_ResourceHandlers.Remove(handler);
}

ResourceHandler* ResourceManager::FindHandler(const SE_ID& type) const
{
	ResourceHandlerList::Iterator it = _ResourceHandlers.GetIterator();
	while (it.Next())
	{
		ResourceHandler* handler = it.Current();
		if (handler->CanHandle(type))
		{
			return handler;
		}
	}

	return NULL;
}

Resource* ResourceManager::Load(const String& name, const SE_ID& type, const String& path, Stream& stream)
{
	Resource* resource = ResourceManager::Get(name);
	if (resource != NULL)
	{
		if (resource->GetResourceType() != type)
			return NULL;
		else
			return resource;
	}

	ResourceHandler* handler = FindHandler(type);
	if (handler != NULL)
	{
		resource = handler->Load(name, type, path, stream);
		if (resource != NULL)
		{
			resource->_handle = GetNextHandle();
			_resourceNames.Add(resource->GetName(), resource);
			_ResourceHandles.Add(resource->GetHandle(), resource);
		}
		return resource;
	}

	return NULL;
}

bool ResourceManager::Save(Resource* resource, const String& path, Stream& stream)
{
	if (resource == NULL)
		return false;

	ResourceHandler* handler = FindHandler(resource->GetResourceType());
	if (handler != NULL)
	{
		if (!handler->Save(resource, path, stream))
			return false;

		return true;
	}

	return false;
}

bool ResourceManager::Unload(Resource* resource)
{
	if (resource == NULL)
		return false;

	ResourceHandler* handler = FindHandler(resource->GetResourceType());
	if (handler != NULL)
	{
		if (!handler->Unload(resource))
			return false;

		_resourceNames.Remove(resource->GetName());
		_ResourceHandles.Remove(resource->GetHandle());
	}

	return false;
}

Resource* ResourceManager::Get(const String& name)
{
	if (_resourceNames.Contains(name))
	{
		return _resourceNames[name];
	}
	else
	{
		return NULL;
	}
}

Resource* ResourceManager::Get(ResourceHandle handle)
{
	if (_ResourceHandles.Contains(handle))
	{
		return _ResourceHandles[handle];}	else
	{
		return NULL;
	}
}

}
