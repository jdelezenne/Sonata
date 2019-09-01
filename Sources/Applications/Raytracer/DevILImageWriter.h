/*=============================================================================
DevILImageWriter.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _RAYTRACER_DEVILIMAGEWRITER_H_
#define _RAYTRACER_DEVILIMAGEWRITER_H_

#include "Common.h"
#include "Image.h"

namespace Raytracer
{

class DevILImageWriter
{
public:
	DevILImageWriter();
	virtual ~DevILImageWriter();

	virtual bool SaveImage(const String& dest, RTImage* image);
};

}

#endif 
