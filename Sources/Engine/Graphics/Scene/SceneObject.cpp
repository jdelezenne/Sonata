/*=============================================================================
SceneObject.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SceneObject.h"
#include "SceneNode.h"

namespace SonataEngine
{

SE_IMPLEMENT_ABSTRACT(SceneObject);
SE_IMPLEMENT_REFLECTION(SceneObject);

SceneObject::SceneObject() :
	NamedObject(),
	_parent(NULL),
	_isVisible(true),
	_needLocalTransformUpdate(true),
	_needWorldTransformUpdate(true),
	_needWorldBoundsUpdate(true),
	_localPosition(Vector3::Zero),
	_localOrientation(Quaternion::Identity),
	_localScale(Vector3::One),
	_localBoundingBox(BoundingBox::Empty),
	_localTransform(Matrix4::Identity)
{
}

SceneObject::~SceneObject()
{
}

bool SceneObject::IsAncestor(SceneObject* value)
{
	if (value == NULL)
		return true;

	if (value == this)
		return false;

	SceneObject* parent = GetParent();
	while (parent != NULL)
	{
		if (parent == value)
			return true;

		parent = parent->GetParent();
	}
	return false;
}

void SceneObject::SetLocalPosition(const Vector3& value)
{
	_localPosition = value;
	_NotifyLocalTransformUpdate();
}


void SceneObject::SetLocalOrientation(const Quaternion& value)
{
	_localOrientation = value;
	_NotifyLocalTransformUpdate();
}

void SceneObject::SetLocalScale(const Vector3& value)
{
	_localScale = value;
	_NotifyLocalTransformUpdate();
}

void SceneObject::SetLocalScaleUniform(real value)
{
	_localScale = Vector3(value);
	_NotifyLocalTransformUpdate();
}

const Vector3& SceneObject::GetWorldPosition()
{
	_UpdateWorldTransform();
	return _worldPosition;
}

const Quaternion& SceneObject::GetWorldOrientation()
{
	_UpdateWorldTransform();
	return _worldOrientation;
}

const Vector3& SceneObject::GetWorldScale()
{
	_UpdateWorldTransform();
	return _worldScale;
}

const Matrix4& SceneObject::GetLocalTransform()
{
	_UpdateLocalTransform();
	return _localTransform;
}

void SceneObject::SetLocalTransform(const Matrix4& value)
{
	_localTransform = value;
	_localPosition = value.GetTranslation();
	_localOrientation = Quaternion::CreateFromRotationMatrix(Matrix3::CreateFromMatrix4(value));
	_localScale = Vector3::One;
	_NotifyLocalTransformUpdate();
}

const Matrix4& SceneObject::GetWorldTransform()
{
	_UpdateWorldTransform();
	return _worldTransform;
}

const Matrix4& SceneObject::GetWorldInverseTransform()
{
	_UpdateWorldTransform();
	return _worldInverseTransform;
}

Vector3 SceneObject::GetLocalRight() const
{
	return _localOrientation.Rotate(Vector3::UnitX);
}

Vector3 SceneObject::GetLocalUp() const
{
	return _localOrientation.Rotate(Vector3::UnitY);
}

Vector3 SceneObject::GetLocalDirection() const
{
	return _localOrientation.Rotate(-Vector3::UnitZ);
}

Vector3 SceneObject::GetWorldRight()
{
	_UpdateWorldTransform();
	return _worldOrientation.Rotate(Vector3::UnitX);
}

Vector3 SceneObject::GetWorldUp()
{
	_UpdateWorldTransform();
	return _worldOrientation.Rotate(Vector3::UnitY);
}

Vector3 SceneObject::GetWorldDirection()
{
	_UpdateWorldTransform();
	return _worldOrientation.Rotate(-Vector3::UnitZ);
}

const BoundingSphere& SceneObject::GetLocalBoundingSphere()
{
	return _localBoundingSphere;
}

void SceneObject::SetLocalBoundingSphere(const BoundingSphere& value)
{
	_localBoundingSphere = value;
	_NotifyWorldBoundsUpdate();
}

const BoundingBox& SceneObject::GetLocalBoundingBox()
{
	return _localBoundingBox;
}

void SceneObject::SetLocalBoundingBox(const BoundingBox& value)
{
	_localBoundingBox = value;
	_NotifyWorldBoundsUpdate();
}

const BoundingSphere& SceneObject::GetWorldBoundingSphere()
{
	_UpdateWorldBounds();
	return _worldBoundingSphere;
}

const BoundingBox& SceneObject::GetWorldBoundingBox()
{
	_UpdateWorldBounds();
	return _worldBoundingBox;
}

void SceneObject::Translate(const Vector3& value)
{
	_localPosition += value;
	_NotifyLocalTransformUpdate();
}

void SceneObject::TranslateLocal(const Vector3& value)
{
	_localPosition += _localOrientation.Rotate(value);
	_NotifyLocalTransformUpdate();
}

void SceneObject::Rotate(const Quaternion& value)
{
	_localOrientation = value * _localOrientation;
	_NotifyLocalTransformUpdate();
}

void SceneObject::Rotate(const Vector3& axis, real angle)
{
	Quaternion rotation = Quaternion::CreateFromAxisAngle(axis, angle);
	_localOrientation = rotation * _localOrientation;
	_NotifyLocalTransformUpdate();
}

void SceneObject::RotateLocal(const Vector3& axis, real angle)
{
	Quaternion rotation = Quaternion::CreateFromAxisAngle(_localOrientation.Rotate(axis), angle);
	_localOrientation = rotation * _localOrientation;
	_NotifyLocalTransformUpdate();
}

void SceneObject::Scale(const Vector3& value)
{
	_localScale *= value;
	_NotifyLocalTransformUpdate();
}

void SceneObject::ScaleUniform(real value)
{
	_localScale *= value;
	_NotifyLocalTransformUpdate();
}

SceneObject* SceneObject::GetObjectByName(const String& name) const
{
	return NULL;
}

void SceneObject::Update(const TimeValue& timeValue)
{
}

void SceneObject::Render()
{
}

void SceneObject::_SetParent(SceneNode* value)
{
	_parent = value;
}

void SceneObject::_NotifyLocalTransformUpdate()
{
	_needLocalTransformUpdate = true;
}

void SceneObject::_NotifyWorldTransformUpdate()
{
	_needWorldTransformUpdate = true;
}

void SceneObject::_NotifyWorldBoundsUpdate()
{
	_needWorldBoundsUpdate = true;
}

void SceneObject::_UpdateLocalTransform()
{
	if (!_needLocalTransformUpdate)
	{
		return;
	}

	_localTransform = Matrix4::CreateScale(_localScale);
	_localTransform *= Matrix4::CreateFromQuaternion(_localOrientation);
	_localTransform.SetTranslation(_localPosition);

	_needWorldTransformUpdate = true;
	_needLocalTransformUpdate = false;
}

void SceneObject::_UpdateWorldTransform()
{
	_UpdateLocalTransform();

	if (!_needWorldTransformUpdate)
	{
		return;
	}

	if (_parent != NULL)
	{
		_worldPosition = _parent->GetWorldOrientation().Rotate(_localPosition * _parent->GetWorldScale());
		_worldPosition += _parent->GetWorldPosition();
		_worldOrientation = _parent->GetWorldOrientation() * _localOrientation;
		_worldScale = Vector3::Scaling(_localScale, _parent->GetWorldScale());
		_worldTransform = _parent->GetWorldTransform() * _localTransform;
	}
	else
	{
		_worldPosition = _localPosition;
		_worldOrientation = _localOrientation;
		_worldScale = _localScale;
		_worldTransform = _localTransform;
	}

	_worldInverseTransform = Matrix4::Invert(_worldTransform);

	_needWorldBoundsUpdate = true;
	_needWorldTransformUpdate = false;
}

void SceneObject::_UpdateWorldBounds()
{
	_UpdateWorldTransform();

	if (!_needWorldBoundsUpdate)
	{
		return;
	}

	_worldBoundingBox = BoundingBox::Transform(_localBoundingBox, _worldTransform);
	_worldBoundingSphere = BoundingSphere::Transform(_localBoundingSphere, _worldTransform);

	_needWorldBoundsUpdate = false;
}

}
