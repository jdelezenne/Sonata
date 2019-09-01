/*=============================================================================
SoftwareHardwareBuffer.h
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#ifndef _SOFTWAREHARDWAREBUFFER_H_
#define _SOFTWAREHARDWAREBUFFER_H_

#include "OGPR/OGPR.h"
#include "Graphic/HardwareBuffer.h"
using namespace Teesside;

/**
 * Software hardware buffer.
 */
class SoftwareHardwareBuffer : public HardwareBuffer
{
public:
	/** Constructors / Destructor. */
	//@{
	SoftwareHardwareBuffer();
	SoftwareHardwareBuffer(OGPRBuffer* buffer);
	virtual ~SoftwareHardwareBuffer();
	//@}

	virtual bool IsLocked();
	virtual bool Lock(HardwareBufferLock mode, void** data);
	virtual bool Unlock(void** data);

	OGPRBuffer* GetBuffer() const { return _Buffer; }
	void SetBuffer(OGPRBuffer* buffer) { _Buffer = buffer; }

protected:
	OGPRBuffer* _Buffer;
};

#endif
