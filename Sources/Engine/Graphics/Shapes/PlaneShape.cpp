/*=============================================================================
PlaneShape.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "PlaneShape.h"

namespace SonataEngine
{

PlaneShape::PlaneShape() :
	Shape(),
	_Scale(Vector2::One),
	_Divisions(1)
{
}

PlaneShape::PlaneShape(const Plane& plane) :
	Shape(),
	_Plane(plane),
	_Scale(Vector2::One),
	_Divisions(1)
{
}

PlaneShape::PlaneShape(const PlaneShape& value) :
	Shape(),
	_Plane(value.GetPlane()),
	_Scale(value.GetScale()),
	_Divisions(value.GetDivisions())
{
}

PlaneShape::~PlaneShape()
{
}

/**
	x-----x
	|\    |
	|  \  |
	|    \|
	x-----x
*/
bool PlaneShape::CreateMesh(ShapeDescription* desc)
{
	int i, j;
	int32 vertexCount = (_Divisions+1) * (_Divisions+1);
	int32 faceCount = _Divisions * _Divisions * 2;
	int32 divCount = Math::Sqrt(_Divisions * _Divisions);

	Geometry::VertexArray vertices(vertexCount);
	Geometry::NormalArray normals(vertexCount);
	Geometry::TextureCoordArray texcoords(vertexCount);
	Geometry::IndexArray indices(faceCount * 3);

	real32 delta = 2.0f / (real32)divCount;

	int iVertex = 0;
	for (i = 0; i <= divCount; ++i)
	{
		for (j = 0; j <= divCount; ++j)
		{
			Vector3& vertex = vertices[iVertex];
			vertex.X = (-1.0f + ((real32)j * delta)) * _Scale.X;
			vertex.Y = 0.0f;
			vertex.Z = (-1.0f + ((real32)i * delta)) * _Scale.Y;

			Vector3& normal = normals[iVertex];
			normal = Vector3::UnitY;

			if (desc != NULL && desc->TextureCoordinates)
			{
				Vector2& texcoord = texcoords[iVertex];
				texcoord.X = ((real32)j * delta * 0.5f);
				texcoord.Y = (1.0f - (real32)i * delta * 0.5f);
			}

			iVertex++;
		}
	}

	int iIndex = 0;
	int start;
	for (i = 0; i < divCount; ++i)
	{
		for (j = 0; j < divCount; ++j)
		{
			start = i * (divCount+1) + j;

			// triangle 1
			indices[iIndex++] = start; 
			indices[iIndex++] = start + divCount + 1; 
			indices[iIndex++] = start + 1; 

			// triangle 2
			indices[iIndex++] = start + 1;
			indices[iIndex++] = start + divCount + 1;
			indices[iIndex++] = start + divCount + 2;
		}
	}

	_geometry.SetVertices(vertices);
	_geometry.SetNormals(normals);

	if (desc != NULL && desc->TextureCoordinates)
	{
		_geometry.SetTextureCoords(texcoords);
	}

	_geometry.SetIndices(indices);

	return Shape::CreateMesh(desc);
}

}
