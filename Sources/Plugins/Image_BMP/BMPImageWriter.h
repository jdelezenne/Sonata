/*=============================================================================
BMPImageWriter.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_BMPIMAGEWRITER_H_
#define _SE_BMPIMAGEWRITER_H_

#include "BMP.h"

namespace SE_BMP
{

class BMPImageWriter : public ImageWriter
{
public:
	BMPImageWriter();
	virtual ~BMPImageWriter();

	virtual bool SaveImage(Stream& stream, Image* image, ImageWriterOptions* options = NULL);

protected:

protected:
	Image* _Image;
};

}

#endif 
