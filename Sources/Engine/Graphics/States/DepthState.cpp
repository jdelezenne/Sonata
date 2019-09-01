/*=============================================================================
DepthState.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "DepthState.h"

namespace SonataEngine
{

SE_IMPLEMENT_STRUCT(DepthState);

DepthState::DepthState() :
	Enable(true),
	WriteEnable(true),
	Function(ComparisonFunction_LessEqual),
	DepthBias(0),
	DepthBiasClamp(0.0f),
	SlopeScaledDepthBias(0.0f)
{
}

DepthState::DepthState(bool enable, bool writeEnable, ComparisonFunction function, int32 depthBias) :
	Enable(enable),
	WriteEnable(writeEnable),
	Function(function),
	DepthBias(depthBias),
	DepthBiasClamp(0.0f),
	SlopeScaledDepthBias(0.0f)
{
}

}
