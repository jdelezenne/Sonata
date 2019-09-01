/*=============================================================================
SphereShape.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SphereShape.h"

namespace SonataEngine
{

#define CACHE_SIZE 240

SphereShape::SphereShape() :
	Shape(),
	_Slices(2),
	_Stacks(2)
{
}

SphereShape::SphereShape(const BoundingSphere& sphere) :
	Shape(),
	_Sphere(sphere),
	_Slices(2),
	_Stacks(2)
{
}

SphereShape::SphereShape(const SphereShape& value) :
	Shape(),
	_Sphere(value.GetSphere()),
	_Slices(value.GetSlices()),
	_Stacks(value.GetStacks())
{
}

SphereShape::~SphereShape()
{
}

bool SphereShape::CreateMesh(ShapeDescription* desc)
{
	int32 faceCount = 2 * (_Stacks - 1) * _Slices;
	int32 vertexCount = (_Stacks - 1) * _Slices + 2;

	Geometry::VertexArray vertices(vertexCount);
	Geometry::NormalArray normals(vertexCount);
	Geometry::TextureCoordArray texturecoords(vertexCount);
	Geometry::IndexArray indices(faceCount * 3);

	int i, j;

	// Sin/Cos caches
	real32 sinI[CACHE_SIZE], cosI[CACHE_SIZE];
	real32 sinJ[CACHE_SIZE], cosJ[CACHE_SIZE];

	for (i = 0; i < _Slices; i++)
		Math::SinCos(2.0f * Math::Pi * i / _Slices, sinI[i], cosI[i]);

	for (j = 0; j < _Stacks; j++) 
		Math::SinCos(Math::Pi * j / _Stacks, sinJ[j], cosJ[j]);


	// Generate vertices
	Vector3* pVertex = &vertices[0];
	Vector3* pNormal = &normals[0];
	Vector2* pTexCoord = &texturecoords[0];

	// +Z pole
	*pVertex  = Vector3(0.0f, 0.0f, _Sphere.Radius);
	*pNormal = Vector3(0.0f, 0.0f, 1.0f);
	pVertex++;
	pNormal++;

	// Stacks
	for(j = 1; j < _Stacks; j++) 
	{
		for(i = 0; i < _Slices; i++)
		{
			Vector3 norm(sinI[i] * sinJ[j], cosI[i] * sinJ[j], cosJ[j]);

			*pVertex  = norm * _Sphere.Radius;
			*pNormal = norm;

			pVertex++;
			pNormal++;
		}
	}

	// Z- pole
	*pVertex  = Vector3(0.0f, 0.0f, -_Sphere.Radius);
	*pNormal = Vector3(0.0f, 0.0f, -1.0f);
	pVertex++;
	pNormal++;


	// Generate indices
	int32* pFace = &indices[0];
	uint uRowA, uRowB;

	// Z+ pole
	uRowA = 0;
	uRowB = 1;

	for(i = 0; i < _Slices - 1; i++)
	{
		pFace[0] = uRowA;
		pFace[1] = uRowB + i + 1;
		pFace[2] = uRowB + i;
		pFace += 3;
	}

	pFace[0] = uRowA;
	pFace[1] = uRowB;
	pFace[2] = uRowB + i;
	pFace += 3;

	// Interior stacks
	for (j = 1; j < _Stacks - 1; j++)
	{
		uRowA = 1 + (j - 1) * _Slices;
		uRowB = uRowA + _Slices;

		for (i = 0; i < _Slices - 1; i++)
		{
			pFace[0] = uRowA + i;
			pFace[1] = uRowA + i + 1;
			pFace[2] = uRowB + i;
			pFace += 3;

			pFace[0] = uRowA + i + 1;
			pFace[1] = uRowB + i + 1;
			pFace[2] = uRowB + i;
			pFace += 3;
		}

		pFace[0] = uRowA + i;
		pFace[1] = uRowA;
		pFace[2] = uRowB + i;
		pFace += 3;

		pFace[0] = uRowA;
		pFace[1] = uRowB;
		pFace[2] = uRowB + i;
		pFace += 3;
	}

	// Z- pole
	uRowA = 1 + (_Stacks - 2) * _Slices;
	uRowB = uRowA + _Slices;

	for (i = 0; i < _Slices - 1; i++)
	{
		pFace[0] = uRowA + i;
		pFace[1] = uRowA + i + 1;
		pFace[2] = uRowB;
		pFace += 3;
	}

	pFace[0] = uRowA + i;
	pFace[1] = uRowA;
	pFace[2] = uRowB;
	pFace += 3;

	_geometry.SetVertices(vertices);
	_geometry.SetNormals(normals);

	if (desc != NULL && desc->TextureCoordinates)
	{
		_geometry.SetTextureCoords(texturecoords);
	}

	_geometry.SetIndices(indices);

	return Shape::CreateMesh(desc);
}

}
