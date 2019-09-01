/*=============================================================================
TGAImageReader.cpp
Project: Sonata Engine
Copyright �by7
Julien Delezenne
=============================================================================*/

#include "TGAImageReader.h"

namespace SE_ImagePlugin_TGA
{

void GetShiftFromMask(uint32 Mask, uint32* ShiftLeft, uint32* ShiftRight)
{
	uint32 Temp, i;

	if (Mask == 0)
	{
		*ShiftLeft = *ShiftRight = 0;
		return;
	}

	Temp = Mask;
	for (i = 0; i < 32; i++, Temp >>= 1)
	{
		if (Temp & 1)
			break;
	}
	*ShiftRight = i;

	// Temp is preserved, so use it again:
	for (i = 0; i < 8; i++, Temp >>= 1)
	{
		if (!(Temp & 1))
			break;
	}
	*ShiftLeft = 8 - i;

	return;
}

TGAImageReader::TGAImageReader() :
	ImageReader()
{
}

TGAImageReader::~TGAImageReader()
{
}

Image* TGAImageReader::LoadImage(Stream& stream, ImageReaderOptions* options)
{
	// file header
	stream >> bfh.bfType;

	if (bfh.bfType != BFT_BITMAP)
	{
		return NULL;
	}

	stream >> bfh.bfSize;
	stream >> bfh.bfReserved1;
	stream >> bfh.bfReserved2;
	stream >> bfh.bfOffBits;

	// core info
	DWORD dwSize;
	stream >> dwSize;

	if (dwSize == 12) // v2
	{
		bch.bcSize = dwSize;
		stream >> bch.bcWidth;
		stream >> bch.bcHeight;
		stream >> bch.bcPlanes;
		stream >> bch.bcBitCount;

		if (bih.biPlanes != 1)
		{
			return false;
		}

		// Only handle 8 and 24 bits
		if (bch.bcBitCount != 8 && bch.bcBitCount != 24)
		{
			return false;
		}

		PixelFormat format;
		if (bch.bcBitCount == 8)
			format = PixelFormat::Indexed;

		if (bch.bcBitCount == 24)
			format = PixelFormat::R8G8B8;

		_Image = new Image(format, bch.bcWidth, bch.bcHeight);
	}
	else if (dwSize == 40) // v3
	{
		bih.biSize = dwSize;
		stream >> bih.biWidth;
		stream >> bih.biHeight;
		stream >> bih.biPlanes;
		stream >> bih.biBitCount;
        stream >> bih.biCompression;
        stream >> bih.biSizeImage;
        stream >> bih.biXPelsPerMeter;
        stream >> bih.biYPelsPerMeter;
        stream >> bih.biClrUsed;
        stream >> bih.biClrImportant;

		if (bih.biPlanes != 1)
		{
			return false;
		}

		// Only handle 8, 24 and 32 bits
		if (bih.biBitCount != 8 && bih.biBitCount != 24 && bih.biBitCount != 32)
		{
			return false;
		}

		PixelFormat format;
		if (bih.biBitCount == 8)
			format = PixelFormat::Indexed;

		if (bih.biBitCount == 24)
			format = PixelFormat::R8G8B8;

		if (bih.biBitCount == 32)
			format = PixelFormat::R8G8B8A8;

		_Image = new Image(format, bch.bcWidth, bch.bcHeight);

		bool res;

		switch (bih.biCompression)
		{
			case BI_RGB:
			case BI_BITFIELDS:
				res = ReadRGB(stream);
				break;

			case BI_RLE8:
				res = ReadRLE8(stream);
				break;

			case BI_RLE4:
				res = ReadRLE4(stream);
				break;

			default:
				return false;
		}

	}
	else
	{
		return false;
	}

	return _Image;
}

bool TGAImageReader::ReadRGB(Stream& stream)
{
	int i;

	switch (bih.biBitCount)
	{
	case 4:
	case 8:
		_Image->_Format = PixelFormat(PixelFormatFlag_Indexed);
		break;

	case 16:
	case 24:
	case 32:
		_Image->_Format = PixelFormat::R5G5B5;
		break;

	default:
		return false;
	}

	// read the palette
	bih.biClrUsed = 1 << bih.biBitCount;
	_Image->_Palette = new Palette(bih.biClrUsed);
	if (bih.biSize == 12)
	{
		for (i = 0; i < (int)bih.biClrUsed; ++i)
		{
			RGBTRIPLE rgb;
			stream >> rgb.rgbtBlue;
			stream >> rgb.rgbtGreen;
			stream >> rgb.rgbtRed;
			Colour8 colour = _Image->_Palette->GetEntry(i);
			colour.red = rgb.rgbtRed;
			colour.green = rgb.rgbtGreen;
			colour.blue = rgb.rgbtBlue;
		}	
	}
	else
	{
		for (i = 0; i < (int)bih.biClrUsed; ++i)
		{
			RGBQUAD rgb;
			stream >> rgb.rgbBlue;
			stream >> rgb.rgbGreen;
			stream >> rgb.rgbRed;
			stream >> rgb.rgbReserved;
			Colour8 colour = _Image->_Palette->GetEntry(i);
			colour.red = rgb.rgbRed;
			colour.green = rgb.rgbGreen;
			colour.blue = rgb.rgbBlue;
		}
	}	

	stream.Read((byte*)_Image->_Palette->GetEntries(), 256 * 4);

	// read the data
	stream.Seek(bfh.bfOffBits, SeekOrigin_Begin);
	byte* data = _Image->GetData();

	int32 read;
	byte padSize;
	byte padding[4];
	uint32 rMask, gMask, bMask;
	uint32 rShiftR, gShiftR, bShiftR;
	uint32 rShiftL, gShiftL, bShiftL;

	switch (bih.biBitCount)
	{
	case 8:
	case 24:
		padSize = (4 - (_Image->_BitsPerPixel % 4)) % 4;

		if (padSize == 0)
		{
			if (stream.Read(data, _Image->_DataSize) != _Image->_DataSize)
			{
				return false;
			}
		}
		else
		{
			padSize = (4 - (_Image->_BitsPerPixel % 4));
			for (i = 0; i < _Image->_DataSize; i += _Image->_BitsPerPixel)
			{
				if (stream.Read(data + i, _Image->_BitsPerPixel) != _Image->_BitsPerPixel)
				{
					return false;
				}

				// skip padding
				stream.Read(padding, padSize);
			}
		}

		break;

	case 32:
		if (bih.biCompression == BI_BITFIELDS) // bitfields
		{
			stream.Seek(bfh.bfOffBits - 12, SeekOrigin_Begin); //seek to bitfield data
			stream >> rMask;
			stream >> gMask;
			stream >> bMask;
			GetShiftFromMask(rMask, &rShiftL, &rShiftR);
			GetShiftFromMask(gMask, &gShiftL, &gShiftR);
			GetShiftFromMask(bMask, &bShiftL, &bShiftR);
		}
		else
		{
			rMask = 0xff0000;
			gMask = 0x00ff00;
			bMask = 0x0000ff;
			rShiftR = 16;
			gShiftR = 8;
			bShiftR = 0;
			rShiftL = 0;
			gShiftL = 0;
			bShiftL = 0;
		}

		for (i = 0; i < _Image->_DataSize; i += 3)
		{
			stream >> read;

			data[i] = ((read & bMask) >> bShiftR) << bShiftL;
			data[i + 1] = ((read & gMask) >> gShiftR) << gShiftL;
			data[i + 2] = ((read & rMask) >> rShiftR) << rShiftL;
		}
		break;
	}


	return true;
}

}
