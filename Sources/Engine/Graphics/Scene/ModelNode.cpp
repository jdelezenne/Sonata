/*=============================================================================
ModelNode.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "ModelNode.h"
#include "Graphics/System/RenderSystem.h"
#include "Graphics/SceneManager.h"

namespace SonataEngine
{

SE_IMPLEMENT_CLASS(ModelNode);
SE_IMPLEMENT_REFLECTION(ModelNode);

ModelNode::ModelNode() :
	SceneObject(),
	_model(NULL),
	_isShadowCaster(true),
	_isShadowReceiver(true)
{
}

ModelNode::~ModelNode()
{
	_model = NULL;
}

void ModelNode::SetModel(Model* value)
{
	_model = value;
	UpdateBounds();
}

Matrix4 ModelNode::GetModelTransform()
{
	if (_model == NULL)
		return GetWorldTransform();
	else
		return GetWorldTransform() * _model->GetTransform();
}

void ModelNode::UpdateBounds()
{
	if (_model == NULL)
	{
		return;
	}

	_model->UpdateBounds();

	SetLocalBoundingSphere(_model->GetBoundingSphere());
	SetLocalBoundingBox(_model->GetBoundingBox());
}

void ModelNode::Update(const TimeValue& timeValue)
{
	super::Update(timeValue);

	if (_model == NULL)
	{
		return;
	}

	_model->Update(timeValue);
}

void ModelNode::Render()
{
	if (_model == NULL)
	{
		return;
	}

	Model::MeshList::Iterator it = _model->GetMeshIterator();
	while (it.Next())
	{
		Mesh* mesh = it.Current();
		Matrix4 transform = GetModelTransform();
		if (mesh->GetParentBone() != NULL)
		{
			transform = transform * mesh->GetParentBone()->GetGlobalTransform();
		}
		SceneManager::Instance()->RenderMesh(mesh, transform);
	}
}

}
