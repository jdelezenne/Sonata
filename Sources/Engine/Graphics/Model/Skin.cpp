/*=============================================================================
Skin.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Skin.h"
#include "Graphics/System/RenderSystem.h"
#include "Graphics/Model/Skeleton.h"

namespace SonataEngine
{

Skin::Skin() :
	RefObject(),
	_sourceVertexData(NULL),
	_destVertexData(NULL),
	_boneCount(0),
	_boneMatrixPalette(NULL),
	SkinningMethod(SkinningMethod_Software)
{
}

Skin::~Skin()
{
}

void Skin::Initialize(VertexData* vertexData)
{
	RenderSystem* renderSystem = RenderSystem::Current();

	if (SkinningMethod != SkinningMethod_Software)
	{
		return;
	}

	_destVertexData = vertexData;

	_sourceVertexData = new VertexData();
	_sourceVertexData->VertexLayout = vertexData->VertexLayout;
	_sourceVertexData->VertexCount = vertexData->VertexCount;

	for (int i = 0; i < vertexData->VertexStreams.Count(); ++i)
	{
		HardwareBuffer* destBuffer = vertexData->VertexStreams[i].VertexBuffer;
		uint32 stride = vertexData->VertexStreams[i].Stride;

		HardwareBuffer* sourceBuffer;
		if (!renderSystem->CreateVertexBuffer(vertexData->VertexCount * stride, HardwareBufferUsage_Dynamic, &sourceBuffer))
		{
			_sourceVertexData->VertexStreams.Add(VertexStream(NULL, stride));
			continue;
		}
		_sourceVertexData->VertexStreams.Add(VertexStream(sourceBuffer, stride));

		SEbyte* vertices;
		SEbyte* sourceVertices;
		destBuffer->Map(HardwareBufferMode_ReadOnly, (void**)&vertices);
		sourceBuffer->Map(HardwareBufferMode_WriteOnly, (void**)&sourceVertices);

		Memory::Copy(sourceVertices, vertices, vertexData->VertexCount * stride);

		destBuffer->Unmap();
		sourceBuffer->Unmap();
	}
}

void Skin::Update(Skeleton* skeleton)
{
	int i;
	int boneCount = SkinVertices.Count();
	if (boneCount == 0)
	{
		return;
	}

	if (_boneCount != boneCount)
	{
		SE_DELETE_ARRAY(_boneMatrixPalette);
		_boneCount = boneCount;
	}
	if (_boneMatrixPalette == NULL)
	{
		_boneMatrixPalette = new Matrix4[boneCount];
	}

	for (i = 0; i < boneCount; ++i)
	{
		Bone* bone = skeleton->GetBoneByIndex(SkinVertices[i].BoneIndex);
		_boneMatrixPalette[i] = bone->GetSkinTransform();
	}
}

void Skin::Deform()
{
	if (SkinningMethod != SkinningMethod_Software)
	{
		return;
	}

	if (_sourceVertexData == NULL)
	{
		return;
	}

	int i;
	for (i = 0; i < _destVertexData->VertexStreams.Count(); ++i)
	{
		HardwareBuffer* destBuffer = _destVertexData->VertexStreams[i].VertexBuffer;
		UpdateVertexBuffer(i, destBuffer);
	}
}

void Skin::ProcessWeights(VertexFormat format, SEbyte* weightsIn, real32* weightsOut)
{
	if (format == VertexFormat_Float4)
	{
		weightsOut[0] = ((real32*)weightsIn)[0];
		weightsOut[1] = ((real32*)weightsIn)[1];
		weightsOut[2] = ((real32*)weightsIn)[2];
		weightsOut[3] = ((real32*)weightsIn)[3];
	}
	else if (format == VertexFormat_UByte4N)
	{
		weightsOut[0] = weightsIn[0] / 255.0f;
		weightsOut[1] = weightsIn[1] / 255.0f;
		weightsOut[2] = weightsIn[2] / 255.0f;
		weightsOut[3] = weightsIn[3] / 255.0f;
	}
}

void Skin::UpdateVertexBuffer(int streamIndex, HardwareBuffer* destBuffer)
{
	VertexLayout* vertexLayout = _sourceVertexData->VertexLayout;
	uint32 vertexCount = _sourceVertexData->VertexCount;
	HardwareBuffer* sourceBuffer = _sourceVertexData->VertexStreams[streamIndex].VertexBuffer;
	uint32 stride = _sourceVertexData->VertexStreams[streamIndex].Stride;

	const VertexElement* positionElement = vertexLayout->GetElementBySemantic(VertexSemantic_Position);
	const VertexElement* blendWeightsElement = vertexLayout->GetElementBySemantic(VertexSemantic_BlendWeight);
	const VertexElement* blendIndicesElement = vertexLayout->GetElementBySemantic(VertexSemantic_BlendIndices);
	const VertexElement* normalElement = vertexLayout->GetElementBySemantic(VertexSemantic_Normal);
	const VertexElement* tangentElement = vertexLayout->GetElementBySemantic(VertexSemantic_Tangent);

	if (positionElement == NULL || blendWeightsElement == NULL ||
		blendIndicesElement == NULL || normalElement == NULL)
	{
		return;
	}

	uint16 positionOffset = positionElement->GetOffset();
	uint16 blendWeightsOffset = blendWeightsElement->GetOffset();
	uint16 blendIndicesOffset = blendIndicesElement->GetOffset();
	uint16 normalOffset = normalElement->GetOffset();
	VertexFormat weightFormat = blendWeightsElement->GetVertexFormat();

	SEbyte* sourceVertices;
	sourceBuffer->Map(HardwareBufferMode_ReadOnly, (void**)&sourceVertices);

	SEbyte* destVertices;
	destBuffer->Map(HardwareBufferMode_WriteOnly, (void**)&destVertices);

	for (uint i = 0; i < vertexCount; ++i)
	{
		Vector3 position = *(Vector3*)(sourceVertices + positionOffset);
		real32 blendWeights[4];
		ProcessWeights(weightFormat, sourceVertices + blendWeightsOffset, (real32*)blendWeights);
		uint8* blendIndices = (uint8*)(sourceVertices + blendIndicesOffset);
		Vector3 normal = *(Vector3*)(sourceVertices + normalOffset);

		Vector3 transformedPos = Vector3::Zero;
		Vector3 transformedNormal = Vector3::Zero;
		for (int b = 0; b < 4; ++b)
		{
			if (blendWeights[b] > Math::Epsilon)
			{
				Matrix4 matBone = _boneMatrixPalette[blendIndices[b]];
				transformedPos += blendWeights[b] * Vector3::Transform(position, matBone);
				transformedNormal += blendWeights[b] * Vector3::TransformNormal(normal, matBone);
			}
		}

		*(Vector3*)(destVertices + positionOffset) = transformedPos;
		*(Vector3*)(destVertices + normalOffset) = transformedNormal;

		sourceVertices += stride;
		destVertices += stride;
	}

	destBuffer->Unmap();
	sourceBuffer->Unmap();
}

}
