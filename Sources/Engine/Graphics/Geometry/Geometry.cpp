/*=============================================================================
Geometry.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Geometry.h"
#include "Graphics/System/RenderSystem.h"

namespace SonataEngine
{

Geometry::Geometry() :
	_primitiveType(PrimitiveType_TriangleList)
{
	_textureCoords.Resize(1);
}

Geometry::~Geometry()
{
	_vertices.Clear();
	_normals.Clear();
	_colors.Clear();
	_textureCoords.Clear();
	_indices.Clear();
}

void Geometry::SetTextureCoordUnits(int value)
{
	_textureCoords.SetCapacity(value);
}

Geometry::TextureCoordArray& Geometry::GetTextureCoords(int unit)
{
	if (unit < 0 || unit >= _textureCoords.Count())
	{
		throw new ArgumentOutOfRangeException("unit");
	}

	return _textureCoords[unit];
}

void Geometry::SetTextureCoords(const TextureCoordArray& value, int unit)
{
	if (unit < 0 || unit >= _textureCoords.Count())
	{
		throw new ArgumentOutOfRangeException("unit");
	}

	_textureCoords[unit] = value;
}

bool Geometry::IsValid()
{
	// need vertices
	if (_vertices.Count() == 0)
		return false;

	int vertexCount = _vertices.Count();

	// the number of elements is either the number of vertices of the number if indices
	int count;
	if (_indices.Count() == 0)
	{
		count = vertexCount;
	}
	else
	{
		count = _indices.Count();
	}

	// check the number of elements depending on the type of meshPart
	//int primCount = RenderData::GetPrimitiveCount(_primitiveType, count);
	if (_primitiveType != PrimitiveType_TriangleList) // fix: only supported type for now
		return false;

	// for each vertex element array provided, it must have the same quantity of elements
	if (_normals.Count() != 0 && _normals.Count() != vertexCount)
		return false;

	if (_colors.Count() != 0 && _colors.Count() != vertexCount)
		return false;

	int textureCoordsCount = _textureCoords.Count();
	for (int i = 0; i < textureCoordsCount; ++i)
	{
		if (_textureCoords[i].Count() != 0 && _textureCoords[i].Count() != vertexCount)
			return false;
	}

	return true;
}

void Geometry::ComputeNormals()
{
	_normals.Clear();

	if (!IsValid())
	{
		return;
	}

	bool indexed;
	int count;
	if (_indices.Count() == 0)
	{
		count = _vertices.Count();
		indexed = false;
	}
	else
	{
		count = _indices.Count();
		indexed = true;
	}

	int primCount = RenderData::GetPrimitiveCount(_primitiveType, count);

	_normals.Clear();

	for (int i = 0; i < count; ++i)
	{
		const Vector3 v = (indexed ? _vertices[i] : _vertices[_indices[i]]);
	}
}

void Geometry::UpdateBounds()
{
	if (!IsValid())
	{
		_boundingBox = BoundingBox::Empty;
		_boundingSphere = BoundingSphere::Empty;
		return;
	}

	int count = _vertices.Count();

	Geometry::VertexArray::Iterator it = _vertices.GetIterator();
	if (it.Next())
	{
		_boundingBox.Min = _boundingBox.Max = it.Current();
	}

	while (it.Next())
	{
		const Vector3& v = it.Current();
		_boundingBox.Min = Vector3::Min(_boundingBox.Min, v);
		_boundingBox.Max = Vector3::Max(_boundingBox.Max, v);
	}

	_boundingSphere = BoundingSphere::CreateFromBox(_boundingBox);
}

bool Geometry::CreatePrimitive(MeshPart** meshPart)
{
	if (meshPart == NULL)
	{
		return false;
	}

	if (!IsValid())
	{
		return false;
	}

	RenderSystem* renderer = RenderSystem::Current();
	if (renderer == NULL)
	{
		return false;
	}

	VertexData* vertexData = new VertexData();
	IndexData* indexData = new IndexData();
	VertexLayout* vertexLayout = NULL;
	HardwareBuffer* vertexBuffer = NULL;
	HardwareBuffer* indexBuffer = NULL;
	SEbyte* vbData;
	SEbyte* ibData;

	if (!renderer->CreateVertexLayout(&vertexLayout))
	{
		goto error;
	}

	int textureCoordsCount = _textureCoords.Count();
	int t;
	bool hasNormals = (_normals.Count() != 0);
	bool hasColors = (_colors.Count() != 0);
	bool hasTexCoords = (textureCoordsCount != 0 && _textureCoords[0].Count() != 0);

	uint16 offset = 0;
	vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Position));
	offset += VertexElement::GetTypeSize(VertexFormat_Float3);
	if (hasNormals)
	{
		vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float3, VertexSemantic_Normal));
		offset += VertexElement::GetTypeSize(VertexFormat_Float3);
	}
	if (hasColors)
	{
		vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float4, VertexSemantic_Color));
		offset += VertexElement::GetTypeSize(VertexFormat_Float4);
	}
	if (hasTexCoords)
	{
		for (t = 0; t < textureCoordsCount; ++t)
		{
			vertexLayout->AddElement(VertexElement(0, offset, VertexFormat_Float2,
				VertexSemantic_TextureCoordinate, t));
			offset += VertexElement::GetTypeSize(VertexFormat_Float2);
		}
	}

	bool indexed;
	if (_indices.Count() == 0)
		indexed = false;
	else
		indexed = true;

	int32 vertexCount = _vertices.Count();
	if (!renderer->CreateVertexBuffer(vertexCount * vertexLayout->GetSize(),
		HardwareBufferUsage_Static, &vertexBuffer))
	{
		goto error;
	}

	int32 indexCount = _indices.Count();
	if (indexed)
	{
		if (!renderer->CreateIndexBuffer(indexCount * sizeof(int32), IndexBufferFormat_Int32,
			HardwareBufferUsage_Static, &indexBuffer))
		{
			goto error;
		}
	}

	if (!vertexBuffer->Map(HardwareBufferMode_Normal, (void**)&vbData))
	{
		goto error;
	}

	for (int i = 0; i < vertexCount; ++i)
	{
		Memory::Copy(vbData, _vertices[i], sizeof(Vector3));
		vbData += sizeof(Vector3);

		if (hasNormals)
		{
			Memory::Copy(vbData, _normals[i], sizeof(Vector3));
			vbData += sizeof(Vector3);
		}

		if (hasColors)
		{
			Memory::Copy(vbData, _colors[i], sizeof(Color32));
			vbData += sizeof(Color32);
		}

		if (hasTexCoords)
		{
			for (t = 0; t < textureCoordsCount; ++t)
			{
				const TextureCoordArray& textureCoords = _textureCoords[t];
				Memory::Copy(vbData, (void*)&textureCoords[i], sizeof(Vector2));
				vbData += sizeof(Vector2);
			}
		}
	}

	vertexBuffer->Unmap();

	if (indexed)
	{
		if (!indexBuffer->Map(HardwareBufferMode_Normal, (void**)&ibData))
		{
			goto error;
		}

		Memory::Copy(ibData, &_indices[0], indexCount * sizeof(int32));

		indexBuffer->Unmap();
	}

	*meshPart = new MeshPart();
	vertexData->VertexLayout = vertexLayout;
	vertexData->VertexStreams.Add(VertexStream(vertexBuffer, vertexLayout->GetSize()));
	vertexData->VertexCount = vertexCount;
	(*meshPart)->SetVertexData(vertexData);

	if (indexed)
	{
		indexData->IndexBuffer = indexBuffer;
		indexData->IndexCount = indexCount;
		(*meshPart)->SetIndexData(indexData);
		(*meshPart)->SetIndexed(true);
		(*meshPart)->SetPrimitiveTypeAndCount(_primitiveType, indexCount);
	}
	else
	{
		(*meshPart)->SetPrimitiveTypeAndCount(_primitiveType, vertexCount);
	}

	return true;

error:
	SE_DELETE(vertexLayout);
	SE_DELETE(vertexBuffer);
	SE_DELETE(indexBuffer);
	return false;
}

}
