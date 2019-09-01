/*=============================================================================
QuadShape.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "QuadShape.h"

namespace SampleFramework
{

QuadShape::QuadShape() :
	_mesh(NULL),
	_quadSize(1.0f),
	_color(Color32::White)
{
}

QuadShape::~QuadShape()
{
	_mesh = NULL;
}

void QuadShape::Build()
{
	_mesh = NULL;

	VertexLayout* vertexLayout;
	if (!RenderSystem::Current()->CreateVertexLayout(&vertexLayout))
	{
		return;
	}

	uint16 offset = 0;
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Position));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float2, VertexSemantic_TextureCoordinate));
	offset += VertexElement::GetTypeSize(VertexFormat_Float2);
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Color, VertexSemantic_Color));
	offset += VertexElement::GetTypeSize(VertexFormat_Color);

	struct QuadGeometry
	{
		Vector3 Position;
		Vector2 TexCoord;
		uint32 Color;
	};

	QuadGeometry geometry[] =
	{
		Vector3(0.0, _quadSize, 0.0), Vector2(0.0, 0.0), _color.ToARGB(),
		Vector3(0.0, 0.0, 0.0), Vector2(0.0, 1.0), _color.ToARGB(),
		Vector3(_quadSize, 0.0, 0.0), Vector2(1.0, 1.0), _color.ToARGB(),
		Vector3(_quadSize, _quadSize, 0.0), Vector2(1.0, 0.0), _color.ToARGB()
	};

	uint32 vertexCount = 4;
	HardwareBuffer* vertexBuffer;
	if (!RenderSystem::Current()->CreateVertexBuffer(vertexCount * vertexLayout->GetSize(),
		HardwareBufferUsage_Static, &vertexBuffer))
	{
		delete vertexLayout;
		return;
	}

	SEbyte* vbData;
	vertexBuffer->Map(HardwareBufferMode_Normal, (void**)&vbData);
	Memory::Copy(vbData, geometry, vertexCount * vertexLayout->GetSize());
	vertexBuffer->Unmap();

	HardwareBuffer* indexBuffer;
	uint32 indexCount = 4;
	if (!RenderSystem::Current()->CreateIndexBuffer(indexCount * sizeof(uint16),
		IndexBufferFormat_Int16, HardwareBufferUsage_Static, &indexBuffer))
	{
		return;
	}

	uint16* ibData;
	if (!indexBuffer->Map(HardwareBufferMode_Normal, (void**)&ibData))
		return;

	// 0---3
	// | / |
	// 1---2
	uint16 indices[] =
	{
		0, 1, 3, 2
	};

	Memory::Copy(ibData, indices, indexCount * sizeof(uint16));
	indexBuffer->Unmap();

	DefaultMaterial* shader = new DefaultMaterial();
	FFPPass* pass = (FFPPass*)shader->GetTechnique()->GetPassByIndex(0);
	pass->LightState.Lighting = false;
	pass->DepthState.Enable = false;
	pass->DepthState.WriteEnable = false;

	MeshPart* meshPart = new MeshPart();

	VertexData* vertexData = new VertexData();
	vertexData->VertexLayout = vertexLayout;
	vertexData->VertexStreams.Add(VertexStream(vertexBuffer, vertexLayout->GetSize()));
	vertexData->VertexCount = vertexCount;
	meshPart->SetVertexData(vertexData);

	IndexData* indexData = new IndexData();
	indexData->IndexBuffer = indexBuffer;
	indexData->IndexCount = indexCount;
	meshPart->SetIndexData(indexData);
	meshPart->SetIndexed(true);
	meshPart->SetPrimitiveTypeAndCount(PrimitiveType_TriangleStrip, indexCount);

	_mesh = new Mesh();
	_mesh->AddMeshPart(meshPart);
	meshPart->SetShader(shader);
}
}
