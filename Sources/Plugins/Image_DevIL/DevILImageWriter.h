/*=============================================================================
DevILImageWriter.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_DEVILIMAGEWRITER_H_
#define _SE_DEVILIMAGEWRITER_H_

#include "DevILImageCommon.h"

namespace SE_DevIL
{

class DevILImageWriter : public ImageWriter
{
public:
	DevILImageWriter();
	virtual ~DevILImageWriter();

	virtual bool SaveImage(Stream& stream, Image* image, ImageWriterOptions* options = NULL);
};

}

#endif 
