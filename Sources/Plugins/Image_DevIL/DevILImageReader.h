/*=============================================================================
DevILImageReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_DEVILIMAGEREADER_H_
#define _SE_DEVILIMAGEREADER_H_

#include "DevILImageCommon.h"

namespace SE_DevIL
{

class DevILImageReader : public ImageReader
{
public:
	DevILImageReader();
	virtual ~DevILImageReader();

	virtual Image* LoadImage(Stream& stream, ImageReaderOptions* options = NULL);
};

}

#endif 
