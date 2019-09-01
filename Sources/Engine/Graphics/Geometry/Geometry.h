/*=============================================================================
Geometry.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_GEOMETRY_H_
#define _SE_GEOMETRY_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Model/MeshPart.h"

namespace SonataEngine
{

/**
	@brief Geometry.

	Represents a geometry.
*/
class SE_GRAPHICS_EXPORT Geometry
{
public:
	typedef Array<Vector3> VertexArray;
	typedef Array<Vector3> NormalArray;
	typedef Array<Vector2> TextureCoordArray;
	typedef Array<Color32> ColorArray;
	typedef Array<int32> IndexArray;

protected:
	PrimitiveType _primitiveType;
	VertexArray _vertices;
	NormalArray _normals;
	ColorArray _colors;
	BaseArray<TextureCoordArray> _textureCoords;
	IndexArray _indices;
	BoundingBox _boundingBox;
	BoundingSphere _boundingSphere;

public:
	Geometry();

	virtual ~Geometry();

	PrimitiveType GetPrimitiveType() const { return _primitiveType; }
	void SetPrimitiveType(PrimitiveType value) { _primitiveType = value; }

	VertexArray& GetVertices() { return _vertices; }
	void SetVertices(const VertexArray& value) { _vertices = value; }

	NormalArray& GetNormals() { return _normals; }
	void SetNormals(const NormalArray& value) { _normals = value; }

	ColorArray& GetColors() { return _colors; }
	void SetColors(const ColorArray& value) { _colors = value; }

	int GetTextureCoordUnits() { return _textureCoords.Count(); }
	void SetTextureCoordUnits(int value);

	TextureCoordArray& GetTextureCoords(int unit = 0);
	void SetTextureCoords(const TextureCoordArray& value, int unit = 0);

	BaseArray<TextureCoordArray>& GetTextureCoordArrays() { return _textureCoords; }
	void SetTextureCoordArrays(const BaseArray<TextureCoordArray>& value) { _textureCoords = value; }

	IndexArray& GetIndices() { return _indices; }
	void SetIndices(const IndexArray& value) { _indices = value; }

	/**
		Returns the bounding box of the geometry.
		@return The bounding box of the geometry.
	*/
	const BoundingBox& GetBoundingBox() const { return _boundingBox; }

	/**
		Returns the bounding box of the geometry.
		@return The bounding box of the geometry.
	*/
	const BoundingSphere& GetBoundingSphere() const { return _boundingSphere; }

	/**
		Returns whether the geometr is valid.
		@return true if the geometry is valid; otherwise, false.
	*/
	bool IsValid();

	/**
		Computes the normals from the vertices.
		@remarks The existing normal array will be replaced.
	*/
	void ComputeNormals();

	/**	Updates the bounds of the geometry. */
	void UpdateBounds();

	/**
		Creates a primitive from the geometry.
		@todo creation options.
	*/
	bool CreatePrimitive(MeshPart** primitive);
};

}

#endif
