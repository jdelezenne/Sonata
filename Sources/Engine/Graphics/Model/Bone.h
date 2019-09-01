/*=============================================================================
Bone.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_BONE_H_
#define _SE_BONE_H_

#include "Core/Core.h"
#include "Graphics/Common.h"

namespace SonataEngine
{

class Skeleton;

/** Bone. */
class SE_GRAPHICS_EXPORT Bone : public NamedObject
{
public:
	typedef Array<Bone*> BoneList;

protected:
	Skeleton* _skeleton;
	uint16 _index;
	Bone* _parent;
	BoneList _children;

	bool _needUpdatePose;
	bool _needUpdateReference;
	bool _needUpdateTransform;

	Matrix4 _poseTransform;
	Matrix4 _invPoseTransform;

	Vector3 _referencePosition;
	Quaternion _referenceOrientation;
	Vector3 _referenceScale;
	Matrix4 _referenceTransform;

	Vector3 _localPosition;
	Quaternion _localOrientation;
	Vector3 _localScale;
	Matrix4 _localTransform;

	Vector3 _globalPosition;
	Quaternion _globalOrientation;
	Vector3 _globalScale;
	Matrix4 _globalTransform;

	Matrix4 _skinTransform;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	Bone(Skeleton* skeleton, uint16 index);

	/** Destructor. */
	virtual ~Bone();
	//@}

	/** @name Properties. */
	//@{
	/**
		Gets the parent skeleton.
	*/
	Skeleton* GetSkeleton() const { return _skeleton; }

	/**
		Gets the index of the bone in the skeleton.
	*/
	uint16 GetIndex() const { return _index; }

	/**
		Gets the parent of the bone.
		@return The parent of the bone or NULL if the bone is a root.
	*/
	Bone* GetParent() const { return _parent; }
	// Internal
	void SetParent(Bone* value) { _parent = value; }

	/**
		Sets the local pose transform.
		@remarks
			The skeleton hierarchy needs to be setup.
	*/
	void SetLocalPoseTransform(const Matrix4& value);
	void SetLocalPoseTransform(const Vector3& position, const Quaternion& orientation, const Vector3& scale);

	/** Gets or sets the global pose transform. */
	const Matrix4& GetPoseTransform() const { return _poseTransform; }
	void SetPoseTransform(const Matrix4& value);
	void SetPoseTransform(const Vector3& position, const Quaternion& orientation, const Vector3& scale);

	/** Gets or sets the global inverse pose transform. */
	const Matrix4& GetInvPoseTransform() const { return _invPoseTransform; }
	void SetInvPoseTransform(const Matrix4& value);

	/** Gets or sets the reference position. */
	const Vector3& GetReferencePosition() const { return _referencePosition; }
	void SetReferencePosition(const Vector3& value);

	/** Gets or sets the reference orientation. */
	const Quaternion& GetReferenceOrientation() const { return _referenceOrientation; }
	void SetReferenceOrientation(const Quaternion& value);

	/** Gets or sets the reference scale. */
	const Vector3& GetReferenceScale() const { return _referenceScale; }
	void SetReferenceScale(const Vector3& value);

	/** Gets the reference transform. */
	const Matrix4& GetReferenceTransform();

	/** Gets or sets the local position. */
	const Vector3& GetLocalPosition() const { return _localPosition; }
	void SetLocalPosition(const Vector3& value);

	/** Gets or sets the local orientation. */
	const Quaternion& GetLocalOrientation() const { return _localOrientation; }
	void SetLocalOrientation(const Quaternion& value);

	/** Gets or sets the local scale. */
	const Vector3& GetLocalScale() const { return _localScale; }
	void SetLocalScale(const Vector3& value);

	/** Gets the local transform. */
	const Matrix4& GetLocalTransform();

	/** Gets the global position. */
	const Vector3& GetGlobalPosition();

	/** Gets the global orientation. */
	const Quaternion& GetGlobalOrientation();

	/** Gets the global scale. */
	const Vector3& GetGlobalScale();

	/** Gets the global transform. */
	const Matrix4& GetGlobalTransform();

	/**
		Gets the skinning transform.
		@remarks
			Mskin = Mglobal * MinvPose
	*/
	const Matrix4& GetSkinTransform();
	//@}

	/** Children. */
	//@{
	int GetChildCount() const;
	BoneList::Iterator GetChildIterator() const;
	bool AddChild(Bone* value);
	int GetChildIndex(Bone* value) const;
	Bone* GetChildByIndex(int index) const;
	Bone* GetChildByName(const String& name) const;
	//@}

	/** Resets the bone transform to its reference transform. */
	void SetTransformToReference();

	/** Update. */
	void Update();

private:
	void _UpdateReference();
	void _UpdateTransform();
	void _UpdateSkin();
};

SEPointer(Bone);

}

#endif
