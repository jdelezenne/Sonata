/*=============================================================================
StaticTerrainRenderer.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "StaticTerrainRenderer.h"
#include "Graphics/System/RenderSystem.h"
#include "Graphics/SceneManager.h"
#include "Graphics/Terrain/HeightField.h"
#include "Graphics/Terrain/Terrain.h"
#include "Graphics/Materials/DefaultMaterial.h"

namespace SonataEngine
{

StaticTerrainRenderer::StaticTerrainRenderer() :
	TerrainRenderer(),
	_mesh(NULL),
	_texture(NULL),
	_detailTexture(NULL),
	_textureScale(Vector2::One),
	_detailTextureScale(Vector2::One)
{
}

StaticTerrainRenderer::~StaticTerrainRenderer()
{
}

bool StaticTerrainRenderer::Create()
{
	if (!_Terrain)
		return false;

	HeightField* field = _Terrain->GetHeightField();
	if (!field)
		return false;

	SE_DELETE(_mesh);

	_ComputeFaceNormals();
	_ComputeVertexNormals();

	VertexLayout* vertexLayout;
	if (!RenderSystem::Current()->CreateVertexLayout(&vertexLayout))
	{
		return false;
	}

	uint16 offset = 0;
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Position));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Normal));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);
	vertexLayout->AddElement(VertexElement(0, offset,
		VertexFormat_Float2, VertexSemantic_TextureCoordinate, 0));
	offset += VertexElement::GetTypeSize(VertexFormat_Float2);
	//vertexLayout->AddElement(VertexElement(0, offset,
	//	VertexFormat_Float2, VertexSemantic_TextureCoordinate, 1));
	//offset += VertexElement::GetTypeSize(VertexFormat_Float2);

	_mesh = new Mesh();

	MeshPart* meshPart = new MeshPart();
	meshPart->SetPrimitiveType(PrimitiveType_TriangleList);

	DefaultMaterial* shader = new DefaultMaterial();
	FFPPass* pass = (FFPPass*)shader->GetTechnique()->GetPassByIndex(0);

	pass->LightState.Lighting = true;
	//pass->RasterizerState.FillMode = FillMode_WireFrame;

	if (_texture != NULL)
	{
		SamplerState* samplerDiffuse = pass->GetSamplerStateByIndex(0);
		samplerDiffuse->SetTexture(_texture);
		pass->AddSamplerState(samplerDiffuse);

		TextureState* textureDiffuse = pass->GetTextureStateByIndex(0);
		textureDiffuse->TextureCoordinateIndex = 0;
		pass->AddTextureState(textureDiffuse);
	}

	if (_detailTexture != NULL)
	{
		SamplerState* samplerDetail = new SamplerState();
		samplerDetail->SetTexture(_detailTexture);
		pass->AddSamplerState(samplerDetail);

		TextureState* textureDetail = new TextureState();
		textureDetail->TextureCoordinateIndex = 1;
		textureDetail->ColorOperation = TextureOperation_AddSigned;
		textureDetail->ColorArgument1 = TextureArgument_TextureColor;
		textureDetail->ColorArgument2 = TextureArgument_Current;
		pass->AddTextureState(textureDetail);
	}

	_mesh->AddMeshPart(meshPart);
	meshPart->SetShader(shader);

	int x, y;
	int32 width = field->GetWidth();
	int32 height = field->GetHeight();

	int32 vertexCount = width * height;
	HardwareBuffer* vertexBuffer;
	if (RenderSystem::Current()->CreateVertexBuffer(vertexCount * vertexLayout->GetSize(), HardwareBufferUsage_Static, &vertexBuffer))
	{
		SEbyte* vbData;
		vertexBuffer->Map(HardwareBufferMode_Normal, (void**)&vbData);

		real32 sx, sy;
		real32 h;
		SEbyte color;
		real32 tu, tv;

		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width; x++)
			{
				_Terrain->GetScaledPosition(x, y, sx, sy);
				h = _Terrain->GetScaledHeight(x, y);
				color = field->GetHeight(x, y);
				tu = (real32) x / width;
				tv = (real32) y / height;

				Memory::Copy(vbData, Vector3(sx, h, sy), sizeof(Vector3));
				vbData += sizeof(Vector3);

				Vector3 normal = GetNormal(x, y);
				Memory::Copy(vbData, normal, sizeof(Vector3));
				vbData += sizeof(Vector3);

				Memory::Copy(vbData, Vector2(tu * _textureScale.X, tv * _textureScale.Y), sizeof(Vector2));
				vbData += sizeof(Vector2);

				//Memory::Copy(vbData, Vector2(tu * _detailTextureScale.X, tv * _detailTextureScale.Y), sizeof(Vector2));
				//vbData += sizeof(Vector2);
			}
		}

		vertexBuffer->Unmap();

		VertexData* vertexData = new VertexData();
		vertexData->VertexLayout = vertexLayout;
		vertexData->VertexStreams.Add(VertexStream(vertexBuffer, vertexLayout->GetSize()));
		vertexData->VertexCount = vertexCount;
		meshPart->SetVertexData(vertexData);
	}

	int32 indexCount = (width-1) * (height-1) * 3 * 2;
	HardwareBuffer* indexBuffer;
	if (RenderSystem::Current()->CreateIndexBuffer(indexCount * sizeof(int32),
		IndexBufferFormat_Int32, HardwareBufferUsage_Static, &indexBuffer))
	{
		int32* ibData;
		indexBuffer->Map(HardwareBufferMode_Normal, (void**)&ibData);

		int32 index = 0;
		for (y = 0; y < height-1; y++)
		{
			for (x = 0; x < width-1; x++)
			{
				*ibData++ = index;
				*ibData++ = index + width;
				*ibData++ = index + 1;

				*ibData++ = index + 1;
				*ibData++ = index + width;
				*ibData++ = index + width + 1;

				index++;
			}
			index++;
		}

		indexBuffer->Unmap();

		IndexData* indexData = new IndexData();
		indexData->IndexBuffer = indexBuffer;
		indexData->IndexCount = indexCount;
		meshPart->SetIndexData(indexData);
		meshPart->SetIndexed(true);
		meshPart->SetPrimitiveCount(RenderData::GetPrimitiveCount(meshPart->GetPrimitiveType(), indexCount));
	}

	return true;
}

void StaticTerrainRenderer::Render()
{
	if (_mesh != NULL)
	{
		SceneManager::Instance()->RenderMesh(_mesh, Matrix4::Identity);
	}
}

}
