/*=============================================================================
Mesh.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Mesh.h"
#include "Graphics/Animation/AnimationSet.h"
#include "Graphics/Model/Model.h"
#include "Graphics/Model/Skeleton.h"
#include "Graphics/Model/Skin.h"

namespace SonataEngine
{

Mesh::Mesh() :
	NamedObject(),
	_parentModel(NULL),
	_parentBone(NULL),
	_vertexData(NULL),
	_indexData(NULL),
	_skin(NULL)
{
}

Mesh::~Mesh()
{
}

Mesh* Mesh::Clone()
{
	Mesh* mesh = new Mesh();

	mesh->_name = _name;
	mesh->_parentBone = _parentBone;
	mesh->_meshParts = _meshParts;
	mesh->_boundingBox = _boundingBox;
	mesh->_boundingSphere = _boundingSphere;
	mesh->_shaders = _shaders;

	return mesh;
}

void Mesh::SetSkin(Skin* value)
{
	_skin = value;
	if (_skin != NULL)
	{
		_skin->Initialize(_vertexData);
	}
}

bool Mesh::IsSkinned() const
{
	return (_skin != NULL);
}

int Mesh::GetShaderCount() const
{
	return _shaders.Count();
}

ShaderMaterial* Mesh::GetShader(int index) const
{
	return _shaders[index];
}

int Mesh::GetMeshPartCount() const
{
	return _meshParts.Count();
}

bool Mesh::AddMeshPart(MeshPart* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return false;
	}

	if (value->GetParentMesh() != NULL)
	{
		return false;
	}

	_meshParts.Add(value);
	value->SetParentMesh(this);
	return true;
}

bool Mesh::RemoveMeshPart(MeshPart* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return false;
	}

	_meshParts.Remove(value);
	return true;
}

void Mesh::RemoveAllMeshParts()
{
	_meshParts.Clear();
}

Mesh::MeshPartList::Iterator Mesh::GetMeshPartIterator() const
{
	return _meshParts.GetIterator();
}

MeshPart* Mesh::GetMeshPart(int index) const
{
	return _meshParts[index];
}

void Mesh::BuildSkinBuffer()
{
	for (int i = 0; i < _meshParts.Count(); ++i)
	{
		if (_meshParts[i]->GetVertexData() == NULL)
			continue;

		VertexLayout* vertexLayout = _meshParts[i]->GetVertexData()->VertexLayout;
		if (vertexLayout == NULL)
			continue;

		const VertexElement* blendIndices = vertexLayout->GetElementBySemantic(VertexSemantic_BlendIndices);
		const VertexElement* blendWeight = vertexLayout->GetElementBySemantic(VertexSemantic_BlendWeight);
		if (blendIndices != NULL && blendWeight != NULL)
			continue;

		if (blendIndices == NULL && blendWeight == NULL)
		{
			vertexLayout->AddElement(VertexElement(0, vertexLayout->GetSize(),
				VertexFormat_UByte4, VertexSemantic_BlendIndices));
			vertexLayout->AddElement(VertexElement(0, vertexLayout->GetSize() + sizeof(uint8) * 4,
				VertexFormat_Float4, VertexSemantic_BlendWeight));
		}
	}
}

void Mesh::Update(const TimeValue& timeValue)
{
	if (_parentModel != NULL)
	{
		Skeleton* skeleton = _parentModel->GetSkeleton();
		if (_skin != NULL && skeleton != NULL)
		{
			_skin->Update(skeleton);
			_skin->Deform();
		}
	}
}

}
