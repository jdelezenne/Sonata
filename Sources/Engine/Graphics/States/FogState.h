/*=============================================================================
FogState.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_FOGSTATE_H_
#define _SE_FOGSTATE_H_

#include "Graphics/Common.h"

namespace SonataEngine
{

/** Fog state. */
class SE_GRAPHICS_EXPORT FogState
{
public:
	/** @name Fog. */
	//@{
	/// Enables or disables fog blending.
	bool Enable;

	/// Retrieves or sets the fog color.
	Color32 Color;

	/// Retrieves or sets the fog density for pixel or vertex fog used in exponential fog modes.
	real32 Density;

	/// Retrieves or sets the depth at which pixel or vertex fog effects begin for linear fog mode.
	real32 Start;

	/// Retrieves or sets the depth at which pixel or vertex fog effects end for linear fog mode.
	real32 End;

	/// Enables or disables range-based vertex fog.
	bool RangeEnable;

	/// Retrieves or sets the fog formula to use for vertex fog.
	FogMode VertexMode;

	/// Retrieves or sets the fog formula to use for pixel fog.
	FogMode PixelMode;
	//@}

public:
	/** @name Constructor. */
	//@{
	FogState();
	FogState(bool enable, const Color32& color, real32 density, real32 start,
		real32 end, bool rangeEnable = false,
		FogMode vertexMode = FogMode_None, FogMode pixelMode = FogMode_None);
	//@}
};

SE_DECLARE_STRUCT(FogState);
SE_BEGIN_STRUCT(FogState);
	SE_StructField(Enable, bool);
	SE_StructField(Color, Color32);
	SE_StructField(Density, real32);
	SE_StructField(Start, real32);
	SE_StructField(End, real32);
	SE_StructField(RangeEnable, bool);
	SE_StructField(VertexMode, FogMode);
	SE_StructField(PixelMode, FogMode);
SE_END_STRUCT(FogState);

}

#endif
