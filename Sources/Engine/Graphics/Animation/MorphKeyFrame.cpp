/*=============================================================================
MorphKeyFrame.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "MorphKeyFrame.h"

namespace SonataEngine
{

MorphKeyFrame::MorphKeyFrame() :
	KeyFrame(),
	_vertexBuffer(NULL)
{
}

MorphKeyFrame::MorphKeyFrame(const TimeValue& timeValue) :
	KeyFrame(timeValue),
	_vertexBuffer(NULL)
{
}

MorphKeyFrame::~MorphKeyFrame()
{
}

}
