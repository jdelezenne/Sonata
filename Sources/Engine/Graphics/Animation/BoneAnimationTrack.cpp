/*=============================================================================
BoneAnimationTrack.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "BoneAnimationTrack.h"

namespace SonataEngine
{

BoneAnimationTrack::BoneAnimationTrack() :
	AnimationTrack(),
	_bone(NULL)
{
}

BoneAnimationTrack::~BoneAnimationTrack()
{
}

TransformKeyFrame* BoneAnimationTrack::GetBoneKeyFrame(int index) const
{
	return (TransformKeyFrame*)GetKeyFrameByIndex(index);
}

void BoneAnimationTrack::GetTranslationKeys(BaseArray<KeyVector3>& translationKeys)
{
	KeyFrameList::Iterator it = _keyFrames.GetIterator();
	while (it.Next())
	{
		TransformKeyFrame* keyFrame = (TransformKeyFrame*)it.Current();
		if ((keyFrame->GetTransformType() & TransformKeyFrameType_Translation) != 0)
		{
			translationKeys.Add(KeyVector3(keyFrame->GetTime(), keyFrame->GetTranslation()));
		}
	}
}

void BoneAnimationTrack::GetRotationKeys(BaseArray<KeyQuaternion>& rotationKeys)
{
	KeyFrameList::Iterator it = _keyFrames.GetIterator();
	while (it.Next())
	{
		TransformKeyFrame* keyFrame = (TransformKeyFrame*)it.Current();
		if ((keyFrame->GetTransformType() & TransformKeyFrameType_Rotation) != 0)
		{
			rotationKeys.Add(KeyQuaternion(keyFrame->GetTime(), keyFrame->GetRotation()));
		}
	}
}

void BoneAnimationTrack::GetScaleKeys(BaseArray<KeyVector3>& scaleKeys)
{
	KeyFrameList::Iterator it = _keyFrames.GetIterator();
	while (it.Next())
	{
		TransformKeyFrame* keyFrame = (TransformKeyFrame*)it.Current();
		if ((keyFrame->GetTransformType() & TransformKeyFrameType_Scale) != 0)
		{
			scaleKeys.Add(KeyVector3(keyFrame->GetTime(), keyFrame->GetScale()));
		}
	}
}

KeyFrame* BoneAnimationTrack::_CreateKeyFrame(const TimeValue& timeValue)
{
	return new TransformKeyFrame(timeValue);
}

void BoneAnimationTrack::Update(const TimeValue& timeValue)
{
	if (_bone == NULL)
	{
		return;
	}

	KeyFrame* keyA = NULL;
	KeyFrame* keyB = NULL;
	TimeValue delta = GetKeyFramesAtTime(timeValue, &keyA, &keyB);

	if (keyA == NULL || keyB == NULL)
	{
		return;
	}

	real64 t = 0.0;
	if (timeValue != keyA->GetTime())
	{
		t = (real64)(delta / (timeValue - keyA->GetTime()));
	}

	TransformKeyFrame* key1 = (TransformKeyFrame*)keyA;
	TransformKeyFrame* key2 = (TransformKeyFrame*)keyB;

	if ((key1->GetTransformType() & TransformKeyFrameType_Translation) != 0 &&
		(key2->GetTransformType() & TransformKeyFrameType_Translation) != 0)
	{
		Vector3 translation = Vector3::Lerp(key1->GetTranslation(), key2->GetTranslation(), t);
		_bone->SetLocalPosition(translation);
	}
	if ((key1->GetTransformType() & TransformKeyFrameType_Rotation) != 0 &&
		(key2->GetTransformType() & TransformKeyFrameType_Rotation) != 0)
	{
		Quaternion rotation = Quaternion::Slerp(key1->GetRotation(), key2->GetRotation(), t);
		_bone->SetLocalOrientation(rotation);
	}
	if ((key1->GetTransformType() & TransformKeyFrameType_Scale) != 0 &&
		(key2->GetTransformType() & TransformKeyFrameType_Scale) != 0)
	{
		Vector3 scale = Vector3::Lerp(key1->GetScale(), key2->GetScale(), t);
		_bone->SetLocalScale(scale);
	}
}

}
