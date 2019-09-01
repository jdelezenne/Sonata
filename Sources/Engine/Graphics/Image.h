/*=============================================================================
Image.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_IMAGE_H_
#define _SE_IMAGE_H_

#include "Graphics/Common.h"
#include "Graphics/PixelFormat.h"
#include "Graphics/Palette.h"

namespace SonataEngine
{

/**
	@brief Image data.
*/
class SE_GRAPHICS_EXPORT Image : public RefObject
{
protected:
	PixelFormat _format;
	Palette* _Palette;
	int32 _width;
	int32 _height;
	int32 _depth;
	int32 _bitsPerPixel;
	int32 _bytesPerLine;
	int32 _mipLevels;
	Array<Image*> _mipmaps;
	int32 _dataSize;
	SEbyte* _data;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	Image();

	/** Destructor. */
	virtual ~Image();
	//@}

	/** Retrieves the pixel format of the image. */
	PixelFormat GetFormat() const { return _format; }

	/** Retrieves the palette of the image. */
	const Palette* GetPalette() const { return _Palette; }

	/** Sets the palette of the image. */
	void SetPalette(Palette* value) { _Palette = value; }

	/** Retrieves the width of the image, in pixels. */
	int32 GetWidth() const { return _width; }

	/** Retrieves the height of the image, in pixels. */
	int32 GetHeight() const { return _height; }

	/** Retrieves the depth of the image, in pixels. */
	int32 GetDepth() const { return _depth; }

	/** Retrieves the size of the image, in pixels. */
	const SizeInt GetSize() { return SizeInt(_width, _height); }

	/** Retrieves the number of bits per pixel in the image. */
	int32 GetBitsPerPixel() const { return _bitsPerPixel; }

	/** Retrieves the number of mip levels in the image. */
	int32 GetMipLevels() const { return _mipLevels; }

	/** Sets the number of mip levels in the image. */
	void SetMipLevels(int32 value);

	/** Retrieves the size of the image data, in bytes. */
	int32 GetDataSize() const { return _dataSize; }

	/** Retrieves the number of bytes per line in the image (pitch). */
	int32 GetBytesPerLine() const { return _bytesPerLine; }

	/** Retrieves the image data. */
	SEbyte* GetData() const { return _data; }

	/** Retrieves the mipmap data at a given miplevel. */
	Image* GetMipmap(int32 mipLevel);

	/** Sets the mipmap data at a given miplevel. */
	void SetMipmap(int32 mipLevel, Image* mipmap);

	Color8 GetRGB(int x, int y);

	void SetRGB(int x, int y, const Color8& color);

	void Create(PixelFormat format, int32 width, int32 height, int32 depth = 1, int32 mipLevels = 0);

	void Destroy();
};

}

#endif 
