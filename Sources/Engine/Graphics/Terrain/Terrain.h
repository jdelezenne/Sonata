/*=============================================================================
Terrain.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_TERRAIN_H_
#define _SE_TERRAIN_H_

#include "Graphics/Scene/SceneObject.h"
#include "Graphics/Model/Mesh.h"
#include "Graphics/Terrain/HeightField.h"
#include "Graphics/Terrain/TerrainRenderer.h"

namespace SonataEngine
{

/**
	Terrain.
*/
class SE_GRAPHICS_EXPORT Terrain : public SceneObject
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	Terrain();

	/** Destructor. */
	virtual ~Terrain();
	//@}

	/** @name Properties. */
	//@{
	HeightField* GetHeightField() const { return _HeightField; }
	void SetHeightField(HeightField* value) { _HeightField = value; }

	TerrainRenderer* GetTerrainRenderer() const { return _TerrainRenderer; }
	void SetTerrainRenderer(TerrainRenderer* value) { _TerrainRenderer = value; }

	Vector2 GetFieldScale() const { return _FieldScale; }
	void SetFieldScale(Vector2 value) { _FieldScale = value; }
	void SetFieldScale(real32 value) { _FieldScale = Vector2(value); }

	real32 GetHeightScale() const { return _HeightScale; }
	void SetHeightScale(real32 value) { _HeightScale = value; }

	real32 GetWidth() const { return _Width; }

	real32 GetHeight() const { return _Height; }

	void GetScaledPosition(int32 x, int32 y, real32& sx, real32& sy);

	Vector3 GetVector3(real32 x, real32 y, real32 z);

	real32 GetScaledHeight(int32 x, int32 y);

	real32 GetInterpolatedHeight(real32 x, real32 y);
	//@}

	bool Create();

	virtual void Render();

protected:
	HeightFieldPtr _HeightField;
	TerrainRenderer* _TerrainRenderer;
	Vector2 _FieldScale;
	real32 _HeightScale;
	real32 _Width;
	real32 _Height;
};

typedef SmartPtr<Terrain> TerrainPtr;

}

#endif 
