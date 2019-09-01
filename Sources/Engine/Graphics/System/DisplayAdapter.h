/*=============================================================================
DisplayAdapter.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _sE_dISPLAYADAPTER_H_
#define _sE_dISPLAYADAPTER_H_

#include "Graphics/Common.h"

namespace SonataEngine
{

/** Display Adapter.
	Description of an adapter.
	@remark From D3D10
*/
class SE_GRAPHICS_EXPORT DisplayAdapter
{
public:
	/** Constructor. */
	DisplayAdapter();

	/** Constructor. */
	DisplayAdapter(uint32 index, const String& name, const String& description,
		uint32 vendorId, uint32 deviceId, uint32 subSystemId, uint32 revision,
		uint64 dedicatedVideoMemory, uint64 sharedVideoMemory);

	/** Retrieves the index of the adapter. */
	uint32 GetIndex() const { return _index; }

	/** Retrieves the name of the adapter. */
	String GetName() const { return _name; }

	/** Retrieves the description of the adapter. */
	String GetDescription() const { return _description; }

	/** Retrieves the PCI ID of the hardware vendor. */
	uint32 GetVendorId() const { return _vendorId; }

	/** Retrieves the PCI ID of the hardware device. */
	uint32 GetDeviceId() const { return _deviceId; }

	/** Retrieves the PCI ID of the sub system. */
	uint32 GetSubSystemId() const { return _subSystemId; }

	/** Retrieves the revision number of the adapter. */
	uint32 GetRevision() const { return _revision; }

	/** Retrieves the number of bytes of dedicated video memory that are not shared with the CPU. */
	uint64 GetDedicatedVideoMemory() const { return _dedicatedVideoMemory; }

	/** Retrieves the number of bytes of shared video memory. */
	uint64 GetSharedVideoMemory() const { return _sharedVideoMemory; }

protected:
	uint32 _index;
	String _name;
	String _description;
	uint32 _vendorId;
	uint32 _deviceId;
	uint32 _subSystemId;
	uint32 _revision;
	uint64 _dedicatedVideoMemory;
	uint64 _sharedVideoMemory;
};


SE_INLINE DisplayAdapter::DisplayAdapter() :
	_vendorId(0),
	_deviceId(0),
	_subSystemId(0),
	_revision(0),
	_dedicatedVideoMemory(0),
	_sharedVideoMemory(0)
{
}

SE_INLINE DisplayAdapter::DisplayAdapter(uint32 index, const String& name, const String& description,
	uint32 vendorId, uint32 deviceId, uint32 subSystemId, uint32 revision,
	uint64 dedicatedVideoMemory, uint64 sharedVideoMemory) :
	_index(index),
	_name(name),
	_description(description),
	_vendorId(vendorId),
	_deviceId(deviceId),
	_subSystemId(subSystemId),
	_revision(revision),
	_dedicatedVideoMemory(dedicatedVideoMemory),
	_sharedVideoMemory(sharedVideoMemory)
{
}

}

#endif 
