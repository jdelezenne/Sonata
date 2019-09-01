/*=============================================================================
ViewAxisShape.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SFW_VIEWAXISSHAPE_H_
#define _SFW_VIEWAXISSHAPE_H_

#include "SampleFramework/Common.h"
#include "SampleFramework/Visualizers/AxisShape.h"

namespace SampleFramework
{

enum AxisLocation
{
	AxisLocation_BottomLeft,
	AxisLocation_BottomRight,
	AxisLocation_TopLeft,
	AxisLocation_TopRight,
};

class SE_SAMPLEFRAMEWORK_EXPORT ViewAxisShape
{
protected:
	AxisShape* _shape;
	Camera* _camera;
	AxisLocation _axisLocation;
	int _screenSize;

public:
	ViewAxisShape();

	virtual ~ViewAxisShape();

	int GetScreenSize() const { return _screenSize; }
	void SetScreenSize(int value) { _screenSize = value; }

	Camera* GetCamera() const { return _camera; }
	void SetCamera(Camera* value) { _camera = value; }

	AxisShape* GetShape() const { return _shape; }
	void SetShape(AxisShape* value) { _shape = value; }

	void Render();
};

}

#endif
