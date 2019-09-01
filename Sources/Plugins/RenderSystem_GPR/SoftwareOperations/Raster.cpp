/*=============================================================================
Raster.cpp
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#include "Software/SoftwareRenderer.h"

bool SoftwareRenderer::Raster(OGPRFragmentFuncEnv& env, Colour32& colour)
{
	Vector4 position;
	real32 depth;
	Colour32 destination;

	env.ReadInterpolant(0, (real32*)&position);
	env.GetDepth(&depth);
#ifndef ARGB_COLOUR_BUFFER
	env.ReadPixel(DBT_ColourBuffer, &destination);
#else
	uint32 pixel;
	env.ReadPixel(DBT_ColourBuffer, &pixel);
	destination = Colour32::FromARGB(pixel);
#endif

	if (!ScissorTest(env, position))
		return false;

	if (!AlphaTest(env, colour.alpha))
		return false;

	if (!StencilTest(env))
		return false;

	if (!DepthTest(env, depth))
		return false;

	PixelFog(depth, colour);
	Blending(destination, colour);
	Masking(colour);

	return true;
}
