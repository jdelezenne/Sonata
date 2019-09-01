/*=============================================================================
QuadShape.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SFW_QUADSHAPE_H_
#define _SFW_QUADSHAPE_H_

#include "SampleFramework/Common.h"

namespace SampleFramework
{

class SE_SAMPLEFRAMEWORK_EXPORT QuadShape
{
protected:
	MeshPtr _mesh;
	real32 _quadSize;
	Color32 _color
		;
public:
	QuadShape();

	virtual ~QuadShape();

	Mesh* GetMesh() const { return _mesh; }

	real32 GetQuadSize() const { return _quadSize; }
	void SetQuadSize(real32 value) { _quadSize = value; }

	const Color32& GetColor(const Color32& value) const { return _color; }
	void SetColor(const Color32& value) { _color = value; }

	void Build();
};

}

#endif
