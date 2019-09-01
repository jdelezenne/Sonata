/*=============================================================================
DDSImageReader.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "DDSImageReader.h"

namespace SE_DDS
{

DDSImageReader::DDSImageReader() :
	ImageReader()
{
}

DDSImageReader::~DDSImageReader()
{
}

Image* DDSImageReader::LoadImage(Stream& stream, ImageReaderOptions* options)
{
	DWORD dwMagic;
	DDSURFACEDESC2 ddsd;

	// magic value
	stream >> dwMagic;

	if (dwMagic != DDSMAGIC)
	{
		return NULL;
	}

	// surface format header
	stream.Read((byte*)&ddsd, sizeof(DDSURFACEDESC2));

	if ((ddsd.dwFlags & DDSD_CAPS) == 0 || (ddsd.dwFlags & DDSD_PIXELFORMAT) == 0 ||
		(ddsd.dwFlags & DDSD_WIDTH) == 0 || (ddsd.dwFlags & DDSD_HEIGHT) == 0)
	{
		return NULL;
	}

	if ((ddsd.ddsCaps.dwCaps & DDSCAPS_TEXTURE) == 0)
	{
		return NULL;
	}

	PixelFormat format;

	if (ddsd.ddpfPixelFormat.dwFlags & DDPF_FOURCC)
	{
		// compressed
		switch(ddsd.ddpfPixelFormat.dwFourCC)
		{
			case FOURCC_DXT1:
				format = PixelFormat::DXT1;
				break;
			case FOURCC_DXT2:
				format = PixelFormat::DXT2;
				break;
			case FOURCC_DXT3:
				format = PixelFormat::DXT3;
				break;
			case FOURCC_DXT4:
				format = PixelFormat::DXT4;
				break;
			case FOURCC_DXT5:
				format = PixelFormat::DXT5;
				break;
			default:
				return NULL;
		}
	}
	else if (ddsd.ddpfPixelFormat.dwFlags & DDPF_RGB)
	{
		// uncompressed rgb/rgba
		if (ddsd.ddpfPixelFormat.dwFlags & DDPF_ALPHAPIXELS)
		{
			format = PixelFormat(ddsd.ddpfPixelFormat.dwRGBBitCount,
				ddsd.ddpfPixelFormat.dwRBitMask,
				ddsd.ddpfPixelFormat.dwGBitMask,
				ddsd.ddpfPixelFormat.dwBBitMask,
				ddsd.ddpfPixelFormat.dwLuminanceAlphaBitMask);
		}
		else
		{
			format = PixelFormat(ddsd.ddpfPixelFormat.dwRGBBitCount,
				ddsd.ddpfPixelFormat.dwRBitMask,
				ddsd.ddpfPixelFormat.dwGBitMask,
				ddsd.ddpfPixelFormat.dwBBitMask, 0);
		}
	}
	else if (ddsd.ddpfPixelFormat.dwFlags & DDPF_ALPHA)
	{
		// uncompressed alpha luminance
		format = PixelFormat(ddsd.ddpfPixelFormat.dwRGBBitCount,
			0, 0, 0, ddsd.ddpfPixelFormat.dwLuminanceAlphaBitMask);
		format.SetFlags(PixelFormatFlag_Luminance);
	}
	else
	{
		return NULL;
	}

	int32 mipLevels = (ddsd.ddsCaps.dwCaps & DDSCAPS_MIPMAP ? ddsd.dwMipMapCount : 0);

	_Image = new Image();
	_Image->Create(format, ddsd.dwWidth, ddsd.dwHeight, ddsd.dwDepth, mipLevels);

	Image* image = _Image;
	int32 width = ddsd.dwWidth;
	int32 height = ddsd.dwHeight;
	int32 depth = ddsd.dwDepth;
	int32 bpp = ddsd.ddpfPixelFormat.dwRGBBitCount / 8;

	// surface
	byte* data = image->GetData();
	int32 dataSize = width * height * (depth == 0 ? 1 : depth) * bpp;
	stream.Read(data, dataSize);

	// mipmaps
	if (ddsd.dwFlags & DDSD_MIPMAPCOUNT)
	{
		for (int mipLevel = 0; mipLevel < (int)ddsd.dwMipMapCount - 1; ++mipLevel)
		{
			width >>= 1;
			height >>= 1;
			depth >>= 1;

			image = image->GetMipmap(mipLevel);
			image->Create(format, width, height, depth, 0);

			data = image->GetData();
			dataSize = width * height * depth * bpp;
			stream.Read(data, dataSize);
		}
	}

	return _Image;
}

}
