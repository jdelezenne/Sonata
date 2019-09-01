/*=============================================================================
PixelFormat.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "PixelFormat.h"

namespace SonataEngine
{
/*
int8 GetRed(Image* image, int x, int y)
{
	byte* data = image->GetData();
	uint32 pixel = *(int32*)&data[(y*image->GetWidth()+x)*image->GetBitsPerPixel()/8];
	uint32 mask = image->GetFormat().GetRedMask();
	uint32 shift = PixelFormatDesc::GetMaskShift(mask);
	uint32 bits = PixelFormatDesc::GetMaskBits(mask);
	return ((pixel >> shift) & mask);
}

int8 GetGreen(Image* image, int x, int y)
{
	byte* data = image->GetData();
	uint32 pixel = *(int32*)&data[(y*image->GetWidth()+x)*image->GetBitsPerPixel()/8];
	uint32 mask = image->GetFormat().GetGreenMask();
	uint32 shift = PixelFormatDesc::GetMaskShift(mask);
	uint32 bits = PixelFormatDesc::GetMaskBits(mask);
	return ((pixel >> shift) & mask);
}

int8 GetBlue(Image* image, int x, int y)
{
	byte* data = image->GetData();
	uint32 pixel = *(int32*)&data[(y*image->GetWidth()+x)*image->GetBitsPerPixel()/8];
	uint32 mask = image->GetFormat().GetBlueMask();
	uint32 shift = PixelFormatDesc::GetMaskShift(mask);
	uint32 bits = PixelFormatDesc::GetMaskBits(mask);
	return ((pixel >> shift) & mask);
}

Color8 GetRGB(Image* image, int x, int y)
{
	Color8 color;
	color.r = GetRed(image, x, y);
	color.g = GetGreen(image, x, y);
	color.b = GetBlue(image, x, y);
	return color;
}

void SetRGB(Image* image, int x, int y, const Color8& color)
{
	uint32 pixel;
	byte* data = image->GetData();

	uint32 r_mask = image->GetFormat().GetRedMask();
	uint32 g_mask = image->GetFormat().GetGreenMask();
	uint32 b_mask = image->GetFormat().GetBlueMask();

	uint32 r_shift = PixelFormatDesc::GetMaskShift(r_mask);
	uint32 g_shift = PixelFormatDesc::GetMaskShift(g_mask);
	uint32 b_shift = PixelFormatDesc::GetMaskShift(b_mask);

	uint32 r_bits = PixelFormatDesc::GetMaskBits(r_mask);
	uint32 g_bits = PixelFormatDesc::GetMaskBits(g_mask);
	uint32 b_bits = PixelFormatDesc::GetMaskBits(b_mask);

	pixel = 0;
	pixel |= (color.r >> (32 - r_bits)) << r_shift;
	pixel |= (color.g >> (32 - g_bits)) << g_shift;
	pixel |= (color.b >> (32 - b_bits)) << b_shift;

	Memory::Copy(&data[(y*image->GetWidth()+x)*image->GetBitsPerPixel()/8], &pixel, 4);
}
*/
const PixelFormatDesc PixelFormatDesc::Unknown = PixelFormatDesc();
const PixelFormatDesc PixelFormatDesc::Indexed1 = PixelFormatDesc(PixelFormatFlag_Indexed, PixelFormatSpecial_Normal, 1);
const PixelFormatDesc PixelFormatDesc::Indexed4 = PixelFormatDesc(PixelFormatFlag_Indexed, PixelFormatSpecial_Normal, 4);
const PixelFormatDesc PixelFormatDesc::Indexed8 = PixelFormatDesc(PixelFormatFlag_Indexed, PixelFormatSpecial_Normal, 8);
const PixelFormatDesc PixelFormatDesc::DXT1 = PixelFormatDesc(PixelFormatFlag_Compressed, PixelFormatSpecial_DXT1);
const PixelFormatDesc PixelFormatDesc::DXT2 = PixelFormatDesc(PixelFormatFlag_Compressed, PixelFormatSpecial_DXT2);
const PixelFormatDesc PixelFormatDesc::DXT3 = PixelFormatDesc(PixelFormatFlag_Compressed, PixelFormatSpecial_DXT3);
const PixelFormatDesc PixelFormatDesc::DXT4 = PixelFormatDesc(PixelFormatFlag_Compressed, PixelFormatSpecial_DXT4);
const PixelFormatDesc PixelFormatDesc::DXT5 = PixelFormatDesc(PixelFormatFlag_Compressed, PixelFormatSpecial_DXT5);
const PixelFormatDesc PixelFormatDesc::R5G5B5 = PixelFormatDesc(16, 0x7c00, 0x03e0, 0x001f, 0x0000);
const PixelFormatDesc PixelFormatDesc::B5G5R5 = PixelFormatDesc(16, 0x001f, 0x03e0, 0x7c00, 0x0000);
const PixelFormatDesc PixelFormatDesc::R5G6B5 = PixelFormatDesc(16, 0xf800, 0x07e0, 0x001f, 0x0000);
const PixelFormatDesc PixelFormatDesc::B5G6R5 = PixelFormatDesc(16, 0x001f, 0x07e0, 0xf800, 0x0000);
const PixelFormatDesc PixelFormatDesc::R4G4B4A4 = PixelFormatDesc(16, 0xf000, 0x0f00, 0x00f0, 0x000f);
const PixelFormatDesc PixelFormatDesc::A4R4G4B4 = PixelFormatDesc(16, 0x0f00, 0x00f0, 0x000f, 0xf000);
const PixelFormatDesc PixelFormatDesc::X4R4G4B4 = PixelFormatDesc(16, 0x0f00, 0x00f0, 0x000f, 0x0000);
const PixelFormatDesc PixelFormatDesc::A1R5G5B5 = PixelFormatDesc(16, 0x7c00, 0x03e0, 0x001f, 0x8000);
const PixelFormatDesc PixelFormatDesc::X1R5G5B5 = PixelFormatDesc(16, 0x7c00, 0x03e0, 0x001f, 0x0000);
const PixelFormatDesc PixelFormatDesc::R8G8B8 = PixelFormatDesc(24, 0xff0000, 0x00ff00, 0x0000ff, 0x000000);
const PixelFormatDesc PixelFormatDesc::B8G8R8 = PixelFormatDesc(24, 0x0000ff, 0x00ff00, 0xff0000, 0x000000);
const PixelFormatDesc PixelFormatDesc::R8G8B8A8 = PixelFormatDesc(32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
const PixelFormatDesc PixelFormatDesc::B8G8R8A8 = PixelFormatDesc(32, 0x0000ff00, 0x00ff0000, 0xff000000, 0x000000ff);
const PixelFormatDesc PixelFormatDesc::A8R8G8B8 = PixelFormatDesc(32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
const PixelFormatDesc PixelFormatDesc::A8B8G8R8 = PixelFormatDesc(32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
const PixelFormatDesc PixelFormatDesc::X8R8G8B8 = PixelFormatDesc(32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000);
const PixelFormatDesc PixelFormatDesc::X8B8G8R8 = PixelFormatDesc(32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000);

PixelFormatDesc::PixelFormatDesc(PixelFormatFlag flags, PixelFormatSpecial special, int32 depth)
{
	_Flags = flags;
	_Special = special;
	_Depth = depth;
	_RedMask = 0;
	_GreenMask = 0;
	_BlueMask = 0;
	_AlphaMask = 0;
	_HasColorKey = false;
	_ColorKey = Color8::Black;
}

PixelFormatDesc::PixelFormatDesc(int32 depth, uint32 redMask, uint32 greenMask, uint32 blueMask, uint32 alphaMask, PixelFormatType formatType)
{
	_Flags = (PixelFormatFlag)0;
	_Special = PixelFormatSpecial_Normal;
	_Depth = depth;
	_RedMask = redMask;
	_GreenMask = greenMask;
	_BlueMask = blueMask;
	_AlphaMask = alphaMask;
	_HasColorKey = false;
	_ColorKey = Color8::Black;
}

PixelFormatDesc::PixelFormatDesc(PixelFormat format)
{
	switch (format)
	{
	case PixelFormat_Unknown:
		_Depth = 0;
		break;
	case PixelFormat_R8G8B8:
		*this = PixelFormatDesc::R8G8B8;
		break;
	case PixelFormat_R8G8B8A8:
		*this = PixelFormatDesc::R8G8B8A8;
		break;
	case PixelFormat_Luminance:
		_Depth = 1;
		break;
	case PixelFormat_DXT1:
		break;
	case PixelFormat_DXT3:
		break;
	case PixelFormat_DXT5:
		break;
	}
}

PixelFormatDesc::~PixelFormatDesc()
{
}

bool PixelFormatDesc::operator==(const PixelFormatDesc& value) const
{
	if (_Flags != value.GetFlags()) return false;
	if (_Special != value.GetSpecial()) return false;
	if (_Depth != value.GetDepth()) return false;
	if (_RedMask != value.GetRedMask()) return false;
	if (_GreenMask != value.GetGreenMask()) return false;
	if (_BlueMask != value.GetBlueMask()) return false;
	if (_AlphaMask != value.GetAlphaMask()) return false;
	if (_FormatType != value.GetFormatType()) return false;
	if (_Flags != value.GetFlags()) return false;

	return true;
}

bool PixelFormatDesc::operator!=(const PixelFormatDesc& value) const
{
	return !(*this == value);
}

int32 PixelFormatDesc::GetMaskBits(uint32 mask)
{
	if (mask == 0)
		return 0;

	int32 shift = GetMaskShift(mask);
	mask >>= shift;

	int32 remaining = (32 - shift);
	int32 bits;
	for (bits = 0; bits < remaining; bits++)
	{
		if ((mask & 1) == 0)
			return bits;
		else
			mask >>= 1;
	}

	return bits;
}

int32 PixelFormatDesc::GetMaskShift(uint32 mask)
{
	if (mask == 0)
		return 0;

	int32 shift;
	for (shift = 0; shift < 32; shift++)
	{
		if ((mask & 1) != 0)
			return shift;
		else
			mask >>= 1;
	}

	return 0;
}

uint32 PixelFormatDesc::GetBitmask(int32 bits, int32 shift)
{
	uint32 mask = 0;
	for (int32 i = 0; i < bits; i++)
		mask |= (1 << i);

	return mask << shift;
}

}
