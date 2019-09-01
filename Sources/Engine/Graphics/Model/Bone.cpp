/*=============================================================================
Bone.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Bone.h"

namespace SonataEngine
{

Bone::Bone(Skeleton* skeleton, uint16 index) :
	NamedObject(),
	_skeleton(skeleton),
	_index(index),
	_parent(NULL),
	_needUpdatePose(true),
	_needUpdateReference(true),
	_needUpdateTransform(true),
	_poseTransform(Matrix4::Identity),
	_invPoseTransform(Matrix4::Identity),
	_referenceTransform(Matrix4::Identity)
{
	_localPosition = Vector3::Zero;
	_localOrientation = Quaternion::Identity;
	_localScale = Vector3::One;

	_referencePosition = Vector3::Zero;
	_referenceOrientation = Quaternion::Identity;
	_referenceScale = Vector3::One;

	Update();
}

Bone::~Bone()
{
}

int Bone::GetChildCount() const
{
	return _children.Count();
}

Bone::BoneList::Iterator Bone::GetChildIterator() const
{
	return _children.GetIterator();
}

bool Bone::AddChild(Bone* value)
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return false;
	}
	else if (value->GetParent() != NULL)
	{
		return false;
	}
	else
	{
		_children.Add(value);
		value->SetParent(this);
		return true;
	}
}

int Bone::GetChildIndex(Bone* value) const
{
	if (value == NULL)
	{
		SEthrow(ArgumentNullException("value"));
		return false;
	}
	else
	{
		return _children.IndexOf(value);
	}
}

Bone* Bone::GetChildByIndex(int index) const
{
	return _children[index];
}

Bone* Bone::GetChildByName(const String& name) const
{
	BoneList::Iterator it = _children.GetIterator();
	while (it.Next())
	{
		if (it.Current()->GetName() == name)
			return it.Current();
	}

	return NULL;
}

void Bone::SetLocalPoseTransform(const Matrix4& value)
{
	if (_parent != NULL)
	{
		SetPoseTransform(_parent->GetPoseTransform() * value);
	}
	else
	{
		SetPoseTransform(value);
	}
}

void Bone::SetLocalPoseTransform(const Vector3& position, const Quaternion& orientation, const Vector3& scale)
{
	Matrix4 transform = Matrix4::CreateScale(scale);
	transform *= Matrix4::CreateFromQuaternion(orientation);
	transform.SetTranslation(position);

	SetLocalPoseTransform(transform);
}

void Bone::SetPoseTransform(const Matrix4& value)
{
	_poseTransform = value;
	_invPoseTransform = Matrix4::Invert(_poseTransform);

	_needUpdatePose = true;
}

void Bone::SetPoseTransform(const Vector3& position, const Quaternion& orientation, const Vector3& scale)
{
	_poseTransform = Matrix4::CreateScale(scale);
	_poseTransform *= Matrix4::CreateFromQuaternion(orientation);
	_poseTransform.SetTranslation(position);

	_invPoseTransform = Matrix4::Invert(_poseTransform);

	_needUpdatePose = true;
}

void Bone::SetInvPoseTransform(const Matrix4& value)
{
	_invPoseTransform = value;
	_poseTransform = Matrix4::Invert(_invPoseTransform);

	_needUpdatePose = true;
}

void Bone::SetReferencePosition(const Vector3& value)
{
	_referencePosition = value;
	_needUpdateReference = true;
}

void Bone::SetReferenceOrientation(const Quaternion& value)
{
	_referenceOrientation = value;
	_needUpdateReference = true;
}

void Bone::SetReferenceScale(const Vector3& value)
{
	_referenceScale = value;
	_needUpdateReference = true;
}

const Matrix4& Bone::GetReferenceTransform()
{
	_UpdateReference();
	return _referenceTransform;
}

void Bone::SetLocalPosition(const Vector3& value)
{
	_localPosition = value;
	_needUpdateTransform = true;
}

void Bone::SetLocalOrientation(const Quaternion& value)
{
	_localOrientation = value;
	_needUpdateTransform = true;
}

void Bone::SetLocalScale(const Vector3& value)
{
	_localScale = value;
	_needUpdateTransform = true;
}

const Matrix4& Bone::GetLocalTransform()
{
	_UpdateTransform();
	return _localTransform;
}

const Vector3& Bone::GetGlobalPosition()
{
	_UpdateTransform();
	return _globalPosition;
}

const Quaternion& Bone::GetGlobalOrientation()
{
	_UpdateTransform();
	return _globalOrientation;
}

const Vector3& Bone::GetGlobalScale()
{
	_UpdateTransform();
	return _globalScale;
}

const Matrix4& Bone::GetGlobalTransform()
{
	_UpdateTransform();
	return _globalTransform;
}

const Matrix4& Bone::GetSkinTransform()
{
	_UpdateSkin();
	return _skinTransform;
}

void Bone::SetTransformToReference()
{
	_localPosition = _referencePosition;
	_localOrientation = _referenceOrientation;
	_localScale = _referenceScale;

	_localTransform = Matrix4::Identity;
	_globalTransform = Matrix4::Identity;
	_skinTransform = Matrix4::Identity;

	_needUpdateTransform = true;
}

void Bone::Update()
{
	_UpdateSkin();

	for (int i = 0; i < _children.Count(); ++i)
	{
		_children[i]->Update();
	}
}

void Bone::_UpdateReference()
{
	if (!_needUpdateReference)
	{
		return;
	}

	// Updates the reference transform
	_referenceTransform = Matrix4::CreateScale(_referenceScale);
	_referenceTransform *= Matrix4::CreateFromQuaternion(_referenceOrientation);
	_referenceTransform.SetTranslation(_referencePosition);

	_needUpdateReference = false;
}

void Bone::_UpdateTransform()
{
	if (!_needUpdateTransform)
	{
		return;
	}

	// Updates the local transform
	_localTransform = Matrix4::CreateScale(_localScale);
	_localTransform *= Matrix4::CreateFromQuaternion(_localOrientation);
	_localTransform.SetTranslation(_localPosition);

	// Updates the global transform
	if (_parent != NULL)
	{
		_globalPosition = _parent->GetGlobalOrientation().Rotate(_localPosition * _parent->GetGlobalScale());
		_globalPosition += _parent->GetGlobalPosition();
		_globalOrientation = _parent->GetGlobalOrientation() * _localOrientation;
		_globalScale = Vector3::Scaling(_localScale, _parent->GetGlobalScale());
		_globalTransform = _parent->GetGlobalTransform() * _localTransform;
	}
	else
	{
		_globalPosition = _localPosition;
		_globalOrientation = _localOrientation;
		_globalScale = _localScale;
		_globalTransform = _localTransform;
	}

	_needUpdateTransform = false;
}

void Bone::_UpdateSkin()
{
	bool needUpdateSkin = (_needUpdatePose || _needUpdateTransform);
	_UpdateReference();
	_UpdateTransform();

	if (needUpdateSkin)
	{
		// Updates the skinning transform
		_skinTransform = _globalTransform * _invPoseTransform;
	}
}

}
