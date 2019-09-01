/*=============================================================================
BMPImageReader.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_BMPIMAGEREADER_H_
#define _SE_BMPIMAGEREADER_H_

#include "BMP.h"

namespace SE_BMP
{

class BMPImageReader : public ImageReader
{
public:
	BMPImageReader();
	virtual ~BMPImageReader();

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
