/*=============================================================================
NodeAnimationTrack.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "NodeAnimationTrack.h"

namespace SonataEngine
{

NodeAnimationTrack::NodeAnimationTrack() :
	AnimationTrack(),
	_node(NULL)
{
}

NodeAnimationTrack::~NodeAnimationTrack()
{
}

TransformKeyFrame* NodeAnimationTrack::GetNodeKeyFrame(int index) const
{
	return (TransformKeyFrame*)GetKeyFrameByIndex(index);
}

void NodeAnimationTrack::GetTranslationKeys(BaseArray<KeyVector3>& translationKeys)
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

void NodeAnimationTrack::GetRotationKeys(BaseArray<KeyQuaternion>& rotationKeys)
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

void NodeAnimationTrack::GetScaleKeys(BaseArray<KeyVector3>& scaleKeys)
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

KeyFrame* NodeAnimationTrack::_CreateKeyFrame(const TimeValue& timeValue)
{
	return new TransformKeyFrame(timeValue);
}

void NodeAnimationTrack::Update(const TimeValue& timeValue)
{
	KeyFrame* keyA = NULL;
	KeyFrame* keyB = NULL;
	GetKeyFramesAtTime(timeValue, &keyA, &keyB);

	if (keyA == NULL || keyB == NULL)
	{
		return;
	}

	if (_node != NULL)
	{
		TransformKeyFrame* key = (TransformKeyFrame*)keyA;
		if ((key->GetTransformType() & TransformKeyFrameType_Translation) != 0)
		{
			Vector3 translation = key->GetTranslation();
			_node->SetLocalPosition(translation);
		}
		if ((key->GetTransformType() & TransformKeyFrameType_Rotation) != 0)
		{
			Quaternion rotation = key->GetRotation();
			_node->SetLocalOrientation(rotation);
		}
		if ((key->GetTransformType() & TransformKeyFrameType_Scale) != 0)
		{
			Vector3 scale = key->GetScale();
			_node->SetLocalScale(scale);
		}

		//Matrix4 transform = Matrix4::CreateScale(scale);
		//transform *= Matrix4::CreateFromQuaternion(rotation);
		//transform.SetTranslation(translation);
		//_node->SetLocalTransform(transform);
	}
}

}
