/*=============================================================================
Model.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Model.h"

namespace SonataEngine
{

Model::Model() :
	NamedObject(),
	_skeleton(NULL),
	_transform(Matrix4::Identity),
	_animationSet(NULL),
	_isTransparent(false)
{
}

Model::~Model()
{
	MeshList::Iterator it = _meshes.GetIterator();
	while (it.Next())
	{
		delete it.Current();
	}
}

Model* Model::Clone()
{
	Model* model = new Model();

	model->_name = _name;
	model->_skeleton = _skeleton;

	MeshList::Iterator it = _meshes.GetIterator();
	while (it.Next())
	{
		Mesh* value = it.Current()->Clone();
		model->AddMesh(value);
	}

	model->UpdateBounds();

	return model;
}

void Model::SetTransform(const Matrix4& value)
{
	_transform = value;
}

int Model::GetMeshCount() const
{
	return _meshes.Count();
}

bool Model::AddMesh(Mesh* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return false;
	}

	if (value->GetParentModel() != NULL)
	{
		return false;
	}

	_meshes.Add(value);
	value->SetParentModel(this);
	return true;
}

bool Model::RemoveMesh(Mesh* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return false;
	}

	_meshes.Remove(value);
	return true;
}

void Model::RemoveAllMeshs()
{
	_meshes.Clear();
}

Model::MeshList::Iterator Model::GetMeshIterator() const
{
	return _meshes.GetIterator();
}

Mesh* Model::GetMeshByIndex(int index) const
{
	return _meshes[index];
}

Mesh* Model::GetMeshByName(const String& name) const
{
	MeshList::Iterator it = _meshes.GetIterator();
	while (it.Next())
	{
		if (it.Current()->GetName() == name)
		{
			return it.Current();
		}
	}

	return NULL;
}

void Model::UpdateBounds()
{
	// Compute the bounding box
	_boundingBox = BoundingBox::Empty;
	bool isFirst = true;
	MeshList::Iterator it = _meshes.GetIterator();
	if (it.Next())
	{
		Mesh* mesh = it.Current();
		BoundingBox boundingBox = mesh->GetBoundingBox();

		if (mesh->GetParentBone() != NULL)
		{
			boundingBox.Transform(mesh->GetParentBone()->GetGlobalTransform());
		}

		if (isFirst)
		{
			_boundingBox = boundingBox;
			isFirst= false;
		}
		else
		{
			_boundingBox.Merge(boundingBox);
		}
	}

	_boundingBox.Transform(_transform);

	// Compute the bounding sphere
	_boundingSphere = BoundingSphere::CreateFromBox(_boundingBox);
}

void Model::Update(const TimeValue& timeValue)
{
	if (_skeleton != NULL)
	{
		_skeleton->Update(timeValue);
	}

	if (_animationSet != NULL)
	{
		_animationSet->Update(timeValue);
	}

	MeshList::Iterator it = _meshes.GetIterator();
	if (it.Next())
	{
		it.Current()->Update(timeValue);
	}
}

}
