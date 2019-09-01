/*=============================================================================
SkeletonShape.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SkeletonShape.h"

namespace SampleFramework
{

SkeletonShape::SkeletonShape() :
	_mesh(NULL),
	_skeleton(NULL),
	_skin(NULL),
	_color(Color32::White)
{
}

SkeletonShape::~SkeletonShape()
{
	SE_DELETE(_mesh);
}

void SkeletonShape::Build()
{
	SE_DELETE(_mesh);

	if (_skeleton == NULL || _skeleton->GetRootBone() == NULL || _skin == NULL)
	{
		return;
	}

	VertexLayout* vertexLayout;
	if (!RenderSystem::Current()->CreateVertexLayout(&vertexLayout))
	{
		return;
	}

	vertexLayout->SetElements(VertexPositionColor::VertexElements, VertexPositionColor::ElementCount);

#if 0
	uint32 vertexCount = (_skeleton->GetBoneCount() - 1) * 2;
#else
	uint32 vertexCount = _skin->SkinVertices.Count() * 2;
#endif
	HardwareBuffer* vertexBuffer;
	if (!RenderSystem::Current()->CreateVertexBuffer(vertexCount * vertexLayout->GetSize(),
		HardwareBufferUsage_Dynamic, &vertexBuffer))
	{
		SE_DELETE(vertexLayout);
		return;
	}

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

void SkeletonShape::Update()
{
	if (_mesh == NULL)
	{
		return;
	}

	MeshPart* meshPart = _mesh->GetMeshPart(0);
	HardwareBuffer* vertexBuffer = meshPart->GetVertexData()->VertexStreams[0].VertexBuffer;

	SEbyte* vertices;
	vertexBuffer->Map(HardwareBufferMode_Normal, (void**)&vertices);
#if 0
	for (int i = 0; i < _skeleton->GetBoneCount(); ++i)
	{
		BuildBone(_skeleton->GetBoneByIndex(i), &vertices);
	}
#else
	for (int i = 0; i < _skin->SkinVertices.Count(); ++i)
	{
		BuildBone(_skeleton->GetBoneByIndex(_skin->SkinVertices[i].BoneIndex), &vertices);
	}
#endif
	vertexBuffer->Unmap();
}

void SkeletonShape::BuildBone(Bone* bone, SEbyte** vertices)
{
	if (bone->GetParent() != NULL)
	{
#if 0
		Vector3 p0 = Vector3::Transform(Vector3::Zero, bone->GetPoseTransform());
		Vector3 p1 = Vector3::Transform(Vector3::Zero, bone->GetParent()->GetPoseTransform());
#else
		Vector3 p0 = Vector3::Transform(Vector3::Zero, bone->GetGlobalTransform());
		Vector3 p1 = Vector3::Transform(Vector3::Zero, bone->GetParent()->GetGlobalTransform());
#endif

		VertexPositionColor boneLine[] =
		{
			VertexPositionColor(p0, _color.ToARGB()),
			VertexPositionColor(p1, _color.ToARGB())
		};

		Memory::Copy(*vertices, boneLine, VertexPositionColor::SizeInBytes * 2);
		*vertices += VertexPositionColor::SizeInBytes * 2;
	}
}

}
