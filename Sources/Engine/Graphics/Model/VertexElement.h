/*=============================================================================
VertexElement.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_VERTEXELEMENT_H_
#define _SE_VERTEXELEMENT_H_

#include "Core/Core.h"
#include "Graphics/Common.h"

namespace SonataEngine
{

/* Vertex Semantics. */
enum VertexSemantic
{
	VertexSemantic_Position,
	VertexSemantic_BlendWeight,
	VertexSemantic_BlendIndices,
	VertexSemantic_Normal,
	VertexSemantic_PSize,
	VertexSemantic_TextureCoordinate,
	VertexSemantic_Tangent,
	VertexSemantic_Binormal,
	VertexSemantic_TesselateFactor,
	VertexSemantic_PositionTransformed,
	VertexSemantic_Color,
	VertexSemantic_Fog,
	VertexSemantic_Depth,
	VertexSemantic_Sample3
};

/* Vertex Formats. */
enum VertexFormat
{
	VertexFormat_Float1,
	VertexFormat_Float2,
	VertexFormat_Float3,
	VertexFormat_Float4,
	VertexFormat_Color,
	VertexFormat_UByte4,
	VertexFormat_Short2,
	VertexFormat_Short4,
	VertexFormat_UByte4N,
	VertexFormat_Short2N,
	VertexFormat_Short4N,
	VertexFormat_UShort2N,
	VertexFormat_UShort4N,
	VertexFormat_UDec3,
	VertexFormat_Dec3N,
	VertexFormat_Float16Two,
	VertexFormat_Float16Four,
	VertexFormat_Unused
};

/** Vertex Element. */
class SE_GRAPHICS_EXPORT VertexElement
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	VertexElement();

	/** Constructor. */
	VertexElement(uint16 stream, uint16 offset, VertexFormat format, VertexSemantic semantic, uint8 semanticIndex = 0);

	/** Destructor. */
	virtual ~VertexElement();
	//@}

	/** Retrieves the stream number (or index) to use. */
	uint16 GetStream() const { return _stream; }

	/** Sets the stream number (or index) to use. */
	void SetStream(uint16 value) { _stream = value; }

	/** Retrieves the offset (if any) from the beginning of the stream to the beginning of the vertex data. */
	uint16 GetOffset() const { return _offset; }

	/** Sets the offset (if any) from the beginning of the stream to the beginning of the vertex data. */
	void SetOffset(uint16 value) { _offset = value; }

	/** Retrieves the data format that defines the data size for the vertex element. */
	VertexFormat GetVertexFormat() const { return _vertexFormat; }
	
	/** Sets the data format that defines the data size for the vertex element. */
	void SetVertexFormat(VertexFormat value) { _vertexFormat = value; }

	/** Retrieves one or more semantic values that define the intended use of the vertex data. */
	VertexSemantic GetVertexSemantic() const { return _vertexSemantic; }
	
	/** Sets one or more semantic values that define the intended use of the vertex data. */
	void SetVertexSemantic(VertexSemantic value) { _vertexSemantic = value; }

	/** Retrieves the semantic index. */
	uint8 GetSemanticIndex() const { return _semanticIndex; }
	
	/** Sets the semantic index. */
	void SetSemanticIndex(uint8 value) { _semanticIndex = value; }

	/** Retrieves the size of the element. */
	uint16 GetSize() const { return VertexElement::GetTypeSize(_vertexFormat); };

	static uint16 GetTypeSize(VertexFormat format);
	static uint16 GetTypeComponents(VertexFormat format);

	bool operator==(const VertexElement& other) const;
	bool operator!=(const VertexElement& other) const;

protected:
	VertexSemantic _vertexSemantic;
	uint8 _semanticIndex;
	VertexFormat _vertexFormat;
	uint16 _stream;
	uint16 _offset;
};

}

#endif
