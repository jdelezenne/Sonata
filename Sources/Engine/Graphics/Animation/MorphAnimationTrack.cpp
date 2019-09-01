/*=============================================================================
MorphAnimationTrack.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "MorphAnimationTrack.h"

namespace SonataEngine
{

MorphAnimationTrack::MorphAnimationTrack() :
	AnimationTrack(),
	_vertexData(NULL)
{
}

MorphAnimationTrack::~MorphAnimationTrack()
{
	SE_DELETE(_vertexData);
}

MorphKeyFrame* MorphAnimationTrack::GetMorphKeyFrame(int index) const
{
	return (MorphKeyFrame*)GetKeyFrameByIndex(index);
}

KeyFrame* MorphAnimationTrack::_CreateKeyFrame(const TimeValue& timeValue)
{
	return new MorphKeyFrame(timeValue);
}

void MorphAnimationTrack::Update(const TimeValue& timeValue)
{
	if (_vertexData == NULL)
	{
		return;
	}

	VertexLayout* vertexLayout = _vertexData->VertexLayout;
	const VertexElement* positionElement = vertexLayout->GetElementBySemantic(VertexSemantic_Position);
	const VertexElement* normalElement = vertexLayout->GetElementBySemantic(VertexSemantic_Normal);
	if (positionElement == NULL || normalElement == NULL)
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

	MorphKeyFrame* key1 = (MorphKeyFrame*)keyA;
	MorphKeyFrame* key2 = (MorphKeyFrame*)keyB;

	uint32 stride = _vertexData->VertexStreams[0].Stride;
	uint16 positionOffset = positionElement->GetOffset();
	uint16 normalOffset = normalElement->GetOffset();

	HardwareBuffer* vertexBuffer1 = key1->GetVertexBuffer();
	HardwareBuffer* vertexBuffer2 = key2->GetVertexBuffer();
	HardwareBuffer* vertexBufferOut = _vertexData->VertexStreams[0].VertexBuffer;

	SEbyte* vbData1;
	SEbyte* vbData2;
	SEbyte* vbDataOut;

	vertexBuffer1->Map(HardwareBufferMode_ReadOnly, (void**)&vbData1);
	vertexBuffer2->Map(HardwareBufferMode_ReadOnly, (void**)&vbData2);
	vertexBufferOut->Map(HardwareBufferMode_WriteOnly, (void**)&vbDataOut);

	for (uint i = 0; i < _vertexData->VertexCount; ++i)
	{
		Vector3 position1 = *(Vector3*)(vbData1 + positionOffset);
		Vector3 position2 = *(Vector3*)(vbData2 + positionOffset);

		Vector3 normal1 = *(Vector3*)(vbData1 + normalOffset);
		Vector3 normal2 = *(Vector3*)(vbData2 + normalOffset);

		Vector3 positionOut = Vector3::Lerp(position1, position2, t);
		Vector3 normalOut = Vector3::Lerp(normal1, normal2, t);

		*(Vector3*)(vbDataOut + positionOffset) = positionOut;
		*(Vector3*)(vbDataOut + normalOffset) = normalOut;

		vbData1 += stride;
		vbData2 += stride;
		vbDataOut += stride;
	}

	vertexBuffer1->Unmap();
	vertexBuffer2->Unmap();
	vertexBufferOut->Unmap();
}

}
