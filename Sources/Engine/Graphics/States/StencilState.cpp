/*=============================================================================
StencilState.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "StencilState.h"

namespace SonataEngine
{

SE_IMPLEMENT_STRUCT(DepthStencilOperation);

SE_IMPLEMENT_STRUCT(StencilState);

StencilState::StencilState() :
	Enable(false),
	Reference(0),
	ReadMask(0xffffffff),
	WriteMask(0xffffffff)
{
	FrontFace.Fail = StencilOperation_Keep;
	FrontFace.DepthBufferFail = StencilOperation_Keep;
	FrontFace.Pass = StencilOperation_Keep;
	FrontFace.Function = ComparisonFunction_Always;

	BackFace.Fail = StencilOperation_Keep;
	BackFace.DepthBufferFail = StencilOperation_Keep;
	BackFace.Pass = StencilOperation_Keep;
	BackFace.Function = ComparisonFunction_Always;
}

}
