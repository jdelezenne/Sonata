/*=============================================================================
ScreenQuadShape.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SFW_SCREENQUADSHAPE_H_
#define _SFW_SCREENQUADSHAPE_H_

#include "SampleFramework/Common.h"

namespace SampleFramework
{

class SE_SAMPLEFRAMEWORK_EXPORT ScreenQuadShape
{
protected:
	MeshPtr _mesh;

public:
	ScreenQuadShape();

	virtual ~ScreenQuadShape();

	Mesh* GetMesh() const { return _mesh; }

	void Build();
};

}

#endif
