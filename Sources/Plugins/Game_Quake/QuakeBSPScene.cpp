/*=============================================================================
QuakeBSPScene.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "QuakeBSPScene.h"

namespace SE_Quake
{

BSPNode::BSPNode() :
	_isLeaf(false),
	_bounds(AABB::Empty),
	_frontNode(NULL),
	_backNode(NULL),
	_faceCount(0),
	_faceStart(0),
	_clusterIndex(0)
{
}

BSPNode::~BSPNode()
{
}

PlaneIntersectionType BSPNode::GetSide(const Vector3& point) const
{
	SE_ASSERTMSG(!_isLeaf, "Invalid operation. The node is not a leaf node.");
	return _plane.ClassifyPoint(point);
}

real BSPNode::GetDistance(const Vector3& point) const
{
	SE_ASSERTMSG(!_isLeaf, "Invalid operation. The node is not a leaf node.");
	return _plane.GetDistance(point);
}

BSPNode* BSPNode::GetNextNode(const Vector3& point) const
{
	SE_ASSERTMSG(!_isLeaf, "Invalid operation. The node is not a leaf node.");
	if (GetSide(point) == PlaneIntersectionType_Front)
	{
		return GetFrontNode();
	}
	else
	{
		return GetBackNode();
	}
}


int BSPNode::GetObjectCount()
{
	return _sceneObjects.Count();
}

void BSPNode::AddObject(SceneObject* value)
{
	if (!_sceneObjects.Contains(value))
	{
		_sceneObjects.Add(value);
	}
}

void BSPNode::RemoveObject(SceneObject* value)
{
	_sceneObjects.Remove(value);
}

SceneObject* BSPNode::GetObject(int index)
{
	return _sceneObjects[index];
}


BSPSurface::BSPSurface() :
	_renderData(NULL),
	_shader(NULL)
{
	SE_DELETE(_renderData);
	SE_RELEASE(_shader);
}

BSPSurface::~BSPSurface()
{
}


BSPScene::BSPScene():
	Scene(),
	_vertexData(NULL),
	_indexData(NULL),
	_nodeCount(0),
	_leafStart(0),
	_leafCount(0)
{
}

BSPScene::~BSPScene()
{
	SE_DELETE(_vertexData);
	SE_DELETE(_indexData);
}

BSPNode* BSPScene::GetRootBSPNode() const
{
	return (BSPNode*)&_nodes[0];
}

void BSPScene::Render()
{
	SceneState* sceneState = SceneManager::Instance()->GetSceneState();
	Vector3 position = sceneState->Camera->GetWorldPosition();
	Frustum frustum = sceneState->Camera->GetFrustum();

	BuildVisibility(position, frustum);

	RenderGeometry();
	RenderSceneObjects();
}

void BSPScene::RenderGeometry()
{
	RenderSystem* renderSystem = RenderSystem::Current();
	SceneState* sceneState = SceneManager::Instance()->GetSceneState();

	renderSystem->SetProjectionTransform(sceneState->Projection);
	renderSystem->SetViewTransform(sceneState->View);
	renderSystem->SetWorldTransform(GetWorldTransform());

	renderSystem->SetFillMode(FillMode_Solid);
	renderSystem->SetShadeMode(ShadeMode_Smooth);
	renderSystem->SetCullMode(CullMode_None);

	renderSystem->SetAmbientColor(Color32::Black);

	FogState fogState;
	renderSystem->SetFogState(fogState);

	LightState lightState;
	lightState.Lighting = false;
	renderSystem->SetLightState(lightState);

	TextureState texture0;
	texture0.TextureCoordinateIndex = 0;
	texture0.ColorOperation = TextureOperation_Modulate;
	texture0.ColorArgument1 = TextureArgument_TextureColor;
	texture0.ColorArgument2 = TextureArgument_Diffuse;
	texture0.AlphaOperation = TextureOperation_Disable;
	renderSystem->SetTextureState(0, texture0);

	TextureState texture1;
	texture1.TextureCoordinateIndex = 1;
	texture1.ColorOperation = TextureOperation_Modulate4X;
	texture1.ColorArgument1 = TextureArgument_TextureColor;
	texture1.ColorArgument2 = TextureArgument_Current;
	texture1.AlphaOperation = TextureOperation_Disable;
	renderSystem->SetTextureState(1, texture1);

	int surfaceCount = _visibleSurfaces.Count();
	for (int i = 0; i < surfaceCount; ++i)
	{
		BSPSurface* surface = _visibleSurfaces[i];
		FFPPass* pass = ((FFPPass*)((DefaultMaterial*)surface->GetShader())->
			GetTechnique()->GetPassByIndex(0));

		renderSystem->SetSamplerState(0, *pass->GetSamplerStateByIndex(0));
		if (pass->GetSamplerStateCount() > 1)
		{
			renderSystem->SetSamplerState(1, *pass->GetSamplerStateByIndex(1));
		}
		else
		{
			renderSystem->DisableSamplerState(1);
		}

		renderSystem->Render(surface->GetRenderData());
	}
}

void BSPScene::RenderSceneObjects()
{
}

void BSPScene::BuildVisibility(const Vector3& position, const Frustum& frustum)
{
	_visibleSurfaces.Clear();

	// Locate the node for the given position
	BSPNode* locationNode = FindLeaf(position);

	// Find the visible faces from this leaf node
	for (int i = 0; i < _leafCount; ++i)
	{
		BSPNode* leaf = &_nodes[_leafStart + i];

		if (IsLeafVisible(locationNode, leaf))
		{
			//if (frustum.Intersects(leaf->GetBounds()))
			{
				ProcessLeaf(leaf);
			}
		}
	}
}

bool BSPScene::IsLeafVisible(const BSPNode* source, const BSPNode* dest) const
{
	if (dest->_clusterIndex == -1)
		return false;

	if (source->_clusterIndex == -1)
		return true;

	if (!source->IsLeaf() || !dest->IsLeaf())
	{
		SEthrow(ArgumentException("The source and destination nodes must be leaf nodes."));
		return false;
	}

	// Determine visibility using PVS.
	//
	// - Gets the offset of the source node.
	// Offset = Source.ClusterIndex * ClusterStride;
	//
	// - Adds the columns offset divided by 8 (8 bits per bytes).
	// Offset += Dest.ClusterIndex >> 3;
	//
	// - Gets the right bit of the byte at that offset
	//   by doing a bitwise AND with the bit at the remainded position.
	// Result = *(ClusterData + Offset) & (1 << (Dest.ClusterIndex & 7));
	//
	// - If the resulting value is different than 0, the leaf is visible.
	//   by doing a bitwise AND with the bit at the remainded position

	return (*(_clusterData + source->_clusterIndex * _clusterStride +
		((dest->_clusterIndex) >> 3)) & (1 << ((dest->_clusterIndex) & 7))) != 0;
}

BSPNode* BSPScene::FindLeaf(const Vector3& point) const
{
	BSPNode* node = GetRootBSPNode();
	while (!node->IsLeaf())
	{
		node = node->GetNextNode(point);
	}
	return node;
}

void BSPScene::ProcessLeaf(BSPNode* leaf)
{
	int faceCount = leaf->GetFaceCount();
	int faceStart = leaf->GetFaceStart();

	for (int i = 0; i < faceCount; ++i)
	{
		int faceIndex = _leafFaces[faceStart + i];
		BSPSurface* face = _surfaces[faceIndex];
		_visibleSurfaces.Add(face);
	}

	//TODO: Add scene objects
}

void BSPScene::_SceneObjectMoved(SceneObject* obj, const Vector3& position)
{
}

void BSPScene::_SceneObjectRemoved(SceneObject* obj)
{
}

}
