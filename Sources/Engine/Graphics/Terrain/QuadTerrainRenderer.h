/*=============================================================================
QuadTerrainRenderer.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_QUADTERRAINRENDERER_H_
#define _SE_QUADTERRAINRENDERER_H_

#include "Graphics/Terrain/TerrainRenderer.h"
#include "Core/Math/Frustum.h"

namespace SonataEngine
{

/**
	Represents a node in a quad tree.
	The children are indexed the following way:
0,0					width,0
			0--1 
			|  |
			2--3
0,height			0,height
*/
struct QuadNode
{
	int32 Depth;
	real32 LOD;
	AABB BoundingBox;
	QuadNode* Children[4];
};

/**
	Quad terrain renderer.
*/
class SE_GRAPHICS_EXPORT QuadTerrainRenderer : public TerrainRenderer
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	QuadTerrainRenderer();

	/** Destructor. */
	virtual ~QuadTerrainRenderer();
	//@}

	/** @name Properties. */
	//@{
	int32 GetDepth() const { return _Depth; }
	void SetDepth(int32 value) { _Depth = value; }
	//@}

	virtual bool Create();

	virtual void Destroy();

	virtual void Render();
/*
	QuadNode* CreateNode(real32 lod, const AABB& boundingBox, const Vector3& center,
		QuadNode* up, QuadNode* left, QuadNode* down, QuadNode* right);
	QuadNode* CreateLeaf(real32 lod, const AABB& boundingBox, const Vector3& center);
*/
	void DestroyNode(QuadNode* node);

	void InitNode(QuadNode* parent, int32 depth, const Vector3& min, const Vector3& max);

	void RenderNode(QuadNode* node);

protected:
	MeshPtr _mesh;
	HardwareBufferPtr _indexBuffer;
	QuadNode* _Root;
	int32 _Depth;
	Frustum _Frustum;

	int32 _VisibleNodes;
};

}

#endif 
