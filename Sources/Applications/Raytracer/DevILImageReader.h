/*=============================================================================
DevILImageReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _RAYTRACER_DEVILIMAGEREADER_H_
#define _RAYTRACER_DEVILIMAGEREADER_H_

#include "Common.h"
#include "Image.h"

namespace Raytracer
{

class DevILImageReader
{
public:
	DevILImageReader();
	virtual ~DevILImageReader();

	virtual RTImage* LoadImage(const String& source);
};

}

#endif 
