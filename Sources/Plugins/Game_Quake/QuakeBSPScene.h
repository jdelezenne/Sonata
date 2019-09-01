/*=============================================================================
QuakeBSPScene.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_QUAKEBSPSCENE_H_
#define _SE_QUAKEBSPSCENE_H_

#include <Core/Core.h>
#include <Graphics/Graphics.h>

using namespace SonataEngine;

namespace SE_Quake
{

class BSPFile;
class BSPScene;

struct BSPBrush
{
	BaseArray<Plane> planes;
};

class BSPNode
{
	friend BSPFile;
	friend BSPScene;

protected:
	bool _isLeaf;
	Plane _plane;
	AABB _bounds;
	BSPNode* _frontNode;
	BSPNode* _backNode;
	int _faceCount;
	int _faceStart;
	int _clusterIndex;
	Array<BSPBrush*> _solidBrushes;
	Array<SceneObject*> _sceneObjects;

public:
	BSPNode();
	virtual ~BSPNode();

	bool IsLeaf() const { return _isLeaf; }

	const Plane& GetPlane() const { return _plane; }

	const AABB& GetBounds() const { return _bounds; }

	BSPNode* GetFrontNode() const { return _frontNode; }
	void SetFrontNode(BSPNode* value) { _frontNode = value; }

	BSPNode* GetBackNode() const { return _backNode; }
	void SetBackNode(BSPNode* value) { _backNode = value; }

	int GetFaceCount() const { return _faceCount; }

	int GetFaceStart() const { return _faceStart; }

	int GetObjectCount();
	void AddObject(SceneObject* value);
	void RemoveObject(SceneObject* value);
	SceneObject* GetObject(int index);

	PlaneIntersectionType GetSide(const Vector3& point) const;
	real GetDistance(const Vector3& point) const;
	BSPNode* GetNextNode(const Vector3& point) const;
};

class BSPSurface
{
	friend BSPFile;

protected:
	RenderData* _renderData;
	ShaderMaterialPtr _shader;

public:
	BSPSurface();
	virtual ~BSPSurface();

	RenderData* GetRenderData() const { return _renderData; }
	void SetRenderData(RenderData* value) { _renderData = value; }

	ShaderMaterial* GetShader() const { return _shader; }
	void SetShader(ShaderMaterial* value) { _shader = value; }
};

class BSPScene : public Scene
{
	friend BSPFile;

protected:
	VertexData* _vertexData;
	IndexData* _indexData;
	Array<int> _leafFaces;
	Array<BSPSurface*> _surfaces;
	Array<BSPSurface*> _visibleSurfaces;
	int _nodeCount;
	int _leafStart;
	int _leafCount;
	BaseArray<BSPNode> _nodes;
	BaseArray<BSPBrush> _brushes;

	uint8* _clusterData;
	int _clusterCount;
	int _clusterStride;

public:
	BSPScene();
	virtual ~BSPScene();

	virtual void Render();
	virtual void RenderGeometry();
	virtual void RenderSceneObjects();

	VertexData* GetVertexData() const { return _vertexData; }

	IndexData* GetIndexData() const { return _indexData; }

	const Array<BSPSurface*>& GetSurfaces() const { return _surfaces; }

	int GetNodeCount() const { return _nodeCount; }

	int GetLeafCount() const { return _leafCount; }

	const BaseArray<BSPNode>& GetNodes() const { return _nodes; }

	BSPNode* GetRootBSPNode() const;

	void BuildVisibility(const Vector3& position, const Frustum& frustum);
	bool IsLeafVisible(const BSPNode* source, const BSPNode* dest) const;
	BSPNode* FindLeaf(const Vector3& point) const;

protected:
	void ProcessLeaf(BSPNode* leaf);

	void _SceneObjectMoved(SceneObject* obj, const Vector3& position);
	void _SceneObjectRemoved(SceneObject* obj);
};

}

#endif 
