/*=============================================================================
Image.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Image.h"

namespace Raytracer
{

RTImage::RTImage() :
	RefObject(),
	_Format(PixelFormat_Unknown),
	_Width(0),
	_Height(0),
	_BitsPerPixel(0),
	_BytesPerLine(0),
	_DataSize(0),
	_Data(NULL)
{
}

RTImage::~RTImage()
{
	Destroy();
}

Colour8 RTImage::GetRGB(int x, int y)
{
	int32 width = GetWidth();
	int32 height = GetHeight();
	if (x < 0 || x >= width || y < 0 || y >= height)
		return Colour8::Black;

	//if (_Format != PixelFormat_R8G8B8 || _Format != PixelFormat_R8G8B8A8)
	//	return Colour8::Black;

	int32 bytes = GetBitsPerPixel() / 8;
	kmByte* data = GetData();
	uint8* pixel = (uint8*)&data[(y*width+x)*bytes];
	return Colour8(pixel[2], pixel[1], pixel[0], pixel[3]);
}

void RTImage::SetRGB(int x, int y, const Colour8& colour)
{
	int32 width = GetWidth();
	int32 height = GetHeight();
	if (x < 0 || x >= width || y < 0 || y >= height)
		return;

	//if (_Format != PixelFormat_R8G8B8 || _Format != PixelFormat_R8G8B8A8)
	//	return;

	int32 bytes = GetBitsPerPixel() / 8;
	kmByte* data = GetData();
	uint32 pixel = colour.ToARGB();
	Memory::Copy(&data[(y*width+x)*bytes], &pixel, bytes);
}

void RTImage::Create(PixelFormat format, int32 width, int32 height)
{
	Destroy();

	_Format = format;
	_Width = width;
	_Height = height;

	if (_Format == PixelFormat_R8G8B8)
		_BitsPerPixel = 24;
	else if (_Format == PixelFormat_R8G8B8A8)
		_BitsPerPixel = 32;
	else
		_BitsPerPixel = 0;

	_BytesPerLine = (_BitsPerPixel / 8) * _Width;

	_DataSize = _Width * _Height * (_BitsPerPixel / 8);
	_Data = new kmByte[_DataSize];
}

void RTImage::Destroy()
{
	_Format = PixelFormat_Unknown;
	_Width = 0;
	_Height = 0;
	_BitsPerPixel = 0;
	_BytesPerLine = 0;
	_DataSize = 0;
	SE_SAFE_DELETE_ARRAY(_Data);
}

}
