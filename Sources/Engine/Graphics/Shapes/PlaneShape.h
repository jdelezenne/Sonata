/*=============================================================================
PlaneShape.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_PLANESHAPE_H_
#define _SE_PLANESHAPE_H_

#include "Graphics/Shapes/Shape.h"
#include "Core/Math/Plane.h"

namespace SonataEngine
{

/** Plane shape. */
class SE_GRAPHICS_EXPORT PlaneShape : public Shape
{
public:
	/** @name Constructors. */
	//@{
	/** Default constructor. */
	PlaneShape();

	/**
		Initialize a new plane shape from a plane.
		@param plane The plane.
	*/
	PlaneShape(const Plane& plane);

	/** Copy constructor. */
	PlaneShape(const PlaneShape& value);

	/** Destructor. */
	virtual ~PlaneShape();
	//@}

	const Plane& GetPlane() const { return _Plane; }
	void SetPlane(const Plane& value) { _Plane = value; }

	const Vector2& GetScale() const { return _Scale; }
	void SetScale(const Vector2& value) { _Scale = value; }

	int32 GetDivisions() const { return _Divisions; }
	void SetDivisions(int32 value) { _Divisions = Math::Max(1, value); }

	virtual bool CreateMesh(ShapeDescription* desc);

public:
	Plane _Plane;
	Vector2 _Scale;
	int32 _Divisions;
};

}

#endif 
