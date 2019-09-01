/*=============================================================================
SoftwareHardwareBuffer.cpp
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#include "SoftwareHardwareBuffer.h"

SoftwareHardwareBuffer::SoftwareHardwareBuffer() :
	_Buffer(NULL)
{
}

SoftwareHardwareBuffer::SoftwareHardwareBuffer(OGPRBuffer* buffer) :
	_Buffer(buffer)
{
}

SoftwareHardwareBuffer::~SoftwareHardwareBuffer()
{
	SAFE_RELEASE(_Buffer);
}

bool SoftwareHardwareBuffer::IsLocked()
{
	if (_Buffer == NULL)
		return false;
	else
		return _Buffer->IsLocked();
}

bool SoftwareHardwareBuffer::Lock(HardwareBufferLock mode, void** data)
{
	if (_Buffer == NULL)
	{
		return false;
	}
	else
	{
		GPRLOCK gprMode = GPRLOCK_READONLY;
		switch (mode)
		{
		case HBL_ReadOnly: gprMode = GPRLOCK_READONLY; break;
		case HBL_WriteOnly: gprMode = GPRLOCK_WRITEONLY; break;
		case HBL_ReadWrite: gprMode = GPRLOCK_READWRITE; break;
		}

		return _Buffer->Lock(gprMode, data) == GPRRESULT_OK;
	}
}

bool SoftwareHardwareBuffer::Unlock(void** data)
{
	if (_Buffer == NULL)
		return false;
	else
		return _Buffer->Unlock(data) == GPRRESULT_OK;
}
