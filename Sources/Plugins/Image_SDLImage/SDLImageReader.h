/*=============================================================================
SDLImageReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SDLIMAGEIMAGEREADER_H_
#define _SE_SDLIMAGEIMAGEREADER_H_

#include "SDLImageCommon.h"

namespace SE_SDLImage
{

class SDLImageReader : public ImageReader
{
public:
	SDLImageReader();
	virtual ~SDLImageReader();

	virtual Image* LoadImage(Stream& stream, ImageReaderOptions* options = NULL);

protected:
	Image* _Image;
};

}

#endif 
