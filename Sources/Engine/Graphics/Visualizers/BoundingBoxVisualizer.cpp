/*=============================================================================
BoundingBoxVisualizer.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "BoundingBoxVisualizer.h"
#include "Graphics/System/RenderSystem.h"
#include "Graphics/SceneManager.h"
#include "Graphics/Scene/SceneNode.h"
#include "Graphics/Scene/ModelNode.h"
#include "Graphics/Materials/DefaultMaterial.h"

namespace SonataEngine
{

BoundingBoxVisualizer::BoundingBoxVisualizer() :
	Visualizer(),
	_Object(NULL),
	_BBoxMesh(NULL),
	_Color(Color32::Black)
{
}

BoundingBoxVisualizer::~BoundingBoxVisualizer()
{
	//SE_DELETE(_Object);
	SE_DELETE(_BBoxMesh);
}

void BoundingBoxVisualizer::Render()
{
	if (_Object != NULL)
	{
		_BuildBBoxMesh();
	}

	if (_Object != NULL && _BBoxMesh != NULL)
	{
		SceneManager::Instance()->RenderMesh(_BBoxMesh, Matrix4::Identity);
	}
}

void BoundingBoxVisualizer::_BuildBBoxMesh()
{
	SE_DELETE(_BBoxMesh);

	if (_Object == NULL)
		return;

	VertexLayout* vertexLayout = new VertexLayout();
	uint16 offset = 0;
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Position));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);

	uint32 vertexCount = 8;
	HardwareBuffer* vertexBuffer;
	if (!RenderSystem::Current()->CreateVertexBuffer(
		vertexCount * vertexLayout->GetSize(), HardwareBufferUsage_Static, &vertexBuffer))
	{
		delete vertexLayout;
		return;
	}

	uint32 indexCount = 24;
	HardwareBuffer* indexBuffer;
	if (!RenderSystem::Current()->CreateIndexBuffer(
		indexCount * sizeof(uint32), IndexBufferFormat_Int32, HardwareBufferUsage_Static, &indexBuffer))
	{
		delete vertexLayout;
		delete vertexBuffer;
		return;
	}

	AABB bbox = _Object->GetWorldBoundingBox();

	//      2----------6
	//     /|         /|
	//    / |        / |
	//   3----------7  |
	//   |  0-------|--4
	//   | /        | /
	//   |/         |/
	//   1----------5
	Vector3 vertices[] =
	{
		Vector3(bbox.Min.X, bbox.Min.Y, bbox.Min.Z), //0
		Vector3(bbox.Min.X, bbox.Min.Y, bbox.Max.Z), //1
		Vector3(bbox.Min.X, bbox.Max.Y, bbox.Min.Z), //2
		Vector3(bbox.Min.X, bbox.Max.Y, bbox.Max.Z), //3
		Vector3(bbox.Max.X, bbox.Min.Y, bbox.Min.Z), //4
		Vector3(bbox.Max.X, bbox.Min.Y, bbox.Max.Z), //5
		Vector3(bbox.Max.X, bbox.Max.Y, bbox.Min.Z), //6
		Vector3(bbox.Max.X, bbox.Max.Y, bbox.Max.Z)  //7
	};

	// bbox lines
	uint32 indices[] =
	{
		// x axis
		0, 4,
		1, 5,
		2, 6,
		3, 7,

		// y axis
		0, 2,
		1, 3,
		4, 6,
		5, 7,

		// z axis
		0, 1,
		2, 3,
		4, 5,
		6, 7,
	};

	SEbyte* vbData;
	vertexBuffer->Map(HardwareBufferMode_Normal, (void**)&vbData);
	Memory::Copy(vbData, &vertices, vertexCount * vertexLayout->GetSize());
	vertexBuffer->Unmap();

	SEbyte* ibData;
	indexBuffer->Map(HardwareBufferMode_Normal, (void**)&ibData);
	Memory::Copy(ibData, &indices, indexCount * sizeof(uint32));
	indexBuffer->Unmap();

	MeshPart* meshPart = new MeshPart();
	meshPart->SetPrimitiveType(PrimitiveType_LineList);

	VertexData* vertexData = new VertexData();
	vertexData->VertexLayout = vertexLayout;
	vertexData->VertexCount = vertexCount;
	vertexData->VertexStreams.Add(VertexStream(vertexBuffer, vertexLayout->GetSize()));
	meshPart->SetVertexData(vertexData);

	IndexData* indexData = new IndexData();
	indexData->IndexCount = indexCount;
	indexData->IndexBuffer = indexBuffer;
	meshPart->SetIndexData(indexData);
	meshPart->SetIndexed(true);
	meshPart->SetPrimitiveCount(indexCount);

	DefaultMaterial* shader = new DefaultMaterial();
	FFPPass* pass = (FFPPass*)shader->GetTechnique()->GetPassByIndex(0);
	pass->MaterialState.DiffuseColor = _Color;
	pass->LightState.Lighting = false;
	pass->RasterizerState.FillMode = FillMode_WireFrame;

	_BBoxMesh = new Mesh();
	_BBoxMesh->AddMeshPart(meshPart);
	meshPart->SetShader(shader);
}

}
