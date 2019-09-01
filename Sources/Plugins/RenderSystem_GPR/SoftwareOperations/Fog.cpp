/*=============================================================================
Fog.cpp
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#include "Software/SoftwareRenderer.h"

void SoftwareRenderer::PixelFog(real32 depth, Colour32& colour)
{
	if (!States.FogState.Enable)
		return;

	real32 distance = depth;

	real32 fog;
	switch (States.FogState.PixelMode)
	{
	case FM_None:
		fog = 1.0f;
		break;
	case FM_Linear:
		fog = Clamp((States.FogState.End - distance) /
			(States.FogState.End - States.FogState.Start), 0.0f, 1.0f);
		break;
	case FM_Exponential:
		fog = 1.0f / exp(distance * States.FogState.Density);
		break;
	case FM_Exponential2:
		fog = 1.0f / exp(pow(distance * States.FogState.Density, 2.0f));
		break;
	}

	colour *= fog;
	colour.Clamp(0.0f, 1.0f);
}
