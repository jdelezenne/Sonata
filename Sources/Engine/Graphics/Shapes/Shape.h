/*=============================================================================
Shape.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SHAPE_H_
#define _SE_SHAPE_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Geometry/Geometry.h"
#include "Graphics/Model/Mesh.h"

namespace SonataEngine
{

struct ShapeDescription
{
	ShapeDescription() :
		TextureCoordinates(false)
	{
	}

	bool TextureCoordinates;
};

/**
	@brief Shape.

	Base class for shape implementations.
	A shape is a geometric object that can be used for collision and
	rendering.
*/
class SE_GRAPHICS_EXPORT Shape
{
protected:
	Transform3 _transform;
	Geometry _geometry;
	MeshPtr _mesh;

public:
	/** Destructor. */
	virtual ~Shape();

	const Transform3& GetTransform() const { return _transform; }
	void SetTransform(const Transform3& value) { _transform = value; }

	Geometry* GetGeometry() { return &_geometry; }

	Mesh* GetMesh() const { return _mesh; }

	/** Creates the mesh to render the shape. */
	virtual bool CreateMesh(ShapeDescription* desc);

protected:
	/** Protected constructor. */
	Shape();
};

}

#endif 
