/*=============================================================================
AlphaState.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "AlphaState.h"

namespace SonataEngine
{

SE_IMPLEMENT_STRUCT(AlphaState);

AlphaState::AlphaState() :
	TestEnable(false),
	Reference(0),
	Function(ComparisonFunction_Always),
	SourceBlend(BlendMode_One),
	DestinationBlend(BlendMode_Zero),
	BlendOp(BlendOperation_Add),
	SourceBlendAlpha(BlendMode_One),
	DestinationBlendAlpha(BlendMode_Zero),
	BlendOperationAlpha(BlendOperation_Add),
	BlendFactor(0xffffffff)
{
	Memory::Set(RenderTargetWriteMask, ColorFlag_All, MaxRenderTargets * sizeof(ColorFlag));
	Memory::Set(BlendEnable, false, MaxRenderTargets * sizeof(bool));
}

}
