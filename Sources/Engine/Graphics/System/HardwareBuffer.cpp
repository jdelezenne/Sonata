/*=============================================================================
HardwareBuffer.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "HardwareBuffer.h"

namespace SonataEngine
{

HardwareBuffer::HardwareBuffer() :
	RefObject(),
	_size(0),
	_usage(HardwareBufferUsage_Static)
{
}

HardwareBuffer::HardwareBuffer(uint32 size, HardwareBufferUsage usage) :
	_size(size),
	_usage(usage)
{
}

HardwareBuffer::~HardwareBuffer()
{
}

}
