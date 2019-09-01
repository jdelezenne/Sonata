/*=============================================================================
SkeletonShape.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SFW_SKELETONSHAPE_H_
#define _SFW_SKELETONSHAPE_H_

#include "SampleFramework/Common.h"

namespace SampleFramework
{

class SE_SAMPLEFRAMEWORK_EXPORT SkeletonShape
{
protected:
	MeshPtr _mesh;
	Skeleton* _skeleton;
	Skin* _skin;
	Color32 _color;

public:
	SkeletonShape();

	virtual ~SkeletonShape();

	void Build();
	void Update();

	Mesh* GetMesh() const { return _mesh; }

	Skeleton* GetSkeleton() const { return _skeleton; }
	void SetSkeleton(Skeleton* value) { _skeleton = value; }

	Skin* GetSkin() const { return _skin; }
	void SetSkin(Skin* value) { _skin = value; }

	const Color32& GetColor(const Color32& value) const { return _color; }
	void SetColor(const Color32& value) { _color = value; }

protected:
	void BuildBone(Bone* bone, SEbyte** vertices);
};

}

#endif
