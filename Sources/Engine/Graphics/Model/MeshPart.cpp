/*=============================================================================
MeshPart.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "MeshPart.h"
#include "Graphics/Model/Mesh.h"

namespace SonataEngine
{

MeshPart::MeshPart() :
	NamedObject(),
	_parentMesh(NULL),
	_primitiveType(PrimitiveType_PointList),
	_primitiveCount(0),
	_vertexData(NULL),
	_indexData(NULL),
	_isIndexed(false),
	_startVertex(0),
	_startIndex(0),
	_shader(NULL)
{
}

MeshPart::~MeshPart()
{
	SE_DELETE(_vertexData);
	SE_DELETE(_indexData);
}

MeshPart* MeshPart::Clone()
{
	MeshPart* meshPart = new MeshPart();

	meshPart->_primitiveType = _primitiveType;
	meshPart->_primitiveCount = _primitiveCount;
	meshPart->_vertexData = _vertexData;
	meshPart->_indexData = _indexData;
	meshPart->_isIndexed = _isIndexed;
	meshPart->_startIndex = _startIndex;
	meshPart->_shader = _shader;

	return meshPart;
}

void MeshPart::SetPrimitiveTypeAndCount(PrimitiveType value, uint32 count)
{
	_primitiveType = value;
	_primitiveCount = RenderData::GetPrimitiveCount(_primitiveType, count);
}

void MeshPart::SetShader(ShaderMaterial* value)
{
	SE_ASSERT(_parentMesh != NULL);
	if (_shader != value)
	{
		if (_shader != NULL)
		{
			bool needRemove = true;
			Mesh::MeshPartList::Iterator it = _parentMesh->_meshParts.GetIterator();
			while (it.Next())
			{
				MeshPart* meshPart = it.Current();
				if (_shader == meshPart->GetShader())
				{
					needRemove = false;
					break;
				}
			}

			if (needRemove)
			{
				_parentMesh->_shaders.Remove(_shader);
			}
		}
		if (value != NULL)
		{
			if (!_parentMesh->_shaders.Contains(value))
			{
				_parentMesh->_shaders.Add(value);
			}
		}
		_shader = value;
	}
}

void MeshPart::GetRenderData(RenderData& renderData, uint16 lodIndex) const
{
	renderData.Type = _primitiveType;
	renderData.PrimitiveCount = _primitiveCount;
	renderData.VertexData = _vertexData;
	renderData.IndexData = _indexData;
	renderData.IsIndexed = _isIndexed;
	renderData.StartVertex = _startVertex;
	renderData.StartIndex = _startIndex;
}

}
