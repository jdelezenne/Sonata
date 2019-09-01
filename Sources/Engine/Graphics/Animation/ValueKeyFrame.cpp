/*=============================================================================
ValueKeyFrame.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ValueKeyFrame.h"

namespace SonataEngine
{

ValueKeyFrame::ValueKeyFrame() :
	KeyFrame()
{
}

ValueKeyFrame::ValueKeyFrame(const TimeValue& timeValue) :
	KeyFrame(timeValue)
{
}

ValueKeyFrame::~ValueKeyFrame()
{
}

}
