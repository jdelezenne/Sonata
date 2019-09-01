/*=============================================================================
Masking.cpp
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#include "Software/SoftwareRenderer.h"

void SoftwareRenderer::Masking(Colour32& colour)
{
	if ((States.ColourWriteEnable & CF_Red) == 0)
		colour.red = 0.0f;
	if ((States.ColourWriteEnable & CF_Green) == 0)
		colour.green = 0.0f;
	if ((States.ColourWriteEnable & CF_Blue) == 0)
		colour.blue = 0.0f;
	if ((States.ColourWriteEnable & CF_Alpha) == 0)
		colour.alpha = 0.0f;
}
