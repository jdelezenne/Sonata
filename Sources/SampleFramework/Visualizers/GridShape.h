/*=============================================================================
GridShape.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SFW_GRIDSHAPE_H_
#define _SFW_GRIDSHAPE_H_

#include "SampleFramework/Common.h"

namespace SampleFramework
{

enum GridStyle
{
	GridStyle_line,
	GridStyle_dot
};

enum GridPlane
{
	GridPlane_XY = (1<<0),
	GridPlane_YZ = (1<<1),
	GridPlane_ZX = (1<<2)
};

class SE_SAMPLEFRAMEWORK_EXPORT GridShape
{
protected:
	ModelPtr _model;
	GridStyle _gridStyle;
	GridPlane _gridPlanes;
	int32 _gridSize;
	real32 _gridSpacing;
	Color32 _color;
	real32 _breakUnit;
	real32 _highlightBreak;

public:
	GridShape();

	virtual ~GridShape();

	Model* GetModel() const { return _model; }

	GridStyle GetGridStyle() const { return _gridStyle; }
	void SetGridStyle(GridStyle value) { _gridStyle = value; }

	GridPlane GetGridPlanes() const { return _gridPlanes; }
	void SetGridPlanes(GridPlane value) { _gridPlanes = value; }

	int32 GetGridSize() const { return _gridSize; }
	void SetGridSize(int32 value) { _gridSize = value; }

	real32 GetGridSpacing() const { return _gridSpacing; }
	void SetGridSpacing(real32 value) { _gridSpacing = value; }

	const Color32& GetColor() const { return _color; }
	void SetColor(const Color32& value) { _color = value; }

	real32 GetBreakUnit() const { return _breakUnit; }
	void SetBreakUnit(real32 value) { _breakUnit = value; }

	real32 GetHighlightBreak() const { return _highlightBreak; }
	void SetHighlightBreak(real32 value) { _highlightBreak = value; }

	void Build();

	void Render();
};

}

#endif
