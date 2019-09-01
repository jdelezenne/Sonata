/*=============================================================================
Terrain.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Terrain.h"
#include "Graphics/System/RenderSystem.h"

namespace SonataEngine
{

Terrain::Terrain() :
	SceneObject(),
	_HeightField(NULL),
	_TerrainRenderer(NULL),
	_FieldScale(Vector2::One),
	_HeightScale(1.0f),
	_Width(0.0f),
	_Height(0.0f)
{
}

Terrain::~Terrain()
{
	SE_DELETE(_TerrainRenderer);
}

void Terrain::GetScaledPosition(int32 x, int32 y, real32& sx, real32& sy)
{
	if (!_HeightField)
		return;

	if (x < 0 || x >= _HeightField->GetWidth() || y < 0 || y >= _HeightField->GetHeight())
		return;

	sx = x * _FieldScale.X;
	sy = y * _FieldScale.Y;
}

Vector3 Terrain::GetVector3(real32 x, real32 y, real32 z)
{
	return Vector3(x * _FieldScale.X, y * _HeightScale, z * _FieldScale.Y);
}

real32 Terrain::GetScaledHeight(int32 x, int32 y)
{
	if (!_HeightField)
		return 0.0f;

	if (x < 0 || x >= _HeightField->GetWidth() || y < 0 || y >= _HeightField->GetHeight())
		return 0.0f;

	return _HeightField->GetHeight(x, y) * _HeightScale;
}

real32 Terrain::GetInterpolatedHeight(real32 x, real32 y)
{
	// scale down the coordinate
	int32 x0 = x / _FieldScale.X;
	int32 y0 = y / _FieldScale.X;

	// get the scaled height at the 4 corners
	real32 h = GetScaledHeight(x0, y0);
	real32 hy = GetScaledHeight(x0, y0+1);
	real32 hx = GetScaledHeight(x0+1, y0);
	real32 hxy = GetScaledHeight(x0+1, y0+1);

	real32 i1 = Math::Lerp(h, hx, (x/_FieldScale.X) - x0);
	real32 i2 = Math::Lerp(hy, hxy, (x/_FieldScale.X) - x0);
	return Math::Lerp(i1, i2, (y/_FieldScale.Y) - y0);
}

bool Terrain::Create()
{
	if (!_TerrainRenderer)
		return false;

	_Width = _HeightField->GetWidth() * _FieldScale.X;
	_Height = _HeightField->GetHeight() * _FieldScale.Y;

	return _TerrainRenderer->Create();
}

void Terrain::Render()
{
	if (!_TerrainRenderer)
		return;

	_TerrainRenderer->Render();
}

}
