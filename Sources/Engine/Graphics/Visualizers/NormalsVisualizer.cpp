/*=============================================================================
NormalsVisualizer.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "NormalsVisualizer.h"
#include "Graphics/System/RenderSystem.h"
#include "Graphics/SceneManager.h"
#include "Graphics/Materials/DefaultMaterial.h"

namespace SonataEngine
{

NormalsVisualizer::NormalsVisualizer() :
	Visualizer(),
	_mesh(NULL),
	_NormalMesh(NULL),
	_Length(30.0f),
	_Color(Color32::Black)
{
}

NormalsVisualizer::~NormalsVisualizer()
{
	//SE_DELETE(_mesh);
	SE_DELETE(_NormalMesh);
}

void NormalsVisualizer::Render()
{
	if (_mesh != NULL && _NormalMesh == NULL)
	{
		_BuildNormalMesh();
	}

	if (_NormalMesh != NULL)
	{
		SceneManager::Instance()->RenderMesh(_NormalMesh, Matrix4::Identity);
	}
}

void NormalsVisualizer::_BuildNormalMesh()
{
	SE_DELETE(_NormalMesh);

	if (_mesh == NULL || _mesh->GetMeshPart(0) == NULL)
		return;

	MeshPart* srcPrimitive = _mesh->GetMeshPart(0);
	if (srcPrimitive->GetVertexData()->VertexCount == 0)
		return;

	VertexData* srcVertexData = srcPrimitive->GetVertexData();
	VertexLayout* srcLayout = srcVertexData->VertexLayout;
	uint32 vertexCount = srcVertexData->VertexCount;
	HardwareBuffer* vertexBuffer = srcVertexData->VertexStreams[0].VertexBuffer;

	if (srcLayout == NULL || vertexCount == 0)
		return;

	VertexLayout::VertexElementList::Iterator it = srcLayout->GetElementIterator();
	VertexElement positionElement;
	VertexElement normalElement;
	while (it.Next())
	{
		const VertexElement& element = it.Current();
		if (element.GetVertexSemantic() == VertexSemantic_Position)
			positionElement = element;
		else if (element.GetVertexSemantic() == VertexSemantic_Normal)
			normalElement = element;
	}

	VertexLayout* vertexLayout = new VertexLayout();
	uint16 offset = 0;
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Position));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);

	uint32 normalsCount;
	if (srcPrimitive->IsIndexed())
		normalsCount = 2 * srcPrimitive->GetIndexData()->IndexCount;
	else
		normalsCount = vertexCount;

	HardwareBuffer* normalsBuffer;
	if (!RenderSystem::Current()->CreateVertexBuffer(
		normalsCount * vertexLayout->GetSize(), HardwareBufferUsage_Static, &normalsBuffer))
	{
		delete vertexLayout;
		return;
	}

	SEbyte* meshvbData;
	vertexBuffer->Map(HardwareBufferMode_ReadOnly, (void**)&meshvbData);

	SEbyte* normalsvbData;
	normalsBuffer->Map(HardwareBufferMode_Normal, (void**)&normalsvbData);

	uint32 size, index;
	size = srcLayout->GetSize();

	for (uint i = 0; i < vertexCount; i++)
	{
		index = i * size + positionElement.GetOffset();
		Vector3 position = *(Vector3*)&meshvbData[index];

		index = i * size + normalElement.GetOffset();
		Vector3 normal = *(Vector3*)&meshvbData[index];

		Memory::Copy(normalsvbData, position, sizeof(Vector3));
		normalsvbData += sizeof(Vector3);

		Memory::Copy(normalsvbData, position + _Length * normal, sizeof(Vector3));
		normalsvbData += sizeof(Vector3);
	}

	normalsBuffer->Unmap();
	vertexBuffer->Unmap();

	MeshPart* meshPart = new MeshPart();

	VertexData* vertexData = new VertexData();
	vertexData->VertexLayout = vertexLayout;
	vertexData->VertexStreams.Add(VertexStream(normalsBuffer, vertexLayout->GetSize()));
	vertexData->VertexCount = normalsCount;
	meshPart->SetVertexData(vertexData);
	meshPart->SetPrimitiveTypeAndCount(PrimitiveType_LineList, normalsCount);

	DefaultMaterial* shader = new DefaultMaterial();
	FFPPass* pass = (FFPPass*)shader->GetTechnique()->GetPassByIndex(0);
	pass->LightState.Lighting = false;
	pass->RasterizerState.FillMode = FillMode_WireFrame;

	_NormalMesh = new Mesh();
	_NormalMesh->AddMeshPart(meshPart);
	meshPart->SetShader(shader);
}

}
