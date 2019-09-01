/*=============================================================================
Skin.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SKIN_H_
#define _SE_SKIN_H_

#include "Graphics/Common.h"
#include "Graphics/System/RenderData.h"

namespace SonataEngine
{

class Skeleton;

enum SkinningMethod
{
	SkinningMethod_Software,
	SkinningMethod_Blending,
	SkinningMethod_Hardware
};

/** Bone Influence. */
class SE_GRAPHICS_EXPORT BoneInfluence
{
public:
	uint32 VertexIndex;
	real32 Weight;
};

/** Skin Vertex. */
class SE_GRAPHICS_EXPORT SkinVertex
{
public:
	uint16 BoneIndex;
	BaseArray<BoneInfluence> BoneInfluences;
};

/** Skin. */
class SE_GRAPHICS_EXPORT Skin : public RefObject
{
protected:
	VertexData* _sourceVertexData;
	VertexData* _destVertexData;
	int _boneCount;
	Matrix4* _boneMatrixPalette;

public:
	BaseArray<SkinVertex> SkinVertices;
	SkinningMethod SkinningMethod;

public:
	Skin();
	virtual ~Skin();

	void Initialize(VertexData* vertexData);
	void Update(Skeleton* skeleton);
	void Deform();

protected:
	void ProcessWeights(VertexFormat format, SEbyte* weightsIn, real32* weightsOut);
	void UpdateVertexBuffer(int streamIndex, HardwareBuffer* vertexBuffer);
};

}

#endif
