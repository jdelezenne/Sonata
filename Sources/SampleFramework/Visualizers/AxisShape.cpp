/*=============================================================================
AxisShape.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "AxisShape.h"

namespace SampleFramework
{

AxisShape::AxisShape() :
	_mesh(NULL),
	_axisSize(1.0f),
	_labelSize(0.3f)
{
	_desc[Axes::XAxis]._isEnabled = true;
	_desc[Axes::XAxis]._color = Color32::Red;
	_desc[Axes::XAxis]._label = _T("X");

	_desc[Axes::YAxis]._isEnabled = true;
	_desc[Axes::YAxis]._color = Color32::Green;
	_desc[Axes::YAxis]._label = _T("Y");

	_desc[Axes::ZAxis]._isEnabled = true;
	_desc[Axes::ZAxis]._color = Color32::Blue;
	_desc[Axes::ZAxis]._label = _T("Z");
}

AxisShape::~AxisShape()
{
	_mesh = NULL;
}

void AxisShape::Build()
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
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Color, VertexSemantic_Color));
	offset += VertexElement::GetTypeSize(VertexFormat_Color);

	struct AxisGeometry
	{
		Vector3 Position;
		uint32 Color;
	};

	AxisGeometry geometry[] =
	{
		Vector3::Zero, _desc[Axes::XAxis]._color.ToARGB(),
		Vector3::UnitX * _axisSize, _desc[Axes::XAxis]._color.ToARGB(),
		Vector3::Zero, _desc[Axes::YAxis]._color.ToARGB(),
		Vector3::UnitY * _axisSize, _desc[Axes::YAxis]._color.ToARGB(),
		Vector3::Zero, _desc[Axes::ZAxis]._color.ToARGB(),
		Vector3::UnitZ * _axisSize, _desc[Axes::ZAxis]._color.ToARGB()
	};

	uint32 vertexCount = 6;
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

	DefaultMaterial* shader = new DefaultMaterial();
	FFPPass* pass = (FFPPass*)shader->GetTechnique()->GetPassByIndex(0);
	pass->MaterialState.VertexColor = true;
	pass->LightState.Lighting = false;
	pass->DepthState.Enable = false;
	pass->DepthState.WriteEnable = false;

	MeshPart* meshPart = new MeshPart();

	VertexData* vertexData = new VertexData();
	vertexData->VertexLayout = vertexLayout;
	vertexData->VertexStreams.Add(VertexStream(vertexBuffer, vertexLayout->GetSize()));
	vertexData->VertexCount = vertexCount;
	meshPart->SetVertexData(vertexData);
	meshPart->SetPrimitiveTypeAndCount(PrimitiveType_LineList, vertexCount);

	_mesh = new Mesh();
	_mesh->AddMeshPart(meshPart);
	meshPart->SetShader(shader);
}

}
