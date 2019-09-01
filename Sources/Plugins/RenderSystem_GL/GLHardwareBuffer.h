/*=============================================================================
GLHardwareBuffer.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_GLHARDWAREBUFFER_H_
#define _SE_GLHARDWAREBUFFER_H_

#include "GLRenderSystem.h"

namespace SE_GL
{

/** OpenGL hardware buffer. */
class GLHardwareBuffer : public HardwareBuffer
{
public:
	GLHardwareBuffer(uint32 size, HardwareBufferUsage usage);
	virtual ~GLHardwareBuffer();

	virtual bool IsMapped();
	virtual bool Map(HardwareBufferMode mode, void** data);
	virtual void Unmap();

	GLuint GetBufferID() const { return _BufferID; }

protected:
	bool _Mapped;
	GLuint _BufferID;
};

}

#endif
