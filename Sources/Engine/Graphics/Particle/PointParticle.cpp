/*=============================================================================
PointParticle.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "PointParticle.h"
#include "Graphics/System/RenderSystem.h"
#include "Graphics/SceneManager.h"
#include "Graphics/Particle/ParticleSystem.h"
#include "Graphics/Scene/Camera.h"
#include "Graphics/Model/Mesh.h"
#include "Graphics/Materials/DefaultMaterial.h"

namespace SonataEngine
{

PointParticle::PointParticle() :
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
	_vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float1, VertexSemantic_PSize));
	offset += VertexElement::GetTypeSize(VertexFormat_Float1);
	_vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Color, VertexSemantic_Color));
	offset += VertexElement::GetTypeSize(VertexFormat_Color);

	MeshPart* meshPart = new MeshPart();
	meshPart->SetPrimitiveType(PrimitiveType_PointList);

	_mesh = new Mesh();
	_mesh->AddMeshPart(meshPart);
}

PointParticle::~PointParticle()
{
}

void PointParticle::SetMaxParticles(uint32 maxParticles)
{
	ParticleTemplate::SetMaxParticles(maxParticles);

	RenderSystem* renderer = RenderSystem::Current();

	MeshPart* meshPart = _mesh->GetMeshPart(0);

	VertexData* vertexData = meshPart->GetVertexData();
	meshPart->SetVertexData(NULL);
	SE_DELETE(vertexData);

	uint32 vertexCount = maxParticles;
	HardwareBuffer* vertexBuffer;
	if (renderer->CreateVertexBuffer(vertexCount * _vertexLayout->GetSize(),
		HardwareBufferUsage_Static, &vertexBuffer))
	{
		vertexData = new VertexData();
		vertexData->VertexLayout = _vertexLayout;
		vertexData->VertexStreams.Add(VertexStream(vertexBuffer, _vertexLayout->GetSize()));
		vertexData->VertexCount = vertexCount;
		meshPart->SetVertexData(vertexData);
	}
}

void PointParticle::InitParticle(ParticleEmitter* emitter, Particle* particle)
{
	ParticleTemplate::InitParticle(emitter, particle);
}

void PointParticle::RenderParticles(ParticleEmitter* emitter, const ParticleEmitter::ParticleArray& particles, ShaderMaterial* shader)
{
	RenderSystem* renderer = RenderSystem::Current();

	Matrix4 matWorld = emitter->GetParticleSystem()->GetWorldTransform();

	// Set the world transform
	renderer->SetWorldTransform(Matrix4::Identity);

	uint32 textureSize = 1;
	FFPPass* pass = (FFPPass*)((DefaultMaterial*)shader)->GetTechnique()->GetPassByIndex(0);
	if (pass->GetTextureStateCount() > 0)
	{
		Texture* texture = pass->GetSamplerStateByIndex(0)->GetTexture();
		if (texture != NULL)
		{
			uint32 width = texture->GetWidth();
			uint32 height = texture->GetHeight();
			textureSize = width < height ? width : height;
		}
	}

	MeshPart* meshPart = _mesh->GetMeshPart(0);
	HardwareBuffer* vertexBuffer = meshPart->GetVertexData()->VertexStreams[0].VertexBuffer;
	uint32 vertexCount = 0;

	struct PointSpriteGeometry
	{
		Vector3 Position;
		real32 PSize;
		uint32 Color;
	};

	PointSpriteGeometry* vbData;
	vertexBuffer->Map(HardwareBufferMode_WriteOnly, (void**)&vbData);

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

		size *= textureSize;

		PointSpriteGeometry vertices[] =
		{
			position, size.X, color
		};

		Memory::Copy(vbData+vertexCount, vertices, sizeof(PointSpriteGeometry));
		vertexCount++;
	}

	vertexBuffer->Unmap();

	if (vertexCount == 0)
		return;

	meshPart->GetVertexData()->VertexCount = vertexCount;
	meshPart->SetShader(shader);

	PointState state;
	state.Enable = true;
	state.Size = textureSize;
	state.ScaleEnable = true;
	state.ConstantScale = 1.0f;//0.0f;
	state.LinearScale = 0.0f;
	state.QuadraticScale = 0.01f;//1.0f;
	renderer->SetPointState(state);

	SceneManager::Instance()->RenderMesh(_mesh, Matrix4::Identity);

	state.Enable = false;
	state.ScaleEnable = false;
	renderer->SetPointState(state);
}

}
