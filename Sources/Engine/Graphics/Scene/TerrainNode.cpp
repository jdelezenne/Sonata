/*=============================================================================
TerrainNode.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "TerrainNode.h"
#include "Graphics/System/RenderSystem.h"

namespace SonataEngine
{

SE_IMPLEMENT_CLASS(TerrainNode);
SE_IMPLEMENT_REFLECTION(TerrainNode);

TerrainNode::TerrainNode() :
	SceneObject(),
	_Terrain(NULL)
{
}

TerrainNode::~TerrainNode()
{
}

void TerrainNode::UpdateBounds()
{
	if (_Terrain == NULL)
		return;

	//_Terrain->UpdateBounds();
}

void TerrainNode::Render()
{
	if (_Terrain == NULL)
		return;

	RenderSystem* renderer = RenderSystem::Current();
	if (renderer != NULL)
	{
		renderer->SetWorldTransform(GetWorldTransform());
		_Terrain->Render();
	}
}

}
