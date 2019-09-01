/*=============================================================================
SkyDome.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SkyDome.h"
#include "Graphics/System/RenderSystem.h"
#include "Graphics/SceneManager.h"
#include "Graphics/Scene/Camera.h"
#include "Graphics/Materials/DefaultMaterial.h"

namespace SonataEngine
{

SE_IMPLEMENT_CLASS(SkyDome);
SE_IMPLEMENT_REFLECTION(SkyDome);

struct SkyGeometry
{
	Vector3 Position;
	Vector2 TexCoord;
};

SkyDome::SkyDome() :
	Sky(),
	_radius(100.0f),
	_height(100.0f),
	_rings(15),
	_segments(15),
	_mapping(MappingMode_Spherical),
	_texture(NULL),
	_textureScale(Vector2::One),
	_mesh(NULL)
{
	_radius = 1000.0;
	_height = 1000.0;
}

SkyDome::~SkyDome()
{
	Destroy();
}

bool SkyDome::Create()
{
	Destroy();

	RenderSystem* renderer = RenderSystem::Current();
	if (renderer == NULL)
	{
		return false;
	}

	VertexLayout* vertexLayout = NULL;
	HardwareBuffer* vertexBuffer = NULL;
	HardwareBuffer* indexBuffer = NULL;
	MeshPart* meshPart = NULL;

	int32 rows = _rings+1;
	int32 columns = _segments+1;

	int16 vertexCount = rows * (columns+1);
	BaseArray<SkyGeometry> vertices(vertexCount);

	int16 indexCount = rows * columns * 2;
	BaseArray<int16> indices(indexCount);

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

	int index = 0;
	int x, y;
	for (y = 0; y <= columns; ++y)
	{
		real32 theta = Math::ToRadians((real32)((real32)y / columns * 360.0f));

		for (x = rows-1; x >= 0; --x)
		{
			real32 phi = Math::ToRadians((real32)((real32)x / rows * 90.0f));

			vertices[index].Position = Vector3(
				Math::Sin(phi) * Math::Cos(theta) * _radius,
				Math::Cos(phi) * _height,
				Math::Sin(phi) * Math::Sin(theta) * _radius);

			vertices[index].TexCoord = Vector2((real32)y / columns, (real32)x / rows);

			index++;
		}
	}

	bool leftToRight = false;

	index = 0;
	for (y = 0; y < columns; ++y)
	{
		if (leftToRight)
		{
			for (x = 0; x < rows; ++x)
			{
				indices[index++] = ((y + 1) * rows + x);
				indices[index++] = (y * rows + x);
			}
		}
		else
		{
			for (x = rows-1; x >= 0; --x)
			{
				indices[index++] = (y * rows + x);
				indices[index++] = ((y + 1) * rows + x);
			}
		}

		leftToRight = !leftToRight;
	}

	try
	{
		if (!renderer->CreateVertexBuffer(vertexCount * vertexLayout->GetSize(),
			HardwareBufferUsage_Static, &vertexBuffer))
		{
			throw;
		}

		SEbyte* vbData;
		vertexBuffer->Map(HardwareBufferMode_Normal, (void**)&vbData);
		Memory::Copy(vbData, &vertices[0], vertexCount * vertexLayout->GetSize());
		vertexBuffer->Unmap();

		if (!renderer->CreateIndexBuffer(indexCount * sizeof(int16), IndexBufferFormat_Int16,
			HardwareBufferUsage_Static, &indexBuffer))
		{
			throw;
		}

		SEbyte* ibData;
		indexBuffer->Map(HardwareBufferMode_Normal, (void**)&ibData);
		Memory::Copy(ibData, &indices[0], indexCount * sizeof(int16));
		indexBuffer->Unmap();

		meshPart = new MeshPart();

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

		DefaultMaterial* shader = new DefaultMaterial();
		FFPPass* pass = (FFPPass*)shader->GetTechnique()->GetPassByIndex(0);

		pass->RasterizerState.CullMode = CullMode_None;
		pass->LightState.Lighting = false;
		pass->DepthState.Enable = false;
		pass->DepthState.WriteEnable = false;

		SamplerState* samplerState = pass->GetSamplerStateByIndex(0);
		samplerState->SetTexture(_texture);
		samplerState->AddressModeU = TextureAddressMode_Clamp;
		samplerState->AddressModeV = TextureAddressMode_Clamp;

		_mesh = new Mesh();
		_mesh->AddMeshPart(meshPart);
		meshPart->SetShader(shader);

		return true;
	}
	catch (Exception)
	{
		SE_DELETE(vertexLayout);
		SE_DELETE(vertexBuffer);
		SE_DELETE(indexBuffer);
		return false;
	}
}

void SkyDome::Destroy()
{
}

void SkyDome::Update(const TimeValue& timeValue)
{
}

void SkyDome::Render()
{
	if (_mesh == NULL || _mesh->GetMeshPart(0) == NULL)
		return;

	Vector3 position = Vector3::Zero;

	if (SceneManager::Instance()->GetCamera() != NULL)
		position = SceneManager::Instance()->GetCamera()->GetWorldPosition();

	position.Y = -50.0;
	Transform3 transform(position, Vector3::One, Quaternion::Identity);

	SceneManager::Instance()->RenderMesh(_mesh, transform.GetMatrix());
}

}
