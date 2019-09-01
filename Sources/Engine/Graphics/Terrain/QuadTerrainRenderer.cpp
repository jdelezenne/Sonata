/*=============================================================================
QuadTerrainRenderer.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "QuadTerrainRenderer.h"
#include "Graphics/System/RenderSystem.h"
#include "Graphics/SceneManager.h"
#include "Graphics/Terrain/HeightField.h"
#include "Graphics/Terrain/Terrain.h"

namespace SonataEngine
{

QuadTerrainRenderer::QuadTerrainRenderer() :
	TerrainRenderer(),
	_mesh(NULL),
	_Depth(1)
{
}

QuadTerrainRenderer::~QuadTerrainRenderer()
{
	SE_DELETE(_mesh);
	DestroyNode(_Root);
}

bool QuadTerrainRenderer::Create()
{
	Destroy();

	if (!_Terrain)
	{
		return false;
	}

	HeightField* field = _Terrain->GetHeightField();
	if (!field)
	{
		return false;
	}


	int32 terrainWidth = _Terrain->GetWidth();
	int32 terrainHeight = _Terrain->GetHeight();

	_Root = new QuadNode();
	InitNode(_Root, _Depth, Vector3(0, 0, 0), Vector3(terrainWidth, 0, terrainHeight));














	SE_DELETE(_mesh);

	VertexLayout* vertexLayout = NULL;
	HardwareBuffer* vertexBuffer = NULL;
	HardwareBuffer* indexBuffer = NULL;

	if (!RenderSystem::Current()->CreateVertexLayout(&vertexLayout))
	{
		return false;
	}

	uint16 offset = 0;
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Position));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Normal));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float2, VertexSemantic_TextureCoordinate));
	offset += VertexElement::GetTypeSize(VertexFormat_Float2);

	int x, y;
	int32 width = field->GetWidth();
	int32 height = field->GetHeight();

	try
	{
		int32 vertexCount = width * height;
		if (!RenderSystem::Current()->CreateVertexBuffer(vertexCount * vertexLayout->GetSize(),
			HardwareBufferUsage_Static, &vertexBuffer))
		{
			throw;
		}

		SEbyte* vbData;
		if (!vertexBuffer->Map(HardwareBufferMode_Normal, (void**)&vbData))
		{
			throw;
		}

		real32 sx, sy;
		real32 h;
		SEbyte color;
		real32 tu, tv;

		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width; x++)
			{
				_Terrain->GetScaledPosition(x, y, sx, sy);
				h = _Terrain->GetScaledHeight(x, y);
				color = field->GetHeight(x, y);
				tu = (real32) x / width;
				tv = (real32) y / height;

				Memory::Copy(vbData, Vector3(sx, h, sy), sizeof(Vector3));
				vbData += sizeof(Vector3);

				Vector3 normal = GetNormal(x, y);
				Memory::Copy(vbData, normal, sizeof(Vector3));
				vbData += sizeof(Vector3);

				Memory::Copy(vbData, Vector2(tu, tv), sizeof(Vector2));
				vbData += sizeof(Vector2);
			}
		}

		vertexBuffer->Unmap();

		_indexBuffer = NULL;

		HardwareBuffer* buffer;
		int32 indexCount = (width-1) * (height-1) * 3 * 2 >> (_Depth+1);
		if (!RenderSystem::Current()->CreateIndexBuffer(indexCount * sizeof(int32),
			IndexBufferFormat_Int32, HardwareBufferUsage_Dynamic, &buffer))
		{
			throw;
		}
		_indexBuffer = buffer;

		MeshPart* meshPart = new MeshPart();

		VertexData* vertexData = new VertexData();
		vertexData->VertexLayout = vertexLayout;
		vertexData->VertexStreams.Add(VertexStream(vertexBuffer, vertexLayout->GetSize()));
		vertexData->VertexCount = vertexCount;
		meshPart->SetVertexData(vertexData);
		meshPart->SetIndexed(true);
		meshPart->SetPrimitiveType(PrimitiveType_TriangleList);

		_mesh = new Mesh();
		_mesh->AddMeshPart(meshPart);

		_ComputeFaceNormals();
		_ComputeVertexNormals();

		return true;
	}
	catch (Exception)
	{
		Logger::Current()->Log(LogLevel::Error, _T("QuadTerrainRenderer.Create"),
			_T("Failed to create the terrain geometry."));

		SE_DELETE(vertexLayout);
		SE_DELETE(vertexBuffer);
		SE_DELETE(_indexBuffer);
		return false;
	}
}

void QuadTerrainRenderer::Destroy()
{
}

void QuadTerrainRenderer::Render()
{
	RenderSystem* renderer = RenderSystem::Current();
	if (renderer == NULL)
		return;

	if (_mesh != NULL && _Root != NULL)
	{
		if (_shader != NULL)
		{
			renderer->SetWorldTransform(_Terrain->GetWorldTransform());

			_shader->BeginMaterial();
			_shader->BeginPass(0);
		}

		// Update the frustum with the current camera matrices
		_Frustum.SetMatrix(_camera->GetProjection() * _camera->GetView());

		// Render the quadtree nodes
		_VisibleNodes = 0;
		RenderNode(_Root);

		if (_shader != NULL)
		{
			_shader->EndPass();
			_shader->EndMaterial();
		}

#if 0
		Logger::Current()->Log(LogLevel::Debug, _T("QuadTerrainRenderer.Render"),
			_T("Visible nodes: ") + String::ToString(_VisibleNodes));
#endif
	}
}

void QuadTerrainRenderer::InitNode(QuadNode* parent, int32 depth, const Vector3& offset, const Vector3& size)
{
	int32 x, y;
	real32 min, max;

	parent->Depth = depth;
	parent->BoundingBox = AABB(offset, offset + size);

	if (parent->Depth > 0 && size.X > 2 && size.Z > 2)
	{
		// node
		int32 depth = parent->Depth - 1;
		x = offset.X;
		y = offset.Z;
		int32 width = size.X / 2;
		int32 height = size.Z / 2;

		parent->Children[0] = new QuadNode();
		InitNode(parent->Children[0], depth, Vector3(x, 0, y), Vector3(width, 0, height));

		parent->Children[1] = new QuadNode();
		InitNode(parent->Children[1], depth, Vector3(x + width, 0, y), Vector3(width, 0, height));

		parent->Children[2] = new QuadNode();
		InitNode(parent->Children[2], depth, Vector3(x, 0, y + height), Vector3(width, 0, height));

		parent->Children[3] = new QuadNode();
		InitNode(parent->Children[3], depth, Vector3(x + width, 0, y + height), Vector3(width, 0, height));

		min = parent->Children[0]->BoundingBox.Min.Y;
		max = parent->Children[0]->BoundingBox.Max.Y;

		min = Math::Min(min, parent->Children[1]->BoundingBox.Min.Y);
		max = Math::Max(max, parent->Children[1]->BoundingBox.Max.Y);

		min = Math::Min(min, parent->Children[2]->BoundingBox.Min.Y);
		max = Math::Max(max, parent->Children[2]->BoundingBox.Max.Y);

		min = Math::Min(min, parent->Children[3]->BoundingBox.Min.Y);
		max = Math::Max(max, parent->Children[3]->BoundingBox.Max.Y);

		parent->BoundingBox.Min.Y = min;
		parent->BoundingBox.Max.Y = max;
	}
	else
	{
		// leaf

		parent->Children[0] = NULL;
		parent->Children[1] = NULL;
		parent->Children[2] = NULL;
		parent->Children[3] = NULL;

		// get min/max elevation
		HeightField* field = _Terrain->GetHeightField();
		real32 h;
		min = max = _Terrain->GetScaledHeight(0, 0);

		for (y = 0; y < size.Y; y++)
		{
			for (x = 0; x < size.X; x++)
			{
				h = _Terrain->GetScaledHeight(
					parent->BoundingBox.Min.X+x,
					parent->BoundingBox.Min.Z+y);
				min = Math::Min(min, h);
				max = Math::Max(max, h);
			}
		}

		parent->BoundingBox.Min.Y = min;
		parent->BoundingBox.Max.Y = max;
	}
}
/*
QuadNode* QuadTerrainRenderer::CreateNode(real32 lod, const AABB& boundingBox, const Vector3& center,
	QuadNode* up, QuadNode* left, QuadNode* down, QuadNode* right)
{
	QuadNode* node = new QuadNode();
	node->IsLeaf = false;
	node->LOD = lod;
	node->BoundingBox = boundingBox;
	node->Center = center;
	node->Children[0] = up;
	node->Children[1] = left;
	node->Children[2] = down;
	node->Children[3] = right;
	return node;
}

QuadNode* QuadTerrainRenderer::CreateLeaf(real32 lod, const AABB& boundingBox, const Vector3& center)
{
	QuadNode* node = new QuadNode();
	node->IsLeaf = true;
	node->LOD = lod;
	node->BoundingBox = boundingBox;
	node->Center = center;
	node->Children[0] = NULL;
	node->Children[1] = NULL;
	node->Children[2] = NULL;
	node->Children[3] = NULL;
	return node;
}
*/
void QuadTerrainRenderer::DestroyNode(QuadNode* node)
{
	if (node != NULL)
	{
		DestroyNode(node->Children[0]);
		DestroyNode(node->Children[1]);
		DestroyNode(node->Children[2]);
		DestroyNode(node->Children[3]);

		delete node;
	}
}

void QuadTerrainRenderer::RenderNode(QuadNode* node)
{
	RenderSystem* renderer = RenderSystem::Current();

	if (node->Depth > 0 && node->Children[0] != NULL && node->Children[1] != NULL &&
		node->Children[2] != NULL && node->Children[3] != NULL)
	{
		// node

		// check frustum intersection with the bounds of the node
		// if the node is not in the camera frustum, no need to render it
		// todo: transform terrain 
		if (!_Frustum.Contains(node->BoundingBox))
		{
			return;
		}

		RenderNode(node->Children[0]);
		RenderNode(node->Children[1]);
		RenderNode(node->Children[2]);
		RenderNode(node->Children[3]);
	}
	else
	{
		// leaf
		_VisibleNodes++;

		int x, y;
		int32 width = node->BoundingBox.GetDimensions().X;
		int32 height = node->BoundingBox.GetDimensions().Z;

		HeightField* field = _Terrain->GetHeightField();
		int32 fieldWidth = field->GetWidth();
		int32 fieldHeight = field->GetHeight();

		//int32 indexCount = width * height * 3 * 2;
		int32 indexCount = (fieldWidth-1) * (fieldHeight-1) * 3 * 2 >> (_Depth+1);

		int32* ibData;
		if (!_indexBuffer->Map(HardwareBufferMode_Normal, (void**)&ibData))
		{
			return;
		}

		int32 index = node->BoundingBox.Min.Z*fieldHeight+node->BoundingBox.Min.X;
		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width; x++)
			{
				*ibData++ = index;
				*ibData++ = index + fieldWidth;
				*ibData++ = index + 1;

				*ibData++ = index + 1;
				*ibData++ = index + fieldWidth;
				*ibData++ = index + fieldWidth + 1;

				index++;
			}

			index += fieldWidth-(width/_Terrain->GetFieldScale().X);
		}

		_indexBuffer->Unmap();

		MeshPart* meshPart = _mesh->GetMeshPart(0);

		IndexData* indexData = new IndexData();
		indexData->IndexBuffer = _indexBuffer;
		indexData->IndexCount = indexCount;
		meshPart->SetIndexData(indexData);
		meshPart->SetPrimitiveCount(RenderData::GetPrimitiveCount(meshPart->GetPrimitiveType(), indexCount));

		RenderData renderData;
		meshPart->GetRenderData(renderData);
		renderer->Render(&renderData);
	}
}

}
