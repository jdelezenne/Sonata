/*=============================================================================
Resource.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_RESOURCE_H_
#define _SE_RESOURCE_H_

#include "Core/Object.h"
#include "Core/SE_ID.h"

namespace SonataEngine
{

class ResourceManager;

typedef uint32 ResourceHandle;
static const ResourceHandle ResourceHandleInvalid = 0xffffffff;

/**
	@brief Resource.

	Represents an external resource.
	A resource needs to be loaded before being used, and then unloaded when
	necessary.
*/
class SE_CORE_EXPORT Resource : public RefObject
{
	friend ResourceManager;

protected:
	SE_ID _resourceType;
	String _name;
	ResourceHandle _handle;
	uint32 _size;
	void* _data;
	String _sourceName;
	real64 _lastAccessed;

public:
	/** Constructor. */
	Resource();

	/** Destructor. */
	virtual ~Resource();

	virtual SE_ID GetResourceType() const { return _resourceType; }
	void SetResourceType(const SE_ID& value) { _resourceType = value; }

	String GetName() const { return _name; }
	void SetName(const String& value) { _name = value; }

	ResourceHandle GetHandle() const { return _handle; }

	uint32 GetSize() const { return _size; }
	void SetSize(uint32 value) { _size = value; }

	void* GetData() const { return _data; }
	void SetData(void* value) { _data = value; }

	String GetSourceName() const { return _sourceName; }
	void SetSourceName(String value) { _sourceName = value; }

	real64 GetLastAccessed() const { return _lastAccessed; }

	virtual bool Load();

	virtual bool Unload();

	bool Reload();

	void Touch();
};

}

#endif 
