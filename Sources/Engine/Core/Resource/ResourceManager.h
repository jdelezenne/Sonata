/*=============================================================================
ResourceManager.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_RESOURCEMANAGER_H_
#define _SE_RESOURCEMANAGER_H_

#include "Core/Common.h"
#include "Core/Singleton.h"
#include "Core/String.h"
#include "Core/SE_ID.h"
#include "Core/Containers/Dictionary.h"
#include "Core/Resource/Resource.h"
#include "Core/Resource/ResourceHandler.h"

namespace SonataEngine
{

/** @brief Resource manager.
	This class is responsible for managing the external resources.
	It contains a list of resource handlers that are implemented
	for each type of resource.
*/
class SE_CORE_EXPORT ResourceManager : public Singleton<ResourceManager>
{
public:
	typedef Array<ResourceHandler*> ResourceHandlerList;
	typedef Dictionary<String, Resource*> ResourceNameList;
	typedef Dictionary<ResourceHandle, Resource*> ResourceHandleList;

protected:
	ResourceHandlerList _ResourceHandlers;

	ResourceNameList _resourceNames;
	ResourceHandleList _ResourceHandles;

	ResourceHandle _NextHandle;
	uint32 _MemorySize;
	uint32 _MemoryUsage;

public:
	ResourceManager();
	virtual ~ResourceManager();

	/** Returns the total available memory size. */
	uint32 GetMemorySize() const { return _MemorySize; }

	/** Returns the total memory usage size. */
	uint32 GetMemoryUsage() const { return _MemoryUsage; }

	void RegisterHandler(ResourceHandler* handler);
	void UnregisterHandler(ResourceHandler* handler);

	ResourceHandler* FindHandler(const SE_ID& type) const;

	void Create();

	void Destroy();

	Resource* Load(const String& name, const SE_ID& type, const String& path, Stream& stream);

	bool Save(Resource* resource, const String& path, Stream& stream);

	bool Unload(Resource* resource);

	Resource* Get(const String& name);

	Resource* Get(ResourceHandle handle);







	ResourceHandle AddResource(Resource* value);

	void UnloadAll()
	{
	}

	void ReloadAll()
	{
	}

	void Unload(const String& name)
	{
		if (_resourceNames.Contains(name))
		{
			Resource* resource = _resourceNames[name];
			resource->Unload();

			ResourceHandle handle = resource->GetHandle();
			_resourceNames.Remove(name);
			_ResourceHandles.Remove(handle);
		}
	}

	void Unload(ResourceHandle handle)
	{
		if (_ResourceHandles.Contains(handle))
		{
			Resource* resource = _ResourceHandles[handle];
			resource->Unload();

			String name = resource->GetName();
			_ResourceHandles.Remove(handle);
			_resourceNames.Remove(name);
		}
	}

	void Reload(const String& name)
	{
	}

	void Reload(ResourceHandle handle)
	{
	}

	Array<String>::Iterator GetResourceNameIterator() const
	{
		return _resourceNames.Keys().GetIterator();
	}

	Array<ResourceHandle>::Iterator GetResourceHandleIterator() const
	{
		return _ResourceHandles.Keys().GetIterator();
	}

	ResourceHandle GetResourceHandle(const String& name) const
	{
		if (_resourceNames.Contains(name))
		{
			Resource* resource = _resourceNames[name];
			return resource->GetHandle();
		}

		return ResourceHandleInvalid;
	}

	const String GetResourceName(ResourceHandle handle) const
	{
		if (_ResourceHandles.Contains(handle))
		{
			Resource* resource = _ResourceHandles[handle];
			return resource->GetName();
		}

		return String::Empty;
	}

	void SetResourceName(ResourceHandle handle, const String& name)
	{
		if (_ResourceHandles.Contains(handle) && !_resourceNames.Contains(name))
		{
			Resource* resource = _ResourceHandles[handle];
			_resourceNames.Remove(resource->GetName());
			resource->SetName(name);
			_resourceNames.Add(resource->GetName(), resource);
		}
	}

protected:
	ResourceHandle GetNextHandle()
	{
		return _NextHandle++;
	}
};

}

#endif
