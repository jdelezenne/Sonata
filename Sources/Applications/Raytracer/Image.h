/*=============================================================================
Image.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _RAYTRACER_IMAGE_H_
#define _RAYTRACER_IMAGE_H_

#include "Common.h"

namespace Raytracer
{
	enum PixelFormat
	{
		PixelFormat_Unknown,
		PixelFormat_R8G8B8,
		PixelFormat_R8G8B8A8,
		PixelFormat_Luminance,
		PixelFormat_DXT1,
		PixelFormat_DXT3,
		PixelFormat_DXT5
	};

	class RTImage : public RefObject
	{
	public:
		//@{
		RTImage();

		virtual ~RTImage();
		//@}

		//@{
		PixelFormat GetFormat() const { return _Format; }

		int32 GetWidth() const { return _Width; }

		int32 GetHeight() const { return _Height; }

		const SizeInt GetSize() { return SizeInt(_Width, _Height); }

		int32 GetBitsPerPixel() const { return _BitsPerPixel; }

		int32 GetDataSize() const { return _DataSize; }

		int32 GetBytesPerLine() const { return _BytesPerLine; }

		kmByte* GetData() const { return _Data; }

		Colour8 GetRGB(int x, int y);

		void SetRGB(int x, int y, const Colour8& colour);
		//@}

		//@{
		void Create(PixelFormat format, int32 width, int32 height);
		//@}

	protected:
		void Destroy();

	protected:
		PixelFormat _Format;
		int32 _Width;
		int32 _Height;
		int32 _BitsPerPixel;
		int32 _BytesPerLine;
		int32 _DataSize;
		kmByte* _Data;
	};
}

#endif 
