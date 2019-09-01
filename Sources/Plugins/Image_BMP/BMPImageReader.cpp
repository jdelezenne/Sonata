/*=============================================================================
BMPImageReader.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "BMPImageReader.h"

namespace SE_BMP
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

BMPImageReader::BMPImageReader() :
	ImageReader()
{
}

BMPImageReader::~BMPImageReader()
{
}

Image* BMPImageReader::LoadImage(Stream& stream, ImageReaderOptions* options)
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
		if (bch.bcBitCount == 1)
			format = PixelFormat::Indexed1;
		else if (bch.bcBitCount == 4)
			format = PixelFormat::Indexed4;
		else if (bch.bcBitCount == 8)
			format = PixelFormat::Indexed8;
		else if (bch.bcBitCount == 24)
			format = PixelFormat::R8G8B8;

		_Image = new Image();
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
		if (bih.biBitCount == 1)
			format = PixelFormat::Indexed1;
		else if (bih.biBitCount == 4)
			format = PixelFormat::Indexed4;
		else if (bih.biBitCount == 8)
			format = PixelFormat::Indexed8;
		else if (bih.biBitCount == 24)
			format = PixelFormat::R8G8B8;
		else if (bih.biBitCount == 32)
			format = PixelFormat::R8G8B8A8;

		_Image = new Image();

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

		if (!res)
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return _Image;
}

bool BMPImageReader::ReadRGB(Stream& stream)
{
	int i;
	PixelFormat format;

	switch (bih.biBitCount)
	{
	case 4:
	case 8:
		format = PixelFormat(PixelFormatFlag_Indexed);
		break;

	case 16:
	case 24:
		format = PixelFormat::R8G8B8;
		break;
	case 32:
		format = PixelFormat::R8G8B8A8;
		//format = PixelFormat::R5G5B5;
		break;

	default:
		return false;
	}

	int width = bih.biWidth;
	int height = bih.biHeight;

	_Image->Create(format, width, height, 0, 0);

	// read the palette
	if (bih.biBitCount == 4 || bih.biBitCount == 8)
	{
		bih.biClrUsed = 1 << bih.biBitCount;
		Palette* palette = new Palette(bih.biClrUsed);
		if (bih.biSize == 12)
		{
			for (i = 0; i < (int)bih.biClrUsed; ++i)
			{
				RGBTRIPLE rgb;
				stream >> rgb.rgbtBlue;
				stream >> rgb.rgbtGreen;
				stream >> rgb.rgbtRed;
				Color8 color = palette->GetEntry(i);
				color.r = rgb.rgbtRed;
				color.g = rgb.rgbtGreen;
				color.b = rgb.rgbtBlue;
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
				Color8 color = palette->GetEntry(i);
				color.r = rgb.rgbRed;
				color.g = rgb.rgbGreen;
				color.b = rgb.rgbBlue;
			}
		}	

		_Image->SetPalette(palette);
	}

	// read the data
	stream.Seek(bfh.bfOffBits, SeekOrigin_Begin);
	SEbyte* data = _Image->GetData();
	int32 dataSize = _Image->GetDataSize();
	int32 bits = _Image->GetBitsPerPixel();
	int32 bpp = _Image->GetBitsPerPixel() / 8;

	int32 read;
	SEbyte padSize;
	SEbyte padding[4];
	uint32 rMask, gMask, bMask;
	uint32 rShiftR, gShiftR, bShiftR;
	uint32 rShiftL, gShiftL, bShiftL;

	switch (bih.biBitCount)
	{
	case 8:
	case 24:
		padSize = (4 - (bits % 4)) % 4;

		/*if (padSize == 0)
		{
			if (stream.Read(data, dataSize) != dataSize)
			{
				return false;
			}
		}
		else
		{*/
			int x, y;
			//padSize = (4 - (bpp % 4));
			//for (i = 0; i < dataSize; i += bpp)
			for (y = (height - 1); y >= 0; y--)
			{
				for (x = 0; x < width; x++)
				{
					int line_offset = (y * width * bpp + x * bpp);
					Color8 color;
					stream >> color.b;
					stream >> color.g;
					stream >> color.r;
					data[line_offset] = color.r;
					data[line_offset+1] = color.g;
					data[line_offset+2] = color.b;
				}

				// skip padding
				stream.Read(padding, padSize);
			}
		//}

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

		for (i = 0; i < dataSize; i += 3)
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

bool BMPImageReader::ReadRLE8(Stream& stream)
{
	return false;
}

bool BMPImageReader::ReadRLE4(Stream& stream)
{
	return false;
}

}
