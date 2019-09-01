/*=============================================================================
Mesh.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_MESH_H_
#define _SE_MESH_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Model/MeshPart.h"

namespace SonataEngine
{

class Bone;
class Model;
class Skin;
class ShaderMaterial;

/** Mesh. */
class SE_GRAPHICS_EXPORT Mesh : public NamedObject
{
	friend class MeshPart;

public:
	typedef Array<MeshPart*> MeshPartList;
	typedef Array<ShaderMaterial*> ShaderList;

protected:
	Model* _parentModel;
	Bone* _parentBone;
	VertexData* _vertexData;
	IndexData* _indexData;
	BoundingBox _boundingBox;
	BoundingSphere _boundingSphere;
	MeshPartList _meshParts;
	Skin* _skin;
	Array<ShaderMaterial*> _shaders;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	Mesh();

	/** Destructor. */
	virtual ~Mesh();
	//@}

	/** Clone this object. */
	Mesh* Clone();

	/** @name Properties. */
	//@{
	/** Gets the parent model. */
	Model* GetParentModel() const { return _parentModel; }
	// Internal
	void SetParentModel(Model* value) { _parentModel = value; }

	/** Gets the parent bone. */
	Bone* GetParentBone() const { return _parentBone; }
	// Internal
	void SetParentBone(Bone* value) { _parentBone = value; }

	VertexData* GetVertexData() { return _vertexData; }
	void SetVertexData(VertexData* value) { _vertexData = value; }

	IndexData* GetIndexData() { return _indexData; }
	void SetIndexData(IndexData* value) { _indexData = value; }

	const BoundingBox& GetBoundingBox() const { return _boundingBox; }
	void SetBoundingBox(const BoundingBox& value) { _boundingBox = value; }

	const BoundingSphere& GetBoundingSphere() const { return _boundingSphere; }
	void SetBoundingSphere(const BoundingSphere& value) { _boundingSphere = value; }

	bool IsSkinned() const;

	/** Gets or sets the associated skin. */
	Skin* GetSkin() const { return _skin; }
	void SetSkin(Skin* value);

	int GetShaderCount() const;
	ShaderMaterial* GetShader(int index) const;
	//@}

	/** Mesh Parts. */
	//@{
	int GetMeshPartCount() const;
	bool AddMeshPart(MeshPart* value);
	bool RemoveMeshPart(MeshPart* value);
	void RemoveAllMeshParts();
	MeshPartList::Iterator GetMeshPartIterator() const;
	MeshPart* GetMeshPart(int index) const;
	//@}

	/** Build the skin data in the vertex buffer. */
	void BuildSkinBuffer();

	virtual void Update(const TimeValue& timeValue);
};

typedef SmartPtr<Mesh> MeshPtr;

}

#endif
