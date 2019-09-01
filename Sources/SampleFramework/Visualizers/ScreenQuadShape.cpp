/*=============================================================================
ScreenQuadShape.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ScreenQuadShape.h"

namespace SampleFramework
{

ScreenQuadShape::ScreenQuadShape() :
	_mesh(NULL)
{
}

ScreenQuadShape::~ScreenQuadShape()
{
}

void ScreenQuadShape::Build()
{
	_mesh = NULL;

	RenderSystem* renderSystem = RenderSystem::Current();

	VertexLayout* vertexLayout;
	if (!RenderSystem::Current()->CreateVertexLayout(&vertexLayout))
	{
		return;
	}

	uint16 offset = 0;
	vertexLayout->AddElement(VertexElement(0, offset,
		VertexFormat_Float4, VertexSemantic_PositionTransformed));
	offset += VertexElement::GetTypeSize(VertexFormat_Float4);
	vertexLayout->AddElement(VertexElement(0, offset,
		VertexFormat_Float2, VertexSemantic_TextureCoordinate, 0));
	offset += VertexElement::GetTypeSize(VertexFormat_Float2);
	vertexLayout->AddElement(VertexElement(0, offset,
		VertexFormat_Float2, VertexSemantic_TextureCoordinate, 1));
	offset += VertexElement::GetTypeSize(VertexFormat_Float2);

	struct ScreenQuadGeometry
	{
		Vector4 PositionT;
		Vector2 TexCoord0;
		Vector2 TexCoord1;
	};

	Viewport viewport = renderSystem->GetViewport();
	int32 width = viewport.GetWidth();
	int32 height = viewport.GetHeight();

	ScreenQuadGeometry geometry[] =
	{
		Vector4(-0.5,      -0.5,       1.0, 1.0), Vector2(0.0, 0.0), Vector2(0.0, 0.0),
		Vector4(width-0.5, -0.5,       1.0, 1.0), Vector2(1.0, 0.0), Vector2(1.0, 0.0),
		Vector4(-0.5,      height-0.5, 1.0, 1.0), Vector2(1.0, 0.0), Vector2(1.0, 0.0),
		Vector4(width-0.5, height-0.5, 1.0, 1.0), Vector2(1.0, 1.0), Vector2(1.0, 1.0)
	};

	uint32 vertexCount = 4;
	HardwareBuffer* vertexBuffer;
	if (!renderSystem->CreateVertexBuffer(vertexCount * vertexLayout->GetSize(),
		HardwareBufferUsage_Static, &vertexBuffer))
	{
		delete vertexLayout;
		return;
	}

	SEbyte* vbData;
	vertexBuffer->Map(HardwareBufferMode_Normal, (void**)&vbData);
	memcpy(vbData, geometry, vertexCount * vertexLayout->GetSize());
	vertexBuffer->Unmap();

	MeshPart* meshPart = new MeshPart();

	VertexData* vertexData = new VertexData();
	vertexData->VertexLayout = vertexLayout;
	vertexData->VertexCount = vertexCount;
	vertexData->VertexStreams.Add(VertexStream(vertexBuffer, vertexLayout->GetSize()));
	meshPart->SetVertexData(vertexData);
	meshPart->SetPrimitiveTypeAndCount(PrimitiveType_TriangleStrip, vertexCount);

	_mesh = new Mesh();
	_mesh->AddMeshPart(meshPart);
}

}
