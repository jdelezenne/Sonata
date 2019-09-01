/*=============================================================================
SpriteParticle.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SpriteParticle.h"
#include "Graphics/System/RenderSystem.h"
#include "Graphics/SceneManager.h"
#include "Graphics/Particle/ParticleSystem.h"
#include "Graphics/Scene/Camera.h"
#include "Graphics/Model/Mesh.h"

namespace SonataEngine
{

SpriteParticle::SpriteParticle() :
	ParticleTemplate()
{
	RenderSystem* renderer = RenderSystem::Current();

	SE_DELETE(_vertexLayout);

	VertexLayout* vertexLayout = NULL;
	if (!renderer->CreateVertexLayout(&vertexLayout))
	{
		SEthrow(Exception());
		return;
	}

	_vertexLayout = vertexLayout;

	uint16 offset = 0;
	_vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Position));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);
	_vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float2, VertexSemantic_TextureCoordinate));
	offset += VertexElement::GetTypeSize(VertexFormat_Float2);
	_vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Color, VertexSemantic_Color));
	offset += VertexElement::GetTypeSize(VertexFormat_Color);

	MeshPart* meshPart = new MeshPart();
	meshPart->SetPrimitiveType(PrimitiveType_TriangleList);
	meshPart->SetIndexed(true);

	_mesh = new Mesh();
	_mesh->AddMeshPart(meshPart);
}

SpriteParticle::~SpriteParticle()
{
}

void SpriteParticle::SetMaxParticles(uint32 maxParticles)
{
	ParticleTemplate::SetMaxParticles(maxParticles);

	RenderSystem* renderer = RenderSystem::Current();

	MeshPart* meshPart = _mesh->GetMeshPart(0);

	VertexData* vertexData = meshPart->GetVertexData();
	meshPart->SetVertexData(NULL);
	SE_DELETE(vertexData);

	IndexData* indexData = meshPart->GetIndexData();
	meshPart->SetIndexData(NULL);
	SE_DELETE(indexData);

	HardwareBuffer* vertexBuffer;
	uint32 vertexCount = maxParticles * 4;
	if (renderer->CreateVertexBuffer(vertexCount * _vertexLayout->GetSize(),
		HardwareBufferUsage_Static, &vertexBuffer))
	{
		vertexData = new VertexData();
		vertexData->VertexLayout = _vertexLayout;
		vertexData->VertexStreams.Add(VertexStream(vertexBuffer, _vertexLayout->GetSize()));
		vertexData->VertexCount = vertexCount;
		meshPart->SetVertexData(vertexData);
	}

	HardwareBuffer* indexBuffer;
	uint32 indexCount = maxParticles * 6;
	if (renderer->CreateIndexBuffer(indexCount * sizeof(uint32), IndexBufferFormat_Int32,
		HardwareBufferUsage_Static, &indexBuffer))
	{
		indexData = new IndexData();
		indexData->IndexBuffer = indexBuffer;
		indexData->IndexCount = indexCount;
		meshPart->SetIndexData(indexData);
	}
}

void SpriteParticle::RenderParticles(ParticleEmitter* emitter, const ParticleEmitter::ParticleArray& particles, ShaderMaterial* shader)
{
	RenderSystem* renderer = RenderSystem::Current();

	Matrix4 matWorld = emitter->GetParticleSystem()->GetWorldTransform();

	// Compute the world transform of the billboard set
	Quaternion q = SceneManager::Instance()->GetCamera()->GetWorldOrientation();
	Matrix4 m = Matrix4::CreateFromQuaternion(q);
	Vector3 a = m.GetColumn(2);
	Quaternion q2(a, 0);
	q *= q2;
	Matrix4 mat = Matrix4::CreateFromQuaternion(q);
	mat.SetTranslation(Vector3(0,0,0));
	renderer->SetWorldTransform(mat);

	MeshPart* meshPart = _mesh->GetMeshPart(0);
	HardwareBuffer* vertexBuffer = meshPart->GetVertexData()->VertexStreams[0].VertexBuffer;
	HardwareBuffer* indexBuffer = meshPart->GetIndexData()->IndexBuffer;
	uint32 vertexCount = 0;
	uint32 indexCount = 0;

	struct SpriteGeometry
	{
		Vector3 Position;
		Vector2 TexCoord;
		uint32 Color;
	};

	SpriteGeometry* vbData;
	vertexBuffer->Map(HardwareBufferMode_Normal, (void**)&vbData);

	uint32* ibData;
	indexBuffer->Map(HardwareBufferMode_Normal, (void**)&ibData);

	ParticleEmitter::ParticleArray::Iterator it = particles.GetIterator();
	while (it.Next())
	{
		Particle* particle = it.Current();

		if (!particle->IsAlive())
			continue;

		Vector3 position = particle->GetPosition();
		Vector3 size = particle->GetSize();
		Color32 color = particle->GetColor().ToARGB();
		color.A = particle->GetAlpha();

		SpriteGeometry vertices[] =
		{
			position+Vector3(-size.X/2, -size.Y/2, 0.0), Vector2(0.0, 0.0), color,
			position+Vector3(size.X/2, -size.Y/2, 0.0), Vector2(1.0, 0.0), color,
			position+Vector3(size.X/2, size.Y/2, 0.0), Vector2(1.0, 1.0), color,
			position+Vector3(-size.X/2, size.Y/2, 0.0), Vector2(0.0, 1.0), color
		};

		// 3---2
		// | / |
		// 0---1
		uint32 indices[] =
		{
			vertexCount+0, vertexCount+1, vertexCount+2,
			vertexCount+0, vertexCount+2, vertexCount+3
		};

		Memory::Copy(vbData+vertexCount, vertices, 4 * sizeof(SpriteGeometry));
		Memory::Copy(ibData+indexCount, indices, 6 * sizeof(uint32));

		vertexCount += 4;
		indexCount += 6;
	}

	indexBuffer->Unmap();
	vertexBuffer->Unmap();

	if (vertexCount == 0)
		return;

	meshPart->GetVertexData()->VertexCount = vertexCount;
	meshPart->GetIndexData()->IndexCount = indexCount;

	meshPart->SetShader(shader);

	SceneManager::Instance()->RenderMesh(_mesh, Matrix4::Identity);
}

}
