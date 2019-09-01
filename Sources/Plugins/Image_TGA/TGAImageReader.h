/*=============================================================================
TGAImageReader.h
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#ifndef _SE_TGAIMAGEREADER_H_
#define _SE_TGAIMAGEREADER_H_

#include "Image_TGA.h"
#include "TGA.h"
#include "Core/IO/Stream.h"
#include "Graphics/Image.h"
using namespace SonataEngine;

namespace SonataEngine
{

struct ImageReaderOptions
{
};

class ImageReader
{
public:
	/** Destructor. */
	virtual ~ImageReader() {}

	virtual Image* LoadImage(Stream& stream, ImageReaderOptions* options = NULL) = 0;

protected:
	/** Constructor. */
	ImageReader() {}
};

}

namespace SE_ImagePlugin_TGA
{

class TGAImageReader : public ImageReader
{
public:
	TGAImageReader();
	virtual ~TGAImageReader();

	virtual Image* LoadImage(Stream& stream, ImageReaderOptions* options = NULL);

protected:
	bool ReadRGB(Stream& stream);
	bool ReadRLE8(Stream& stream);
	bool ReadRLE4(Stream& stream);

protected:
	Image* _Image;

	BITMAPFILEHEADER bfh;
	BITMAPCOREHEADER bch;
	BITMAPINFOHEADER bih;
};

}

#endif 
