/*=============================================================================
GLHardwareBuffer.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "GLHardwareBuffer.h"
#include "GLHelper.h"

namespace SE_GL
{

GLHardwareBuffer::GLHardwareBuffer(uint32 size, HardwareBufferUsage usage) :
	HardwareBuffer(size, usage),
	_Mapped(false)
{
	glGenBuffersARB(1, &_BufferID);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, _BufferID);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, NULL, GLHelper::GetHardwareBufferUsage(usage));
}

GLHardwareBuffer::~GLHardwareBuffer()
{
	glDeleteBuffersARB(1, &_BufferID);
}

bool GLHardwareBuffer::IsMapped()
{
	return _Mapped;
}

bool GLHardwareBuffer::Map(HardwareBufferMode mode, void** data)
{
	if (_Mapped)
		return false;

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, _BufferID);

	GLenum param = GL_WRITE_ONLY;

	if (mode == HardwareBufferMode_ReadOnly)
		param = GL_READ_ONLY;
	else if (mode == HardwareBufferMode_WriteOnly)
		param = GL_WRITE_ONLY;
	else if (mode == HardwareBufferMode_ReadWrite)
		param = GL_READ_WRITE;

	*data = glMapBufferARB(GL_ARRAY_BUFFER_ARB, param);
	if (*data == NULL)
		return false;

	_Mapped = true;

	return true;
}

void GLHardwareBuffer::Unmap()
{
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, _BufferID);

	if (!glUnmapBufferARB(GL_ARRAY_BUFFER_ARB))
		return;

	_Mapped = false;
}

}
