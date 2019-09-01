/*=============================================================================
SphereShape.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SPHERESHAPE_H_
#define _SE_SPHERESHAPE_H_

#include "Graphics/Shapes/Shape.h"
#include "Core/Math/BoundingSphere.h"

namespace SonataEngine
{

/** Sphere shape. */
class SE_GRAPHICS_EXPORT SphereShape : public Shape
{
public:
	/** @name Constructors. */
	//@{
	/** Default constructor. */
	SphereShape();

	/**
		Initialize a new sphere shape from a sphere.
		@param sphere The sphere.
	*/
	SphereShape(const BoundingSphere& sphere);

	/** Copy constructor. */
	SphereShape(const SphereShape& value);

	/** Destructor. */
	virtual ~SphereShape();
	//@}

	const BoundingSphere& GetSphere() const { return _Sphere; }
	void SetSphere(const BoundingSphere& value) { _Sphere = value; }

	int32 GetSlices() const { return _Slices; }
	void SetSlices(int32 value) { _Slices = value; }

	int32 GetStacks() const { return _Stacks; }
	void SetStacks(int32 value) { _Stacks = value; }

	virtual bool CreateMesh(ShapeDescription* desc);

public:
	BoundingSphere _Sphere;
	int32 _Slices;
	int32 _Stacks;
};

}

#endif 
