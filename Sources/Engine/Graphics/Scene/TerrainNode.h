/*=============================================================================
TerrainNode.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_TERRAINNODE_H_
#define _SE_TERRAINNODE_H_

#include "Core/Core.h"
#include "Graphics/Scene/SceneObject.h"
#include "Graphics/Terrain/Terrain.h"

namespace SonataEngine
{

/**
	@brief Terrain node.

	Terrains can be attached to this node to be rendered.
*/
class SE_GRAPHICS_EXPORT TerrainNode : public SceneObject
{
	SE_DECLARE_CLASS(TerrainNode, SceneObject);
	SE_BEGIN_REFLECTION(TerrainNode);
		//SE_Field(_Terrain, Terrain, Public);
	SE_END_REFLECTION(TerrainNode);

public:
	TerrainNode();
	virtual ~TerrainNode();

	Terrain* GetTerrain() const { return _Terrain; }
	void SetTerrain(Terrain* mesh) { _Terrain = mesh; }

	virtual void Render();

	virtual void UpdateBounds();

protected:
	TerrainPtr _Terrain;
	AABB _LocalBoundingBox;
};

}

#endif
