/*=============================================================================
DDSImageReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_DDSIMAGEREADER_H_
#define _SE_DDSIMAGEREADER_H_

#include "DDS.h"
#include "Graphics/ImageReader.h"
using namespace SonataEngine;

namespace SE_DDS
{

class DDSImageReader : public ImageReader
{
public:
	DDSImageReader();
	virtual ~DDSImageReader();

	virtual Image* LoadImage(Stream& stream, ImageReaderOptions* options = NULL);

protected:
	Image* _Image;
};

}

#endif 
