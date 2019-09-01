/*=============================================================================
PointState.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "PointState.h"

namespace SonataEngine
{

SE_IMPLEMENT_STRUCT(PointState);

PointState::PointState() :
	Enable(false),
	Size(1.0f),
	MinSize(1.0f),
	MaxSize(64.0f),
	ScaleEnable(false),
	ConstantScale(1.0f),
	LinearScale(0.0f),
	QuadraticScale(0.0f)
{
}

}
