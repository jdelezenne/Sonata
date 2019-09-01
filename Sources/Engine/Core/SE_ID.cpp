/*=============================================================================
SE_ID.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SE_ID.h"
#include "Core/IO/Stream.h"

namespace SonataEngine
{

const SE_ID SE_ID::Null = SE_ID();

Stream& operator<<(Stream& stream, SE_ID& id)
{
	return stream << id.Data1 << id.Data2 << id.Data3 << id.Data4;
}

Stream& operator>>(Stream& stream, SE_ID& id)
{
	return stream >> id.Data1 >> id.Data2 >> id.Data3 >> id.Data4;
}

}
