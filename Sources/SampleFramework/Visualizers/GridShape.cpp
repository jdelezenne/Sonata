/*=============================================================================
GridShape.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "GridShape.h"

namespace SampleFramework
{

GridShape::GridShape() :
	_model(NULL),
	_gridStyle(GridStyle_line),
	_gridPlanes(GridPlane_ZX),
	_gridSize(50),
	_gridSpacing(2.0f),
	_breakUnit(0.0f),
	_highlightBreak(0.0f)
{
}

GridShape::~GridShape()
{
	_model = NULL;
}

void GridShape::Build()
{
	_model = NULL;

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

	struct GridGeometry
	{
		GridGeometry() {}
		GridGeometry(const Vector3& p, const Color32& c) : Position(p), Color(c.ToARGB()) {}
		Vector3 Position;
		uint32 Color;
	};

	uint32 vertexCount = _gridSize * 2.0 * 2.0;
	GridGeometry* geometry = new GridGeometry[vertexCount];

	real32 length = (_gridSize - 1) * _gridSpacing / 2.0f;

	int index = 0;
	int i;
	for (i = 0; i < _gridSize; ++i)
	{
		geometry[index++] = GridGeometry(Vector3(-length + i * _gridSpacing, 0.0, length), _color);
		geometry[index++] = GridGeometry(Vector3(-length + i * _gridSpacing, 0.0, -length), _color);
	}

	for (i = 0; i < _gridSize; ++i)
	{
		geometry[index++] = GridGeometry(Vector3(length, 0.0, -length + i * _gridSpacing), _color);
		geometry[index++] = GridGeometry(Vector3(-length, 0.0, -length + i * _gridSpacing), _color);
	}

	HardwareBuffer* vertexBuffer;
	if (!RenderSystem::Current()->CreateVertexBuffer(vertexCount * vertexLayout->GetSize(),
		HardwareBufferUsage_Static, &vertexBuffer))
	{
		delete vertexLayout;
		delete geometry;
		return;
	}

	SEbyte* vbData;
	vertexBuffer->Map(HardwareBufferMode_Normal, (void**)&vbData);
	memcpy(vbData, geometry, vertexCount * vertexLayout->GetSize());
	vertexBuffer->Unmap();

	delete geometry;

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

	if (_gridStyle == GridStyle_dot)
		meshPart->SetPrimitiveTypeAndCount(PrimitiveType_PointList, vertexCount);
	else// if (_gridStyle == GridStyle_line)
		meshPart->SetPrimitiveTypeAndCount(PrimitiveType_LineList, vertexCount);

	_model = new Model();
	Skeleton* skeleton = new Skeleton();
	_model->SetSkeleton(skeleton);

	Bone* frameXY = skeleton->AddBone();

	Bone* frameYZ = skeleton->AddBone();
	frameYZ->SetPoseTransform(Vector3::Zero, Quaternion::CreateFromAxisAngle(Vector3::UnitZ, Math::PiByTwo), Vector3::One);

	Bone* frameZX = skeleton->AddBone();
	frameZX->SetPoseTransform(Vector3::Zero, Quaternion::CreateFromAxisAngle(Vector3::UnitX, Math::PiByTwo), Vector3::One);

	Mesh* meshXY = new Mesh();
	meshXY->AddMeshPart(meshPart);
	meshXY->SetParentBone(frameXY);
	_model->AddMesh(meshXY);

	Mesh* meshYZ = new Mesh();
	meshYZ->AddMeshPart(meshPart);
	meshYZ->SetParentBone(frameYZ);
	_model->AddMesh(meshYZ);

	Mesh* meshZX = new Mesh();
	meshZX->AddMeshPart(meshPart);
	meshZX->SetParentBone(frameZX);
	_model->AddMesh(meshZX);

	meshPart->SetShader(shader);
}

void GridShape::Render()
{
	SceneManager* sceneManager = SceneManager::Instance();
	//TODO: enable state for meshes
	if (_gridPlanes & GridPlane_XY)
		sceneManager->RenderMesh(_model->GetMeshByIndex(0), Matrix4::Identity);
	if (_gridPlanes & GridPlane_YZ)
		sceneManager->RenderMesh(_model->GetMeshByIndex(1), Matrix4::Identity);
	if (_gridPlanes & GridPlane_ZX)
		sceneManager->RenderMesh(_model->GetMeshByIndex(2), Matrix4::Identity);
}

}
