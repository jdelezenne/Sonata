/*=============================================================================
AxisShape.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SFW_AXISSHAPE_H_
#define _SFW_AXISSHAPE_H_

#include "SampleFramework/Common.h"

namespace SampleFramework
{

class SE_SAMPLEFRAMEWORK_EXPORT AxisShape
{
protected:
	struct AxisDesc
	{
		bool _isEnabled;
		Color32 _color;
		String _label;
	};

	MeshPtr _mesh;
	real32 _axisSize;
	real32 _labelSize;
	AxisDesc _desc[3];

public:
	AxisShape();

	virtual ~AxisShape();

	Mesh* GetMesh() const { return _mesh; }

	real32 GetAxisSize() const { return _axisSize; }
	void SetAxisSize(real32 value) { _axisSize = value; }

	real32 GetLabelSize() const { return _labelSize; }
	void SetLabelSize(real32 value) { _labelSize = value; }

	bool GetAxisEnabled(Axes::Type axis) const
	{
		SE_ASSERT(axis >= 0 && axis < 3);
		return _desc[axis]._isEnabled;
	}

	void SetAxisEnabled(Axes::Type axis, bool value)
	{
		SE_ASSERT(axis >= 0 && axis < 3);
		_desc[axis]._isEnabled = value;
	}

	const Color32& GetAxesColor(Axes::Type axis, const Color32& value) const
	{
		SE_ASSERT(axis >= 0 && axis < 3);
		return _desc[axis]._color;
	}

	void SetAxisColor(Axes::Type axis, const Color32& value)
	{
		SE_ASSERT(axis >= 0 && axis < 3);
		_desc[axis]._color = value;
	}

	const String& GetAxisLabel(Axes::Type axis, const String& value) const
	{
		SE_ASSERT(axis >= 0 && axis < 3);
		return _desc[axis]._label;
	}

	void SetAxisLabel(Axes::Type axis, const String& value)
	{
		SE_ASSERT(axis >= 0 && axis < 3);
		_desc[axis]._label = value;
	}

	void Build();
};

}

#endif
