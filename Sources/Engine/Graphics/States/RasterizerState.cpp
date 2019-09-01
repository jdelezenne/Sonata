/*=============================================================================
RasterizerState.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "RasterizerState.h"

namespace SonataEngine
{

SE_IMPLEMENT_STRUCT(RasterizerState);

RasterizerState::RasterizerState() :
	FillMode(FillMode_Solid),
	CullMode(CullMode_Back),
	FrontCounterClockwise(false),
	MultisampleEnable(false),
	AntialiasedLineEnable(false)
{
}

}
