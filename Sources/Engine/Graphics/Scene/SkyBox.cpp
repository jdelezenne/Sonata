/*=============================================================================
SkyBox.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SkyBox.h"
#include "Graphics/System/RenderSystem.h"
#include "Graphics/SceneManager.h"
#include "Graphics/Scene/Camera.h"
#include "Graphics/Materials/DefaultMaterial.h"

namespace SonataEngine
{

SE_IMPLEMENT_CLASS(SkyBox);
SE_IMPLEMENT_REFLECTION(SkyBox);

SkyBox::SkyBox() :
	Sky(),
	_distance(100.0f),
	_mesh(NULL)
{
	Memory::Set(_planes, SkyBoxPlane_Count * sizeof(Texture*), NULL);
}

SkyBox::~SkyBox()
{
	Destroy();

	for (int i = 0; i < SkyBoxPlane_Count; i++)
	{
		_planes[i] = NULL;
		_mesh = NULL;
	}
}

Texture* SkyBox::GetPlaneTexture(SkyBoxPlane plane) const
{
	if (plane < 0 || plane >= SkyBoxPlane_Count)
		return NULL;

	return _planes[plane];
}


void SkyBox::SetPlaneTexture(SkyBoxPlane plane, Texture* face)
{
	if (plane < 0 || plane >= SkyBoxPlane_Count)
		return;

	_planes[plane] = face;
	((FFPPass*)((DefaultMaterial*)_mesh->GetMeshPart(plane)->GetShader())->GetTechnique()->
		GetPassByIndex(0))->GetSamplerStateByIndex(0)->SetTexture(_planes[plane]);
}

bool SkyBox::Create()
{
	Destroy();

	RenderSystem* renderer = RenderSystem::Current();
	if (renderer == NULL)
	{
		return false;
	}

	int i;
	VertexLayout* vertexLayout = NULL;
	HardwareBuffer* indexBuffer = NULL;

	if (!renderer->CreateVertexLayout(&vertexLayout))
	{
		return false;
	}

	uint16 offset = 0;
	vertexLayout->AddElement(VertexElement(0, offset,
		VertexFormat_Float3, VertexSemantic_Position));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);
	vertexLayout->AddElement(VertexElement(0, offset,
		VertexFormat_Float2, VertexSemantic_TextureCoordinate));
	offset += VertexElement::GetTypeSize(VertexFormat_Float2);

	struct SkyGeometry
	{
		Vector3 Position;
		Vector2 TexCoord;
	};

	SkyGeometry vertices[] =
	{
		//left
		Vector3( 1,-1,-1), Vector2(0.0, 1.0),
		Vector3( 1,-1, 1), Vector2(1.0, 1.0),
		Vector3( 1, 1, 1), Vector2(1.0, 0.0),
		Vector3( 1, 1,-1), Vector2(0.0, 0.0),

		//right
		Vector3(-1,-1, 1), Vector2(0.0, 1.0),
		Vector3(-1,-1,-1), Vector2(1.0, 1.0),
		Vector3(-1, 1,-1), Vector2(1.0, 0.0),
		Vector3(-1, 1, 1), Vector2(0.0, 0.0),

		//top
		Vector3( 1, 1, 1), Vector2(0.0, 1.0),
		Vector3(-1, 1, 1), Vector2(1.0, 1.0),
		Vector3(-1, 1,-1), Vector2(1.0, 0.0),
		Vector3( 1, 1,-1), Vector2(0.0, 0.0),

		//bottom
		Vector3(-1,-1, 1), Vector2(0.0, 0.0),
		Vector3( 1,-1, 1), Vector2(1.0, 0.0),
		Vector3( 1,-1,-1), Vector2(1.0, 1.0),
		Vector3(-1,-1,-1), Vector2(0.0, 1.0),

		//front
		Vector3(-1,-1, 1), Vector2(1.0, 1.0),
		Vector3( 1,-1, 1), Vector2(0.0, 1.0),
		Vector3( 1, 1, 1), Vector2(0.0, 0.0),
		Vector3(-1, 1, 1), Vector2(1.0, 0.0),

		//back
		Vector3( 1,-1,-1), Vector2(1.0, 1.0),
		Vector3(-1,-1,-1), Vector2(0.0, 1.0),
		Vector3(-1, 1,-1), Vector2(0.0, 0.0),
		Vector3( 1, 1,-1), Vector2(1.0, 0.0)
	};

	// 3---2
	// | / |
	// 0---1
	uint16 indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	HardwareBuffer* vertexBuffers[SkyBoxPlane_Count];
	Memory::Set(vertexBuffers, SkyBoxPlane_Count * sizeof(HardwareBuffer*), NULL);

	SEtry
	{
		int16 vertexCount = 4;
		for (i = 0; i < SkyBoxPlane_Count; i++)
		{
			if (!renderer->CreateVertexBuffer(vertexCount * vertexLayout->GetSize(),
				HardwareBufferUsage_Static, &vertexBuffers[i]))
			{
				SEthrow(Exception());
			}

			SEbyte* vbData;
			vertexBuffers[i]->Map(HardwareBufferMode_Normal, (void**)&vbData);
			Memory::Copy(vbData, &vertices[i*vertexCount], vertexCount * vertexLayout->GetSize());
			vertexBuffers[i]->Unmap();
		}

		int16 indexCount = 6;
		if (!renderer->CreateIndexBuffer(indexCount * sizeof(int16), IndexBufferFormat_Int16,
			HardwareBufferUsage_Static, &indexBuffer))
		{
			SEthrow(Exception());
		}

		SEbyte* ibData;
		indexBuffer->Map(HardwareBufferMode_Normal, (void**)&ibData);
		Memory::Copy(ibData, indices, indexCount * sizeof(int16));
		indexBuffer->Unmap();

		IndexData* indexData = new IndexData();
		indexData->IndexBuffer = indexBuffer;
		indexData->IndexCount = indexCount;

		_mesh = new Mesh();

		for (i = 0; i < SkyBoxPlane_Count; i++)
		{
			MeshPart* meshPart = new MeshPart();

			VertexData* vertexData = new VertexData();
			vertexData->VertexLayout = vertexLayout;
			vertexData->VertexStreams.Add(VertexStream(vertexBuffers[i], vertexLayout->GetSize()));
			vertexData->VertexCount = vertexCount;
			meshPart->SetVertexData(vertexData);

			meshPart->SetIndexData(indexData);
			meshPart->SetIndexed(true);
			meshPart->SetPrimitiveTypeAndCount(PrimitiveType_TriangleList, indexCount);

			_mesh->AddMeshPart(meshPart);

			DefaultMaterial* shader = new DefaultMaterial();
			FFPPass* pass = (FFPPass*)shader->GetTechnique()->GetPassByIndex(0);
			pass->RasterizerState.CullMode = CullMode_None;
			pass->LightState.Lighting = false;
			pass->DepthState.Enable = false;
			pass->DepthState.WriteEnable = false;

			SamplerState* samplerState = pass->GetSamplerStateByIndex(0);
			samplerState->AddressModeU = TextureAddressMode_Clamp;
			samplerState->AddressModeV = TextureAddressMode_Clamp;
			samplerState->SetTexture(_planes[i]);

			meshPart->SetShader(shader);
		}

		return true;
	}
	SEcatch (Exception)
	{
		SE_DELETE(vertexLayout);
		for (i = 0; i < SkyBoxPlane_Count; i++)
			SE_DELETE(vertexBuffers[i]);
		SE_DELETE(indexBuffer);
		return false;
	}
}

void SkyBox::Destroy()
{
}

void SkyBox::Update(const TimeValue& timeValue)
{
}

void SkyBox::Render()
{
	if (_mesh == NULL)
		return;

	Vector3 position = Vector3::Zero;
	if (SceneManager::Instance()->GetCamera() != NULL)
	{
		position = SceneManager::Instance()->GetCamera()->GetWorldPosition();
	}

	Transform3 transform(position, Vector3(_distance), Quaternion::Identity);
	SceneManager::Instance()->RenderMesh(_mesh, transform.GetMatrix());
}

}
