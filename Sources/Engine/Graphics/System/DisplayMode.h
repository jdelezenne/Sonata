/*=============================================================================
DisplayMode.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_DISPLAYMODE_H_
#define _SE_DISPLAYMODE_H_

#include "Graphics/Common.h"

namespace SonataEngine
{

/** @brief Display mode scanline order.
	
	Flags indicating the method the raster uses to create an image on a surface.
	@remark From D3D10
*/
enum DisplayModeScanlineOrder
{
	/// Scanline order is unspecified (interlaced).
	DisplayModeScanlineOrder_Unspecified,

	/// The image is created from the first scanline to the last without skipping any.
	DisplayModeScanlineOrder_Progressive,

	/// The image is created beginning with the upper field.
	DisplayModeScanlineOrder_UpperFieldFirst,

	/// The image is created beginning with the lower field.
	DisplayModeScanlineOrder_LowerFieldFirst
};

/** @brief Display mode scaling.

	Flags indicating how an image is stretched to fit a given monitor's resolution.
	@remark From D3D10
*/
enum DisplayModeScaling
{
	/// Unspecified scaling.
	DisplayModeScaling_Unspecified,

	/// No scaling. The image is centered on the display.
	DisplayModeScaling_Centered,

	/// Stretched scaling.
	DisplayModeScaling_Stretched
};

/** @brief Video standard.

	Video standards used by the display.
*/
enum VideoStandard
{
	/// Unspecified video standard.
	VideoStandard_Unknown,

	/// NTSC M.
	VideoStandard_NTSC_M,

	/// NTSC J.
	VideoStandard_NTSC_J,

	/// PAL.
	VideoStandard_PAL_I
};

/**
	@brief Display Mode.

	Description of a display mode.
*/
class SE_GRAPHICS_EXPORT DisplayMode
{
public:
	/// Width of the screen, in pixels.
	int32 Width;

	/// Height of the screen, in pixels.
	int32 Height;

	/// Screen depth (bits per pixel).
	int32 Depth;

	/// Screen refresh rate, in hertz.
	int32 RefreshRate;

	/// Scanline order.
	DisplayModeScanlineOrder ScanlineOrder;

	/// Scaling.
	DisplayModeScaling Scaling;

	/// If true, indicates that the screen is a wide-screen format.
	bool IsWideScreen;

	/// If true, indicates that the screen is a high-definition format.
	bool IsHiDef;

	/// Video standard used by the display.
	VideoStandard VideoStandard;

public:
	/// Default constructor.
	DisplayMode();

	/// Constructor.
	DisplayMode(int32 width, int32 height, int32 depth, int32 refreshRate, DisplayModeScanlineOrder scanlineOrder = DisplayModeScanlineOrder_Unspecified, DisplayModeScaling scaling = DisplayModeScaling_Unspecified);

	/// Copy constructor.
	DisplayMode(const DisplayMode& value);
};

SE_INLINE DisplayMode::DisplayMode() :
	Width(0),
	Height(0),
	RefreshRate(0),
	Depth(0),
	ScanlineOrder(DisplayModeScanlineOrder_Unspecified),
	Scaling(DisplayModeScaling_Unspecified),
	IsWideScreen(false),
	IsHiDef(false),
	VideoStandard(VideoStandard_Unknown)
{
}

SE_INLINE DisplayMode::DisplayMode(int32 width, int32 height, int32 depth, int32 refreshRate,
	DisplayModeScanlineOrder scanlineOrder, DisplayModeScaling scaling) :
	Width(width),
	Height(height),
	RefreshRate(refreshRate),
	Depth(depth),
	ScanlineOrder(scanlineOrder),
	Scaling(scaling),
	IsWideScreen(false),
	IsHiDef(false),
	VideoStandard(VideoStandard_Unknown)
{
}

SE_INLINE DisplayMode::DisplayMode(const DisplayMode& value) :
	Width(value.Width),
	Height(value.Height),
	RefreshRate(value.RefreshRate),
	Depth(value.Depth),
	ScanlineOrder(value.ScanlineOrder),
	Scaling(value.Scaling),
	IsWideScreen(value.IsWideScreen),
	IsHiDef(value.IsHiDef),
	VideoStandard(value.VideoStandard)
{
}

}

#endif 
