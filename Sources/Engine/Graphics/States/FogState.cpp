/*=============================================================================
FogState.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "FogState.h"

namespace SonataEngine
{

SE_IMPLEMENT_STRUCT(FogState);

FogState::FogState() :
	Enable(false),
	Color(Color32::White),
	Density(1.0f),
	Start(0.0f),
	End(1.0f),
	VertexMode(FogMode_None),
	PixelMode(FogMode_None)
{
}

FogState::FogState(bool enable, const Color32& color, real32 density,
	real32 start, real32 end, bool rangeEnable, FogMode vertexMode,
	FogMode pixelMode) :
	Enable(enable),
	Color(color),
	Density(density),
	Start(start),
	End(end),
	RangeEnable(rangeEnable),
	VertexMode(vertexMode),
	PixelMode(pixelMode)
{
}

}
