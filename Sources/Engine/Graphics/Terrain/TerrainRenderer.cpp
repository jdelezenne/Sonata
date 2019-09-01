/*=============================================================================
TerrainRenderer.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "TerrainRenderer.h"
#include "Graphics/System/RenderSystem.h"
#include "Graphics/Terrain/Terrain.h"

namespace SonataEngine
{

TerrainRenderer::TerrainRenderer() :
	_Terrain(NULL),
	_camera(NULL),
	_shader(NULL),
	_FaceNormals(NULL),
	_VertexNormals(NULL)
{
}

TerrainRenderer::~TerrainRenderer()
{
}

Vector3 TerrainRenderer::GetNormal(int32 x, int32 y)
{
	if (!_Terrain || _VertexNormals.IsEmpty())
		return Vector3::UnitY;

	HeightField* field = _Terrain->GetHeightField();
	if (!field)
		return Vector3::UnitY;

	if (x < 0 || x >= field->GetWidth() || y < 0 || y >= field->GetHeight())
		return Vector3::UnitY;

	return _VertexNormals[y * field->GetWidth() + x];
}

Vector3 TerrainRenderer::GetInterpolatedNormal(real32 x, real32 y)
{
	if (!_Terrain)
		return Vector3::UnitY;

	// scale down the coordinate
	int32 x0 = x / _Terrain->GetFieldScale().X;
	int32 y0 = y / _Terrain->GetFieldScale().Y;

	// get the normal at the 4 corners
	Vector3 n = GetNormal(x0, y0);
	Vector3 ny = GetNormal(x0, y0+1);
	Vector3 nx = GetNormal(x0+1, y0);
	Vector3 nxy = GetNormal(x0+1, y0+1);

	Vector3 i1 = Vector3::Lerp(n, nx, (x/_Terrain->GetFieldScale().X) - x0);
	Vector3 i2 = Vector3::Lerp(ny, nxy, (x/_Terrain->GetFieldScale().X) - x0);
	return Vector3::Lerp(i1, i2, (y/_Terrain->GetFieldScale().Y) - y0);
}

void TerrainRenderer::_ComputeFaceNormals()
{
	HeightField* field = _Terrain->GetHeightField();
	if (!field)
		return;

	int32 width = field->GetWidth();
	int32 height = field->GetHeight();

	_FaceNormals.Resize(2 * (width+2) * (height+2));

	int index = 0;
	for (int y = -1; y < height; y++)
	{
		for (int x = -1; x < width; x++)
		{
			// 0-2
			// |/|
			// 1-3
			// t0 = 0,1,2
			// t1 = 3,2,1
			if (x >= 0 && y >= 0 && x < width && y < height)
			{
				real32 h0 = field->GetHeight(x, y);
				real32 h1 = field->GetHeight(x, y+1);
				real32 h2 = field->GetHeight(x+1, y);
				real32 h3 = field->GetHeight(x+1, y+1);

				Vector3 v01 = _Terrain->GetVector3(x, h1, y+1) - _Terrain->GetVector3(x, h0, y);
				Vector3 v02 = _Terrain->GetVector3(x+1, h2, y) - _Terrain->GetVector3(x, h0, y);
				Vector3 n0 = Vector3::Cross(v01, v02);

				Vector3 v32 = _Terrain->GetVector3(x+1, h2, y) - _Terrain->GetVector3(x+1, h3, y+1);
				Vector3 v31 = _Terrain->GetVector3(x, h1, y+1) - _Terrain->GetVector3(x+1, h3, y+1);
				Vector3 n1 = Vector3::Cross(v32, v31);

				_FaceNormals[index++] = Vector3::Normalize(n0);
				_FaceNormals[index++] = Vector3::Normalize(n1);
			}
			else
			{
				// out of the heightmap
				_FaceNormals[index++] = Vector3::UnitY;
				_FaceNormals[index++] = Vector3::UnitY;
			}
		}
	}

	/*for (int y = -1; y < height; y++)
	{
		_FaceNormals[y*width] = Vector3::UnitY;
		_FaceNormals[y*width] = Vector3::UnitY;
	}

	for (int x = -1; x < width; x++)
	{
		_FaceNormals[x] = Vector3::UnitY;
		_FaceNormals[x+width-1] = Vector3::UnitY;
	}*/
}

void TerrainRenderer::_ComputeVertexNormals()
{
	HeightField* field = _Terrain->GetHeightField();
	if (!field)
		return;

	int x, y;
	int32 width = field->GetWidth();
	int32 height = field->GetHeight();

	_VertexNormals.Resize(width * height);

	Vector3 normals[6];

	// width+1 because the face normals array has 2 more rows
	int stride = (width+1) * 2 - 1;
	int index = 0;
	int face = 1;
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			// vertex 4 is connected to faces 1,2,3, 4,5,6
			// 0----1----2
			// |0 / |2 / |
			// | / 1| / 3|
			// 3----4----5
			// |4 / |6 / |
			// | / 5| / 7|
			// 6----7----8
			normals[0] = _FaceNormals[face];
			normals[1] = _FaceNormals[face + 1];
			normals[2] = _FaceNormals[face + 2];
			normals[3] = _FaceNormals[stride + face];
			normals[4] = _FaceNormals[stride + face + 1];
			normals[5] = _FaceNormals[stride + face + 2];

			_VertexNormals[index++] = Vector3::Normalize(
				(normals[0]+normals[1]+normals[2]+
				normals[3]+normals[4]+normals[5]) / 6.0f);

			face += 2;
		}

		face += 2;
	}
}

}
