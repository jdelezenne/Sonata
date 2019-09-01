/*=============================================================================
SceneObject.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SCENEOBJECT_H_
#define _SE_SCENEOBJECT_H_

#include "Core/Core.h"
#include "Graphics/Common.h"

namespace SonataEngine
{

class SceneNode;

/** SceneObject object. */
class SE_GRAPHICS_EXPORT SceneObject : public NamedObject
{
	SE_DECLARE_CLASS(SceneObject, NamedObject);
	SE_BEGIN_REFLECTION(SceneObject);
		SE_Field(_localPosition, Vector3, Public);
		SE_Field(_localOrientation, Quaternion, Public);
		SE_Field(_localScale, Vector3, Public);
	SE_END_REFLECTION(SceneObject);

	friend SceneNode;

protected:
	SceneNode* _parent;
	bool _isVisible;

	bool _needLocalTransformUpdate;
	bool _needWorldTransformUpdate;
	bool _needWorldBoundsUpdate;

	Vector3 _localPosition;
	Quaternion _localOrientation;
	Vector3 _localScale;
	Vector3 _worldPosition;
	Quaternion _worldOrientation;
	Vector3 _worldScale;

	BoundingBox _localBoundingBox;
	BoundingSphere _localBoundingSphere;
	BoundingBox _worldBoundingBox;
	BoundingSphere _worldBoundingSphere;

	Matrix4 _localTransform;
	Matrix4 _worldTransform;
	Matrix4 _worldInverseTransform;

public:
	virtual ~SceneObject();

	/** @name Properties. */
	//@{
	/** Gets a value indicating whether this object is a node. */
	virtual bool IsNode() const { return false; }

	/**
		Gets the parent node of the object.
		@return Parent node of the object.
	*/
	virtual SceneNode* GetParent() const { return _parent; }

	/** Gets a value indicating whether the specified object is an ancestor to the current object. */
	bool IsAncestor(SceneObject* value);

	/**
		Gets a value indicating whether the object is visible.
		@return true if the object is visible; otherwise, false.
	*/
	bool IsVisible() const { return _isVisible; }

	/**
		Sets a value indicating whether the object is visible.
		@param value true to set the object as visible; otherwise, false.
	*/
	void SetVisible(bool value) { _isVisible = value; }

	/**
		Gets the local position.
		@return Local position.
	*/
	const Vector3& GetLocalPosition() const { return _localPosition; }

	/**
		Sets the local position.
		@param value Local position.
	*/
	void SetLocalPosition(const Vector3& value);

	/**
		Gets the local orientation.
		@return Local orientation.
	*/
	const Quaternion& GetLocalOrientation() const { return _localOrientation; }

	/**
		Sets the local orientation.
		@param value Local orientation.
	*/
	void SetLocalOrientation(const Quaternion& value);

	/**
		Gets the local scale.
		@return Local scale.
	*/
	const Vector3& GetLocalScale() const { return _localScale; }

	/**
		Sets the local scale.
		@param value Local scale.
	*/
	void SetLocalScale(const Vector3& value);
	void SetLocalScaleUniform(real value);

	/**
		Gets the world position.
		@return World position.
	*/
	const Vector3& GetWorldPosition();

	/**
		Gets the world orientation.
		@return World orientation.
	*/
	const Quaternion& GetWorldOrientation();

	/**
		Gets the world scale.
		@return World scale.
	*/
	const Vector3& GetWorldScale();

	/**
		Gets the local transformation matrix.
		@return Local transformation matrix.
	*/
	const Matrix4& GetLocalTransform();

	/**
		Sets the local transformation matrix.
		@param value Local transformation matrix.
		@remarks The scale will be set to one.
	*/
	void SetLocalTransform(const Matrix4& value);

	/**
		Gets the world transformation matrix.
		@return World transformation matrix.
	*/
	const Matrix4& GetWorldTransform();

	/**
		Gets the world inverse transformation matrix.
		@return World inverse transformation matrix.
	*/
	const Matrix4& GetWorldInverseTransform();

	Vector3 GetLocalRight() const;
	Vector3 GetLocalUp() const;
	Vector3 GetLocalDirection() const;

	Vector3 GetWorldRight();
	Vector3 GetWorldUp();
	Vector3 GetWorldDirection();

	/**
		Gets the local bounding box.
		@return Local bounding box.
	*/
	const BoundingBox& GetLocalBoundingBox();

	/**
		Sets the local bounding box.
		@param value Local bounding box.
	*/
	void SetLocalBoundingBox(const BoundingBox& value);

	/**
		Gets the local bounding sphere.
		@return Local bounding sphere.
	*/
	const BoundingSphere& GetLocalBoundingSphere();

	/**
		Sets the local bounding sphere.
		@param value Local bounding sphere.
	*/
	void SetLocalBoundingSphere(const BoundingSphere& value);

	/**
		Gets the world bounding box.
		@return World bounding box.
	*/
	const BoundingBox& GetWorldBoundingBox();

	/**
		Gets the world bounding sphere.
		@return World bounding sphere.
	*/
	const BoundingSphere& GetWorldBoundingSphere();
	//@}

	/** @name Operations. */
	//@{
	/**
		Translates the object.
		@param vector Translation vector.
	*/
	void Translate(const Vector3& vector);
	void TranslateLocal(const Vector3& vector);

	/**
		Rotates the object.
		@param quaternion Rotation quaternion.
	*/
	void Rotate(const Quaternion& quaternion);
	void Rotate(const Vector3& axis, real angle);
	void RotateLocal(const Vector3& axis, real angle);

	/**
		Scale the object.
		@param vector Scale vector.
	*/
	void Scale(const Vector3& vector);

	/**
		Scale uniformaly the object.
		@param s Scalar.
	*/
	void ScaleUniform(real vector);

	/** Finds an object with the specified name in the hierarchy. */
	virtual SceneObject* GetObjectByName(const String& name) const;

	/**
		Updates the object.
		@param timeValue The current time.
	*/
	virtual void Update(const TimeValue& timeValue);

	/**
		Renders the object.
	*/
	virtual void Render();
	//@}

protected:
	SceneObject();

	virtual void _SetParent(SceneNode* value);

	virtual void _NotifyLocalTransformUpdate();

	virtual void _NotifyWorldTransformUpdate();

	virtual void _NotifyWorldBoundsUpdate();

	/**
		Updates the local transformation matrix.
	*/
	virtual void _UpdateLocalTransform();

	/**
		Updates the world transformation matrix.
	*/
	virtual void _UpdateWorldTransform();

	/**
		Updates the world bounds.
	*/
	virtual void _UpdateWorldBounds();
};

SEPointer(SceneObject);

}

#endif
