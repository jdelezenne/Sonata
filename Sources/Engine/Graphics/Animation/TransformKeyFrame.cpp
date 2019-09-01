/*=============================================================================
TransformKeyFrame.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "TransformKeyFrame.h"

namespace SonataEngine
{

TransformKeyFrame::TransformKeyFrame() :
	KeyFrame(),
	_transformType(TransformKeyFrameType_All),
	_translation(Vector3::Zero),
	_rotation(Quaternion::Identity),
	_scale(Vector3::One)
{
}

TransformKeyFrame::TransformKeyFrame(const TimeValue& timeValue) :
	KeyFrame(timeValue),
	_transformType(TransformKeyFrameType_All),
	_translation(Vector3::Zero),
	_rotation(Quaternion::Identity),
	_scale(Vector3::One)
{
}

TransformKeyFrame::~TransformKeyFrame()
{
}

}
