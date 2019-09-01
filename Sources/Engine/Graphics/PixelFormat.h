/*=============================================================================
PixelFormat.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PIXELFORMAT_H_
#define _SE_PIXELFORMAT_H_

#include "Core/Core.h"
#include "Graphics/Common.h"

namespace SonataEngine
{

/** Pixel formats. */
enum PixelFormat
{
	PixelFormat_Unknown,
	PixelFormat_R8G8B8,
	PixelFormat_R8G8B8A8,
	PixelFormat_Luminance,
	PixelFormat_DXT1,
	PixelFormat_DXT3,
	PixelFormat_DXT5,
	PixelFormat_Depth
};

/** Pixel format flags. */
enum PixelFormatFlag
{
	PixelFormatFlag_Indexed =    (1<<1),
	PixelFormatFlag_Compressed = (1<<2),
	PixelFormatFlag_Float =      (1<<3),
	PixelFormatFlag_Depth =      (1<<4),
	PixelFormatFlag_Luminance =  (1<<5)
};

/** Special Pixel formats. */
enum PixelFormatSpecial
{
	PixelFormatSpecial_Unknown,
	PixelFormatSpecial_Normal,
	PixelFormatSpecial_DXT1,
	PixelFormatSpecial_DXT2,
	PixelFormatSpecial_DXT3,
	PixelFormatSpecial_DXT4,
	PixelFormatSpecial_DXT5
};

/** Pixel format types.
	@remark From D3D10
*/
enum PixelFormatType
{
	/// Unspecified type
	PixelFormatType_Typeless,

	/// Unsigned int 32 bit unsigned integer
	PixelFormatType_UInt32,

	/// 32 bit signed integer
	PixelFormatType_Int32,

	/// IEEE 32 bit float
	PixelFormatType_Real32,

	/// IEEE 32 bit float in range 0 to 1 inclusive
	PixelFormatType_Real32UNorm,

	/// IEEE 32 bit float in range -1 to 1 inclusive
	PixelFormatType_Real32SNorm
};

/**
	@brief Pixel format.
*/
class SE_GRAPHICS_EXPORT PixelFormatDesc
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	PixelFormatDesc(PixelFormatFlag flags = (PixelFormatFlag)0,
		PixelFormatSpecial special = (PixelFormatSpecial)0, int32 depth = 0);

	/** Constructor. */
	PixelFormatDesc(int32 depth, uint32 redMask, uint32 greenMask, uint32 blueMask, uint32 alphaMask, PixelFormatType formatType = PixelFormatType_Typeless);

	PixelFormatDesc(PixelFormat format);

	/** Destructor. */
	virtual ~PixelFormatDesc();
	//@}

	/** @name Operators. */
	//@{
	bool operator==(const PixelFormatDesc& value) const;
	bool operator!=(const PixelFormatDesc& value) const;
	//@}

	/** @name Properties. */
	//@{
	PixelFormatFlag GetFlags() const { return _Flags; }
	void SetFlags(PixelFormatFlag value) { _Flags = value; }

	PixelFormatSpecial GetSpecial() const { return _Special; }
	void SetSpecial(PixelFormatSpecial value) { _Special = value; }

	int32 GetDepth() const { return _Depth; }
	void SetDepth(int32 value) { _Depth = value; }

	uint32 GetRedMask() const { return _RedMask; }
	void SetRedMask(uint32 value) { _RedMask = value; }

	uint32 GetGreenMask() const { return _GreenMask; }
	void SetGreenMask(uint32 value) { _GreenMask = value; }

	uint32 GetBlueMask() const { return _BlueMask; }
	void SetBlueMask(uint32 value) { _BlueMask = value; }

	uint32 GetAlphaMask() const { return _AlphaMask; }
	void SetAlphaMask(uint32 value) { _AlphaMask = value; }

	PixelFormatType GetFormatType() const { return _FormatType; }
	void SetFormatType(PixelFormatType value) { _FormatType = value; }
	//@}

	/** Gets the number of bits of a pixel format from a mask value. */
	static int32 GetMaskBits(uint32 mask);

	/** Gets a shift value from a mask value. */
	static int32 GetMaskShift(uint32 mask);

	/** Gets a mask value from a number of bits and a shift value. */
	static uint32 GetBitmask(int32 bits, int32 shift);

public:
	static const PixelFormatDesc Unknown;
	static const PixelFormatDesc Indexed1;
	static const PixelFormatDesc Indexed4;
	static const PixelFormatDesc Indexed8;
	static const PixelFormatDesc DXT1;
	static const PixelFormatDesc DXT2;
	static const PixelFormatDesc DXT3;
	static const PixelFormatDesc DXT4;
	static const PixelFormatDesc DXT5;
	static const PixelFormatDesc R5G5B5;
	static const PixelFormatDesc B5G5R5;
	static const PixelFormatDesc R5G6B5;
	static const PixelFormatDesc B5G6R5;
	static const PixelFormatDesc R4G4B4A4;
	static const PixelFormatDesc A4R4G4B4;
	static const PixelFormatDesc X4R4G4B4;
	static const PixelFormatDesc A1R5G5B5;
	static const PixelFormatDesc X1R5G5B5;
	static const PixelFormatDesc R8G8B8;
	static const PixelFormatDesc B8G8R8;
	static const PixelFormatDesc R8G8B8A8;
	static const PixelFormatDesc B8G8R8A8;
	static const PixelFormatDesc A8R8G8B8;
	static const PixelFormatDesc A8B8G8R8;
	static const PixelFormatDesc X8R8G8B8;
	static const PixelFormatDesc X8B8G8R8;

protected:
	PixelFormatFlag _Flags;
	PixelFormatSpecial _Special;
	int32 _Depth;
	uint32 _RedMask;
	uint32 _GreenMask;
	uint32 _BlueMask;
	uint32 _AlphaMask;
	PixelFormatType _FormatType;
	bool _HasColorKey;
	Color8 _ColorKey;
};

}

#endif 
