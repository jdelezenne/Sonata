/*=============================================================================
SceneNode.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SceneNode.h"

namespace SonataEngine
{

SE_IMPLEMENT_CLASS(SceneNode);
SE_IMPLEMENT_REFLECTION(SceneNode);

SceneNode::SceneNode() :
	SceneObject()
{
}

SceneNode::~SceneNode()
{
	SceneNodeList::Iterator it = _children.GetIterator();
	while (it.Next())
	{
		SceneObject* child = it.Current();
		child->_SetParent(NULL);
		delete child;
	}
}

bool SceneNode::IsChild(SceneObject* value)
{
	if (value == NULL)
		return false;

	SceneNodeList::Iterator it = _children.GetIterator();
	while (it.Next())
	{
		SceneObject* child = it.Current();
		if (child == value)
			return true;
	}
	return false;
}

int SceneNode::GetChildCount() const
{
	return _children.Count();
}

SceneNode::SceneNodeList::Iterator SceneNode::GetChildIterator() const
{
	return _children.GetIterator();
}

bool SceneNode::AddChild(SceneObject* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return false;
	}
	else if (value->GetParent() != NULL)
	{
		SEthrow(Exception("The object is already attached to another node."));
		return false;
	}

	value->_SetParent(this);
	_children.Add(value);
	return true;
}

bool SceneNode::RemoveChild(SceneObject* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return false;
	}

	value->_SetParent(NULL);
	_children.Remove(value);
	return true;
}

void SceneNode::RemoveAllChildren()
{
	_children.Clear();
}

SceneObject* SceneNode::GetChild(int index) const
{
	return _children[index];
}

bool SceneNode::SetChild(int index, SceneObject* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return false;
	}

	_children.SetItem(index, value);
	return true;
}

SceneObject* SceneNode::GetObjectByName(const String& name) const
{
	if (name.IsEmpty())
		return NULL;

	SceneNodeList::Iterator it = _children.GetIterator();
	while (it.Next())
	{
		SceneObject* child = it.Current();
		if (String::Compare(child->GetName(), name) == 0)
			return child;

		SceneObject* obj = child->GetObjectByName(name);
		if (obj != NULL)
			return obj;
	}
	return NULL;
}

void SceneNode::Update(const TimeValue& timeValue)
{
	super::Update(timeValue);

	SceneNodeList::Iterator it = _children.GetIterator();
	while (it.Next())
	{
		it.Current()->Update(timeValue);
	}
}

void SceneNode::Render()
{
	SceneNodeList::Iterator it = _children.GetIterator();
	while (it.Next())
	{
		it.Current()->Render();
	}
}

void SceneNode::_NotifyLocalTransformUpdate()
{
	super::_NotifyLocalTransformUpdate();

	SceneNodeList::Iterator it = _children.GetIterator();
	while (it.Next())
	{
		it.Current()->_NotifyLocalTransformUpdate();
	}
}

void SceneNode::_NotifyWorldTransformUpdate()
{
	super::_NotifyWorldTransformUpdate();

	SceneNodeList::Iterator it = _children.GetIterator();
	while (it.Next())
	{
		it.Current()->_NotifyWorldTransformUpdate();
	}
}

void SceneNode::_NotifyWorldBoundsUpdate()
{
	super::_NotifyWorldBoundsUpdate();

	SceneNodeList::Iterator it = _children.GetIterator();
	while (it.Next())
	{
		it.Current()->_NotifyWorldBoundsUpdate();
	}
}

void SceneNode::_UpdateWorldBounds()
{
	_UpdateWorldTransform();

	if (!_needWorldBoundsUpdate)
	{
		return;
	}

	// Compute the bounding box
	_worldBoundingBox = BoundingBox::Empty;
	bool isFirst = true;
	SceneNodeList::Iterator it = _children.GetIterator();
	while (it.Next())
	{
		SceneObject* child = it.Current();
		if (isFirst)
		{
			_worldBoundingBox = child->GetWorldBoundingBox();
			isFirst = false;
		}
		else
		{
			_worldBoundingBox.Merge(child->GetWorldBoundingBox());
		}
	}

	// Compute the bounding sphere
	_worldBoundingSphere = BoundingSphere::CreateFromBox(_worldBoundingBox);

	_needWorldBoundsUpdate = false;
}

}
