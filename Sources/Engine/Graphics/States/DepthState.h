/*=============================================================================
DepthState.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_DEPTHSTATE_H_
#define _SE_DEPTHSTATE_H_

#include "Graphics/Common.h"

namespace SonataEngine
{

/**
	@brief Depth state.

	@remarks
		The DepthBias, DepthBiasClamp, and SlopeScaledDepthBias members
		are all part of the following equation:
		PixelDepth = max(DepthBiasClamp, PixelDepth + DepthBias + Slope * SlopeScaledDepthBias)
*/
class SE_GRAPHICS_EXPORT DepthState
{
public:
	/** @name Depth Testing */
	//@{
	/// Enables or disables depth buffering.
	bool Enable;

	/// Enables or disables writing to the depth buffer.
	bool WriteEnable;

	/// Retrieves or sets the comparison function for the z-buffer test.
	ComparisonFunction Function; 

	/// Retrieves or sets the depth value added to a given pixel.
	int32 DepthBias;

	/// Retrieves or sets the maximum depth of a pixel.
	real32 DepthBiasClamp;

	/// Retrieves or sets the scalar on a given pixel's slope.
	real32 SlopeScaledDepthBias;
	//@}

public:
	/** @name Constructor. */
	//@{
	DepthState();
	DepthState(bool enable, bool writeEnable, ComparisonFunction function, int32 depthBias);
	//@}
};

SE_DECLARE_STRUCT(DepthState);
SE_BEGIN_STRUCT(DepthState);
	SE_StructField(Enable, bool);
	SE_StructField(WriteEnable, bool);
	SE_StructField(Function, ComparisonFunction);
	SE_StructField(DepthBias, int32);
	SE_StructField(DepthBiasClamp, real32);
	SE_StructField(SlopeScaledDepthBias, real32);
SE_END_STRUCT(DepthState);

}

#endif
