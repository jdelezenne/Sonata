/*=============================================================================
HardwareBuffer.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_HARDWAREBUFFER_H_
#define _SE_HARDWAREBUFFER_H_

#include "Graphics/Common.h"

namespace SonataEngine
{

enum HardwareBufferUsage
{
	/// Write only to the buffer.
	HardwareBufferUsage_Static,
	
	/// Dynamic memory usage.
	HardwareBufferUsage_Dynamic
};

enum HardwareBufferMode
{
	/// Normal.
	HardwareBufferMode_Normal,
	
	/// Read only.
	HardwareBufferMode_ReadOnly,
	
	/// Write only.
	HardwareBufferMode_WriteOnly,
	
	/// Read and write.
	HardwareBufferMode_ReadWrite
};

/**
	@brief Base class for the hardware buffers.

	This class needs to be implemented in a render system.
*/
class SE_GRAPHICS_EXPORT HardwareBuffer : public RefObject
{
public:
	/** Destructor. */
	virtual ~HardwareBuffer();

	/** @name Properties. */
	//@{
	/**
		Gets the size of the buffer, in bytes.
		@return The size of the buffer, in bytes.
	*/
	uint32 GetSize() const { return _size; }

	/**
		Gets the usage of the buffer.
		@return The usage of the buffer.
	*/
	HardwareBufferUsage GetUsage() const { return _usage; }
	//@}

	/**
		Returns whether the buffer is mapped.
		@return true if successful; otherwise, false.
	*/
	virtual bool IsMapped() = 0;

	/**
		Get a pointer to buffer memory to modify its contents.
		@param mode Permissions.
		@param data [out] Pointer to the buffer data.
		@return true if successful; otherwise, false.
	*/
	virtual bool Map(HardwareBufferMode mode, void** data) = 0;

	/** Unmap the buffer. */
	virtual void Unmap() = 0;

protected:
	/** Protected constructor. */

	HardwareBuffer();
	/** Protected constructor. */
	HardwareBuffer(uint32 size, HardwareBufferUsage usage);

protected:
	uint32 _size;
	HardwareBufferUsage _usage;
};

typedef SmartPtr<HardwareBuffer> HardwareBufferPtr;

}

#endif
