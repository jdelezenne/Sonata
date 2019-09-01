/*=============================================================================
Model.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_MODEL_H_
#define _SE_MODEL_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Model/Bone.h"
#include "Graphics/Model/Mesh.h"
#include "Graphics/Model/Skeleton.h"

namespace SonataEngine
{

class AnimationSet;

/** Model. */
class SE_GRAPHICS_EXPORT Model : public NamedObject
{
public:
	typedef Array<BonePtr> BoneList;
	typedef Array<MeshPtr> MeshList;

protected:
	String _name;
	Skeleton* _skeleton;
	MeshList _meshes;
	BoundingBox _boundingBox;
	BoundingSphere _boundingSphere;
	AnimationSet* _animationSet;
	Matrix4 _transform;
	bool _isTransparent;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	Model();

	/** Destructor. */
	virtual ~Model();
	//@}

	/** Clone this object. */
	Model* Clone();

	/** @name Properties. */
	//@{
	/** Get or sets the skeleton. */
	Skeleton* GetSkeleton() const { return _skeleton; }
	void SetSkeleton(Skeleton* value)  { _skeleton = value; }

	/** Get the bounding box. */
	const BoundingBox& GetBoundingBox() const { return _boundingBox; }

	/** Get the bounding sphere. */
	const BoundingSphere& GetBoundingSphere() const { return _boundingSphere; }

	/** Gets the animation set. */
	AnimationSet* GetAnimationSet() const { return _animationSet; }

	/** Sets the animation set. */
	void SetAnimationSet(AnimationSet* value) { _animationSet = value; }

	/** Get or sets the skeleton. */
	const Matrix4& GetTransform() const { return _transform; }
	void SetTransform(const Matrix4& value);

	/** Gets or sets a value indicating whether the Model is transparent. */
	bool IsTransparent() const { return _isTransparent; }
	void SetTransparent(bool value) { _isTransparent = value; }
	//@}

	/** Meshes. */
	//@{
	int GetMeshCount() const;
	bool AddMesh(Mesh* value);
	bool RemoveMesh(Mesh* value);
	void RemoveAllMeshs();
	MeshList::Iterator GetMeshIterator() const;
	Mesh* GetMeshByIndex(int index) const;
	Mesh* GetMeshByName(const String& name) const;
	//@}

	/** @name Operations. */
	//@{
	/** Updates the Model. */
	virtual void Update(const TimeValue& timeValue);

	/** Updates the bounds of the Model. */
	void UpdateBounds();
	//@}
};

SEPointer(Model);

}

#endif
