/*=============================================================================
BMPImageWriter.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "BMPImageWriter.h"

namespace SE_BMP
{

BMPImageWriter::BMPImageWriter() :
	ImageWriter()
{
}

BMPImageWriter::~BMPImageWriter()
{
}

bool BMPImageWriter::SaveImage(Stream& stream, Image* image, ImageWriterOptions* options)
{
	if (image == NULL)
		return false;

	bool hasPalette = ((image->GetFormat().GetFlags() & PixelFormatFlag_Indexed) != 0);
	if (hasPalette)
	{
		if (image->GetPalette() == NULL || image->GetPalette()->GetCount() != 256)
			return false;
	}

	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;

	int width = image->GetWidth();
	int height = image->GetHeight();
	int bits = image->GetBitsPerPixel() / 8;
	int padding = (4 - (width * bits) % 4) % 4;

	bfh.bfType = BFT_BITMAP;
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	//sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) + bih.biClrUsed * sizeof(RGBQUAD);
	bfh.bfOffBits = 14 + 40 + (hasPalette ? 256 * 4 : 0);

	bih.biSize = 40;
	bih.biWidth = width;
	bih.biHeight = height;
	bih.biPlanes = 1;
	bih.biBitCount = bits * 8;
	bih.biCompression = BI_RGB;
    bih.biSizeImage = width * (height+padding) * bits;
    bih.biXPelsPerMeter = 0;
    bih.biYPelsPerMeter = 0;
    bih.biClrUsed = 0;
    bih.biClrImportant = 0;

	bfh.bfSize = bfh.bfOffBits + bih.biSizeImage;

	stream << bfh.bfType;
	stream << bfh.bfSize;
	stream << bfh.bfReserved1;
	stream << bfh.bfReserved2;
	stream << bfh.bfOffBits;

	stream << bih.biSize;
	stream << bih.biWidth;
	stream << bih.biHeight;
	stream << bih.biPlanes;
	stream << bih.biBitCount;
	stream << bih.biCompression;
	stream << bih.biSizeImage;
	stream << bih.biXPelsPerMeter;
	stream << bih.biYPelsPerMeter;
	stream << bih.biClrUsed;
	stream << bih.biClrImportant;

	if (hasPalette)
	{
		const Palette* palette = image->GetPalette();
		RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
		{
			rgb[i].rgbRed = palette->GetEntry(i).r;
			rgb[i].rgbGreen = palette->GetEntry(i).g;
			rgb[i].rgbBlue = palette->GetEntry(i).b;
			rgb[i].rgbReserved = 0;

			stream.Write((SEbyte*)rgb, sizeof(RGBQUAD) * 256);
		}

		//TODO: indices
	}
	else
	{
		int x, y;
		byte* data = image->GetData();
		int32 dataSize = image->GetDataSize();

		int32 lineSize = width * bits + padding;
		SEbyte* buffer = new SEbyte[lineSize];
		Memory::Copy(buffer, 0, lineSize);

		int32 pixel;

		for (y = height - 1; y >= 0; y--)
		{
			for (x = 0; x < width; x++)
			{
				pixel = bits*(y*width + x);

				buffer[bits*x] = data[pixel+2];
				buffer[bits*x + 1] = data[pixel+1];
				buffer[bits*x + 2] = data[pixel];
			}

			stream.Write(buffer, lineSize);
		}

		delete buffer;
	}

	return true;
}

}
