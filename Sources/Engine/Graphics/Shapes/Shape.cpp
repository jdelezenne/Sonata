/*=============================================================================
Shape.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Shape.h"

namespace SonataEngine
{

Shape::Shape() :
	_mesh(NULL)
{
}

Shape::~Shape()
{
	SE_DELETE(_mesh);
}

bool Shape::CreateMesh(ShapeDescription* desc)
{
	SE_DELETE(_mesh);

	MeshPart* meshPart;
	if (!_geometry.CreatePrimitive(&meshPart))
	{
		return false;
	}

	_mesh = new Mesh();
	_mesh->AddMeshPart(meshPart);
	_geometry.UpdateBounds();
	_mesh->SetBoundingBox(_geometry.GetBoundingBox());

	return true;
}

}
