/*=============================================================================
ImageHelper.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ImageHelper.h"

namespace SonataEngine
{

void ImageHelper::Convert(Image* destination, Image* source, const PixelFormatDesc& format)
{
	int width = source->GetWidth();
	int height = source->GetHeight();
	PixelFormatDesc in_pf = source->GetFormat();
	PixelFormatDesc out_pf = destination->GetFormat();
	const int in_bpp = in_pf.GetDepth() / 8;
	const int out_bpp = out_pf.GetDepth() / 8;
	SEbyte* in_p = source->GetData();
	SEbyte* out_p = destination->GetData();

	const int in_r_shift = PixelFormatDesc::GetMaskShift(in_pf.GetRedMask());
	const int in_g_shift = PixelFormatDesc::GetMaskShift(in_pf.GetGreenMask());
	const int in_b_shift = PixelFormatDesc::GetMaskShift(in_pf.GetBlueMask());
	const int in_a_shift = PixelFormatDesc::GetMaskShift(in_pf.GetAlphaMask());

	const int in_r_bits = PixelFormatDesc::GetMaskBits(in_pf.GetRedMask());
	const int in_g_bits = PixelFormatDesc::GetMaskBits(in_pf.GetGreenMask());
	const int in_b_bits = PixelFormatDesc::GetMaskBits(in_pf.GetBlueMask());
	const int in_a_bits = PixelFormatDesc::GetMaskBits(in_pf.GetAlphaMask());

	const int in_r_bitmask = PixelFormatDesc::GetBitmask(in_r_bits,  0);
	const int in_g_bitmask = PixelFormatDesc::GetBitmask(in_g_bits, 0);
	const int in_b_bitmask = PixelFormatDesc::GetBitmask(in_b_bits, 0);
	const int in_a_bitmask = PixelFormatDesc::GetBitmask(in_a_bits, 0);

	const int out_r_shift = PixelFormatDesc::GetMaskShift(out_pf.GetRedMask());
	const int out_g_shift = PixelFormatDesc::GetMaskShift(out_pf.GetGreenMask());
	const int out_b_shift = PixelFormatDesc::GetMaskShift(out_pf.GetBlueMask());
	const int out_a_shift = PixelFormatDesc::GetMaskShift(out_pf.GetAlphaMask());

	const int out_r_bits = PixelFormatDesc::GetMaskBits(out_pf.GetRedMask());
	const int out_g_bits = PixelFormatDesc::GetMaskBits(out_pf.GetGreenMask());
	const int out_b_bits = PixelFormatDesc::GetMaskBits(out_pf.GetBlueMask());
	const int out_a_bits = PixelFormatDesc::GetMaskBits(out_pf.GetAlphaMask());
	
	for (int y = 0; y < height; y++, in_p += 0, out_p += 0)
	{
		SEbyte* in_p2 = in_p;
		SEbyte* out_p2 = out_p;
		for (int x = 0; x < width; x++, in_p2 += in_bpp, out_p2 += out_bpp)
		{
			unsigned int in_pixel = 0;
			Memory::Copy(&in_pixel, in_p2, in_bpp);

			// decode
			unsigned int in_r = 0, in_g = 0, in_b = 0, in_a = 0xffffffff;
			if (in_r_bitmask)
			{
				in_r = ((in_pixel >> in_r_shift) & in_r_bitmask);
				if (in_r & 1)
					in_r = (in_r << (32 - in_r_bits)) | PixelFormatDesc::GetBitmask(32 - in_r_bits, 0);
				else
					in_r <<= 32 - in_r_bits;
			}
			if (in_g_bitmask)
			{
				in_g = ((in_pixel >> in_g_shift) & in_g_bitmask);
				if (in_g & 1)
					in_g = (in_g << (32 - in_g_bits)) | PixelFormatDesc::GetBitmask(32 - in_g_bits, 0);
				else
					in_g <<= 32 - in_g_bits;
			}
			if (in_b_bitmask)
			{
				in_b = ((in_pixel >> in_b_shift) & in_b_bitmask);
				if (in_b & 1)
					in_b = (in_b << (32 - in_b_bits)) | PixelFormatDesc::GetBitmask(32 - in_b_bits, 0);
				else
					in_b <<= 32 - in_b_bits;
			}
			if (in_a_bitmask)
			{
				in_a = ((in_pixel >> in_a_shift) & in_a_bitmask);
				if (in_a & 1)
					in_a = (in_a << (32 - in_a_bits)) | PixelFormatDesc::GetBitmask(32 - in_a_bits, 0);
				else
					in_a <<= 32 - in_a_bits;
			}

			unsigned int out_pixel;

			// encode
			out_pixel = 0;
			if (out_r_bits)
				out_pixel |= (in_r >> (32 - out_r_bits)) << out_r_shift;
			if (out_g_bits)
				out_pixel |= (in_g >> (32 - out_g_bits)) << out_g_shift;
			if (out_b_bits)
				out_pixel |= (in_b >> (32 - out_b_bits)) << out_b_shift;
			if (out_a_bits)
				out_pixel |= (in_a >> (32 - out_a_bits)) << out_a_shift;

			Memory::Copy(out_p2, &out_pixel, out_bpp);
		}
	}
}

void ImageHelper::Flip(Image* destination, Image* source, ImageFlipType type)
{
	//TODO
}

void ImageHelper::Resize(Image* destination, Image* source, ImageResizeType type)
{
	//TODO
}

}
