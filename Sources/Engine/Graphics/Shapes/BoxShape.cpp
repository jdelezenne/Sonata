/*=============================================================================
BoxShape.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "BoxShape.h"

namespace SonataEngine
{

// Structures from DXUT
static float s_CubeVertices[8][3] =
{
    // Lower tier (lower in y)
    {-0.5f, -0.5f, -0.5f}, 
    {-0.5f, -0.5f,  0.5f}, 
    { 0.5f, -0.5f,  0.5f}, 
    { 0.5f, -0.5f, -0.5f}, 
    
    // Upper tier
    {-0.5f, 0.5f, -0.5f}, 
    {-0.5f, 0.5f,  0.5f}, 
    { 0.5f, 0.5f,  0.5f}, 
    { 0.5f, 0.5f, -0.5f}, 
};

static real32 s_CubeNormals[6][3] =
{
    {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
    {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}
};

static real32 s_CubeTexCoords[4][2] =
{
    // Lower tier (lower in y)
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f},
    {1.0f, 0.0f}
};

static int32 s_CubeFaces[6][4] =
{
    { 0, 1, 5, 4 }, { 4, 5, 6, 7 }, { 7, 6, 2, 3 },
    { 1, 0, 3, 2 }, { 1, 2, 6, 5 }, { 0, 4, 7, 3 }
};

BoxShape::BoxShape() :
	Shape(),
	_Divisions(1)
{
}

BoxShape::BoxShape(const BoundingBox& box) :
	Shape(),
	_Box(box),
	_Divisions(1)
{
}

BoxShape::BoxShape(const BoxShape& value) :
	Shape(),
	_Box(value.GetBox()),
	_Divisions(value.GetDivisions())
{
}

BoxShape::~BoxShape()
{
}

bool BoxShape::CreateMesh(ShapeDescription* desc)
{
	int32 vertexCount = 24;
	int32 faceCount = 12;

	int32 divCount = Math::Sqrt(_Divisions * _Divisions);

	Geometry::VertexArray vertices(vertexCount);
	Geometry::NormalArray normals(vertexCount);
	Geometry::TextureCoordArray texcoords(vertexCount);
	Geometry::IndexArray indices(faceCount * 3);

	int start = 0;

	Vector3* pVertex = &vertices[0];
	Vector3* pNormal = &normals[0];
	Vector2* pTexcoord = &texcoords[0];
	int32* pFace = &indices[0];

	const Vector3 extents = _Box.GetExtents()*2;

	// iterates over the faces
	for (int i=0; i<6; ++i)
	{
		for (int j=0; j<4; ++j)
		{
			(*pVertex).X = s_CubeVertices[s_CubeFaces[i][j]][0] * extents.X;
			(*pVertex).Y = s_CubeVertices[s_CubeFaces[i][j]][1] * extents.Y;
			(*pVertex).Z = s_CubeVertices[s_CubeFaces[i][j]][2] * extents.Z;
			pVertex++;

			(*pNormal).X = s_CubeNormals[i][0];
			(*pNormal).Y = s_CubeNormals[i][1];
			(*pNormal).Z = s_CubeNormals[i][2];
			pNormal++;

			if (desc != NULL && desc->TextureCoordinates)
			{
				(*pTexcoord).X = s_CubeTexCoords[j][0];
				(*pTexcoord).Y = s_CubeTexCoords[j][1];
				pTexcoord++;
			}
		}

		// 2 triangles per face
		pFace[0] = start;
		pFace[1] = start + 1;
		pFace[2] = start + 2;
		pFace += 3;

		pFace[0] = start + 2;
		pFace[1] = start + 3;
		pFace[2] = start;
		pFace += 3;

		start += 4;
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
