/*=============================================================================
Socket.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SOCKET_H_
#define _SE_SOCKET_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Scene/SceneObject.h"

namespace SonataEngine
{

class Bone;
class Skeleton;

/** Socket. */
class SE_GRAPHICS_EXPORT Socket : public NamedObject
{
public:
	typedef Array<SceneObject*> AttachmentList;

protected:
	Skeleton* _skeleton;
	Bone* _bone;
	Vector3 _position;
	Quaternion _orientation;
	Vector3 _scale;
	Matrix4 _transform;

public:
	Socket();
	virtual ~Socket();

	/**
		Gets the parent skeleton.
	*/
	Skeleton* GetParentSkeleton() const { return _skeleton; }
	// Internal
	void SetParentSkeleton(Skeleton* value) { _skeleton = value; }

	Bone* GetBone() const { return _bone; }
	void SetBone(Bone* value) { _bone = value; }

	const Vector3& GetPosition() const { return _position; }
	void SetPosition(const Vector3& value) { _position = value; }

	const Quaternion& GetOrientation() const { return _orientation; }
	void SetOrientation(const Quaternion& value) { _orientation = value; }

	const Vector3& GetScale() const { return _scale; }
	void SetScale(const Vector3& value) { _scale = value; }

	const Matrix4& GetTransform();

	/** Attachments to this socket. */
	AttachmentList Attachments;

	void Update();
};

}

#endif
