/*=============================================================================
Image.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Image.h"

namespace SonataEngine
{

Image::Image() :
	RefObject(),
	_format(PixelFormat_Unknown),
	_width(0),
	_height(0),
	_depth(0),
	_bitsPerPixel(0),
	_bytesPerLine(0),
	_mipLevels(0),
	_dataSize(0),
	_data(NULL)
{
}

Image::~Image()
{
	Destroy();
}

void Image::SetMipLevels(int32 value)
{
	if (value < 0)
		return;

	_mipLevels = value;
	_mipmaps.Resize(value);
}

Image* Image::GetMipmap(int32 mipLevel)
{
	if (mipLevel < 0 || mipLevel >= _mipLevels)
		return NULL;

	return _mipmaps[mipLevel];
}

void Image::SetMipmap(int32 mipLevel, Image* mipmap)
{
	if (mipLevel < 0 || mipLevel >= _mipLevels)
		return;

	_mipmaps[mipLevel] = mipmap;
}

Color8 Image::GetRGB(int x, int y)
{
	int32 width = GetWidth();
	int32 height = GetHeight();
	if (x < 0 || x >= width || y < 0 || y >= height)
		return Color8::Black;

	//if (_format != PixelFormat_R8G8B8 || _format != PixelFormat_R8G8B8A8)
	//	return Color8::Black;

	int32 bytes = GetBitsPerPixel() / 8;
	SEbyte* data = GetData();
	uint8* pixel = (uint8*)&data[(y*width+x)*bytes];
	return Color8(pixel[2], pixel[1], pixel[0], pixel[3]);
}

void Image::SetRGB(int x, int y, const Color8& color)
{
	int32 width = GetWidth();
	int32 height = GetHeight();
	if (x < 0 || x >= width || y < 0 || y >= height)
		return;

	//if (_format != PixelFormat_R8G8B8 || _format != PixelFormat_R8G8B8A8)
	//	return;

	int32 bytes = GetBitsPerPixel() / 8;
	SEbyte* data = GetData();
	uint32 pixel = color.ToARGB();
	Memory::Copy(&data[(y*width+x)*bytes], &pixel, bytes);
}

void Image::Create(PixelFormat format, int32 width, int32 height, int32 depth, int32 mipLevels)
{
	Destroy();

	_format = format;
	_width = width;
	_height = height;
	_depth = depth;
	_bitsPerPixel = PixelFormatDesc(_format).GetDepth();
	_bytesPerLine = (_bitsPerPixel / 8) * _width;

	_dataSize = _width * _height * _depth * (_bitsPerPixel / 8);
	//todo: add mipmap sizes
	_data = new SEbyte[_dataSize];
}

void Image::Destroy()
{
	for (int i = 0; i < _mipLevels; i++)
		SE_DELETE(_mipmaps[i]);

	_format = PixelFormat_Unknown;
	_width = 0;
	_height = 0;
	_depth = 0;
	_bitsPerPixel = 0;
	_bytesPerLine = 0;
	_mipLevels = 0;
	_dataSize = 0;
	SE_DELETE_ARRAY(_data);
}

}
