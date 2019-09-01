/*=============================================================================
BoxShape.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_BOXSHAPE_H_
#define _SE_BOXSHAPE_H_

#include "Graphics/Shapes/Shape.h"
#include "Core/Math/BoundingBox.h"

namespace SonataEngine
{

/** Box shape. */
class SE_GRAPHICS_EXPORT BoxShape : public Shape
{
public:
	/** @name Constructors. */
	//@{
	/** Default constructor. */
	BoxShape();

	/**
		Initialize a new box shape from a box.
		@param box The box.
	*/
	BoxShape(const BoundingBox& box);

	/** Copy constructor. */
	BoxShape(const BoxShape& value);

	/** Destructor. */
	virtual ~BoxShape();
	//@}

	const BoundingBox& GetBox() const { return _Box; }
	void SetBox(const BoundingBox& value) { _Box = value; }

	int32 GetDivisions() const { return _Divisions; }
	void SetDivisions(int32 value) { _Divisions = Math::Min(1, value); }

	virtual bool CreateMesh(ShapeDescription* desc);

public:
	BoundingBox _Box;
	int32 _Divisions;
};

}

#endif 
