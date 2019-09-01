/*=============================================================================
MeshPart.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_MESHPART_H_
#define _SE_MESHPART_H_

#include "Graphics/Common.h"
#include "Graphics/System/RenderData.h"

namespace SonataEngine
{

class Mesh;
class ShaderMaterial;

/**
	@brief Mesh Part.
	
	This class describes a subset of a mesh.
*/
class SE_GRAPHICS_EXPORT MeshPart : public NamedObject
{
protected:
	Mesh* _parentMesh;
	PrimitiveType _primitiveType;
	uint32 _primitiveCount;
	VertexData* _vertexData;
	IndexData* _indexData;
	bool _isIndexed;
	uint32 _startVertex;
	uint32 _startIndex;
	ShaderMaterial* _shader;

public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	MeshPart();

	/** Destructor. */
	virtual ~MeshPart();
	//@}

	/** Clone this object. */
	MeshPart* Clone();

	/** @name Properties. */
	//@{
	/** Gets the parent model. */
	Mesh* GetParentMesh() const { return _parentMesh; }
	// Internal
	void SetParentMesh(Mesh* value) { _parentMesh = value; }

	PrimitiveType GetPrimitiveType() const { return _primitiveType; }
	void SetPrimitiveType(PrimitiveType value) { _primitiveType = value; }

	uint32 GetPrimitiveCount() const { return _primitiveCount; }
	void SetPrimitiveCount(uint32 value) { _primitiveCount = value; }

	void SetPrimitiveTypeAndCount(PrimitiveType value, uint32 count);

	VertexData* GetVertexData() { return _vertexData; }
	// Internal
	void SetVertexData(VertexData* value) { _vertexData = value; }

	IndexData* GetIndexData() { return _indexData; }
	// Internal
	void SetIndexData(IndexData* value) { _indexData = value; }

	bool IsIndexed() const { return _isIndexed; }
	void SetIndexed(bool value) { _isIndexed = value; }

	uint32 GetStartVertex() const { return _startVertex; }
	void SetStartVertex(uint32 value) { _startVertex = value; }

	uint32 GetStartIndex() const { return _startIndex; }
	void SetStartIndex(uint32 value) { _startIndex = value; }

	ShaderMaterial* GetShader() const { return _shader; }
	void SetShader(ShaderMaterial* value);
	//@}

	void GetRenderData(RenderData& renderData, uint16 lodIndex = 0) const;
};

typedef SmartPtr<MeshPart> MeshPartPtr;

}

#endif
